В Linux работа с UART сводится к работе с **serial port** через POSIX API. На практике используются системные вызовы:

* `open()`
* `read()`
* `write()`
* `tcgetattr()`
* `tcsetattr()`
* `select()` или `poll()` (неблокирующий ввод)
* `ioctl()` (дополнительные настройки)

Рассмотрим production-style пример.

---

# Подключение

Предположим, имеется устройство

```text
/dev/ttyUSB0
```

которое работает на

```text
115200 8N1
```

---

# UART класс

```cpp
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

class SerialPort
{
public:
    SerialPort() = default;

    ~SerialPort()
    {
        close();
    }

    bool open(const std::string& device, speed_t baud)
    {
        m_fd = ::open(device.c_str(), O_RDWR | O_NOCTTY);

        if (m_fd < 0)
        {
            std::cerr << "Open failed: "
                      << strerror(errno)
                      << '\n';
            return false;
        }

        termios tty{};

        if (tcgetattr(m_fd, &tty) != 0)
        {
            std::cerr << "tcgetattr failed\n";
            return false;
        }

        cfsetispeed(&tty, baud);
        cfsetospeed(&tty, baud);

        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;

        tty.c_cflag |= CLOCAL;
        tty.c_cflag |= CREAD;

        tty.c_iflag = 0;
        tty.c_oflag = 0;
        tty.c_lflag = 0;

        tty.c_cc[VMIN] = 1;
        tty.c_cc[VTIME] = 0;

        tcflush(m_fd, TCIOFLUSH);

        if (tcsetattr(m_fd, TCSANOW, &tty) != 0)
        {
            std::cerr << "tcsetattr failed\n";
            return false;
        }

        return true;
    }

    void close()
    {
        if (m_fd >= 0)
        {
            ::close(m_fd);
            m_fd = -1;
        }
    }

    bool write(const std::vector<uint8_t>& data)
    {
        ssize_t result =
            ::write(m_fd, data.data(), data.size());

        return result == static_cast<ssize_t>(data.size());
    }

    ssize_t read(uint8_t* buffer, size_t size)
    {
        return ::read(m_fd, buffer, size);
    }

private:
    int m_fd = -1;
};
```

---

# Использование

```cpp
#include <iostream>

int main()
{
    SerialPort uart;

    if (!uart.open("/dev/ttyUSB0", B115200))
    {
        return 1;
    }

    std::vector<uint8_t> packet =
    {
        0xAA,
        0x55,
        0x01,
        0x02
    };

    uart.write(packet);

    uint8_t buffer[256];

    while (true)
    {
        ssize_t n = uart.read(buffer, sizeof(buffer));

        if (n > 0)
        {
            std::cout << "Received "
                      << n
                      << " bytes\n";
        }
    }
}
```

Компиляция:

```bash
g++ main.cpp -std=c++20
```

---

# Что происходит внутри

Когда вызывается

```cpp
uart.write(packet);
```

ядро Linux начинает записывать байты в драйвер UART.

```
Application

      |

write()

      |

Linux tty subsystem

      |

UART driver

      |

UART Controller

      |

TX Pin
```

Передача происходит аппаратно, без участия приложения после записи в буфер драйвера.

---

# Неблокирующий режим

Часто UART открывают так:

```cpp
::open(
    "/dev/ttyUSB0",
    O_RDWR |
    O_NOCTTY |
    O_NONBLOCK);
```

Теперь

```cpp
read(...)
```

сразу возвращает управление.

Если данных нет

```text
errno = EAGAIN
```

---

# Ожидание данных через select()

Production-код редко крутится в бесконечном цикле. Вместо этого используют `select()` или `poll()`.

```cpp
fd_set set;

FD_ZERO(&set);
FD_SET(fd, &set);

timeval timeout{};
timeout.tv_sec = 1;

int ret =
    select(fd + 1,
           &set,
           nullptr,
           nullptr,
           &timeout);

if (ret > 0)
{
    read(fd, buffer, sizeof(buffer));
}
```

Поток "спит", пока драйвер не сообщит о появлении данных.

---

# Пример с GPS

GPS-модуль постоянно отправляет строки NMEA:

```text
$GPGGA,123456,...
$GPRMC,123457,...
$GPGSV,...
```

Чтение может выглядеть так:

```cpp
std::string line;

char c;

while (true)
{
    if (uart.read(reinterpret_cast<uint8_t*>(&c), 1) == 1)
    {
        if (c == '\n')
        {
            std::cout << line << '\n';
            line.clear();
        }
        else
        {
            line += c;
        }
    }
}
```

---

# Что обычно делают в production

В реальных проектах редко читают по одному байту. Обычно архитектура выглядит так:

```
               UART Interrupt
                     │
                     ▼
             Linux UART Driver
                     │
                     ▼
               read(4096 bytes)
                     │
                     ▼
              Ring Buffer (SPSC)
                     │
         ┌───────────┴───────────┐
         ▼                       ▼
  Packet Parser            Logging Thread
         │
         ▼
   Business Logic
```

Типичные практики:

* читать сразу большими блоками (например, 512–4096 байт), чтобы уменьшить число системных вызовов;
* накапливать данные в кольцевом буфере (`RingBuffer`);
* разбирать поток на сообщения отдельным парсером (по разделителям, длине пакета или заголовку);
* использовать `select()`, `poll()` или `epoll()` вместо активного ожидания;
* обрабатывать частичные чтения и записи — `read()` и `write()` не гарантируют обработку всего буфера за один вызов;
* корректно реагировать на ошибки (`EAGAIN`, `EINTR`, отключение устройства и т.д.).

Именно такой подход обычно встречается в Linux-приложениях, работающих с GPS-приемниками, промышленными контроллерами, модемами, датчиками и другим UART-оборудованием.
