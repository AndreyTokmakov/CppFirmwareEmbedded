/**============================================================================
Name        : experiments.cpp
Created on  : 
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : C++ UART Experiments
============================================================================**/

#include "uart.hpp"

#include <iostream>
#include <print>
#include <format>
#include <string_view>
#include <array>
#include <vector>
#include <thread>
#include <algorithm>
#include <cerrno>
#include <poll.h>
#include <stdexcept>
#include <string>

#include <termios.h>
#include <unistd.h>

#include <sys/epoll.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iomanip>

namespace
{
    using Handle = int32_t;
    using size_type = size_t;
    constexpr Handle InvalidHandle { -1 };
}

namespace uart::experiments
{
    struct SerialPort
    {
        ~SerialPort() {
            close();
        }

        bool open(const std::string& device, const speed_t baud)
        {
            hSerial = ::open(device.c_str(), O_RDWR | O_NOCTTY);
            if (hSerial < 0)
            {
                std::cerr << "Open failed: " << strerror(errno)<< '\n';
                return false;
            }

            termios tty{};

            if (tcgetattr(hSerial, &tty) != 0)
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

            tcflush(hSerial, TCIOFLUSH);

            if (tcsetattr(hSerial, TCSANOW, &tty) != 0)
            {
                std::cerr << "tcsetattr failed\n";
                return false;
            }

            return true;
        }

        void close()
        {
            if (hSerial >= 0)
            {
                ::close(hSerial);
                hSerial = -1;
            }
        }

        [[nodiscard]]
        bool write(const std::vector<uint8_t>& data) const
        {
            const ssize_t result = ::write(hSerial, data.data(), data.size());
            return result == std::ssize(data);
        }

        ssize_t read(uint8_t* buffer, const size_t size) const
        {
            return ::read(hSerial, buffer, size);
        }

    private:
        Handle hSerial { -InvalidHandle };
    };


    void demo()
    {
        SerialPort uart;
        if (!uart.open("/dev/ttyUSB0", B115200)) {
            return;
        }

        const std::vector<uint8_t> packet { 0xAA, 0x55, 0x01, 0x02 };
        const bool _  = uart.write(packet);

        uint8_t buffer[256];
        while (true)
        {
            ssize_t n = uart.read(buffer, sizeof(buffer));
            if (n > 0) {
                std::cout << "Received " << n << " bytes\n";
            }
        }
    }

    void readBytes()
    {
        SerialPort uart;
        if (!uart.open("/dev/ttyUSB0", B115200)) {
            return;
        }

        uint8_t buffer[256];
        while (true)
        {
            const ssize_t bytesRead = uart.read(buffer, sizeof(buffer));
            if (bytesRead > 0)
            {
                std::print("Received {} bytes. [", bytesRead);
                for (int i = 0; i < bytesRead; ++i) {
                    std::printf("%02X ", buffer[i]);
                }
                std::println("]");
            }
        }
    }
}

namespace uart::experiments::poll_seria_port
{
    struct SerialPort
    {
        explicit SerialPort(const std::string& device,
                            const speed_t baud = B115200)
        {
            open(device, baud);
        }

        ~SerialPort()
        {
            close();
        }

        SerialPort(const SerialPort&) = delete;
        SerialPort& operator=(const SerialPort&) = delete;

        SerialPort(SerialPort&& other) noexcept
        {
            hSerial = other.hSerial;
            other.hSerial = -1;
        }

        SerialPort& operator=(SerialPort&& other) noexcept
        {
            if (this != &other)
            {
                if (hSerial >= 0) {
                    ::close(hSerial);
                }

                hSerial = other.hSerial;
                other.hSerial = InvalidHandle;
            }

            return *this;
        }

        [[nodiscard]]
        bool waitForData(const int timeoutMs) const
        {
            pollfd pfd{};
            pfd.fd = hSerial;
            pfd.events = POLLIN;

            const int ret = ::poll(&pfd, 1, timeoutMs);
            if (ret < 0){
                throw std::runtime_error(std::strerror(errno));
            }
            if (ret == 0) {
                return false;
            }
            if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)){
                throw std::runtime_error("UART disconnected");
            }
            return (pfd.revents & POLLIN);
        }

        ssize_t read(uint8_t* buffer, const size_t size) const
        {
            return ::read(hSerial, buffer, size);
        }

        ssize_t write(const uint8_t* buffer, const size_t size) const
        {
            return ::write(hSerial, buffer, size);
        }

    private:

        void open(const std::string& device, speed_t baud)
        {
            hSerial = ::open(device.c_str(),
                          O_RDWR |
                          O_NOCTTY |
                          O_NONBLOCK);

            if (hSerial < 0)
            {
                throw std::runtime_error(std::strerror(errno));
            }

            termios tty{};

            if (tcgetattr(hSerial, &tty) != 0)
            {
                throw std::runtime_error(std::strerror(errno));
            }

            cfsetispeed(&tty, baud);
            cfsetospeed(&tty, baud);

            tty.c_cflag |= (CLOCAL | CREAD);
            tty.c_cflag &= ~PARENB;
            tty.c_cflag &= ~CSTOPB;
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS8;

            tty.c_iflag = 0;
            tty.c_oflag = 0;
            tty.c_lflag = 0;

            tty.c_cc[VMIN] = 0;
            tty.c_cc[VTIME] = 0;

            tcflush(hSerial, TCIOFLUSH);

            if (tcsetattr(hSerial, TCSANOW, &tty) != 0)
            {
                throw std::runtime_error(std::strerror(errno));
            }
        }

        void close()
        {
            if (hSerial >= 0)
            {
                ::close(hSerial);
                hSerial = InvalidHandle;
            }
        }

    private:

        Handle hSerial { -InvalidHandle };
    };

    void run()
    {
        const SerialPort serial("/dev/ttyUSB0", B115200);
        std::array<uint8_t, 1024> buffer{};
        while (true)
        {
            if (!serial.waitForData(3000))
            {
                std::cout << "Timeout\n";
                continue;
            }

            const auto bytes = serial.read(buffer.data(), buffer.size());
            if (bytes <= 0){
                continue;
            }

            std::cout << "Received " << bytes << " bytes\n";
            for (int i = 0; i < bytes; ++i)
            {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                    << static_cast<int>(buffer[i])<< ' ';
            }

            std::cout << '\n';
        }
    }
}

namespace uart::experiments::epoll_seria_port
{
    class SerialPort
    {
    public:

        SerialPort(const std::string& device,
                   const speed_t baud = B115200)
        {
            open(device, baud);
            createEpoll();
        }

        ~SerialPort()
        {
            if (hEpollFd >= 0) {
                ::close(hEpollFd);
            }
            if (hSerial >= 0) {
                ::close(hSerial);
            }
        }

        SerialPort(const SerialPort&) = delete;
        SerialPort& operator=(const SerialPort&) = delete;

        ssize_t read(uint8_t* buffer,
                     const size_t size,
                     const int timeoutMs) const
        {
            epoll_event event{};

            const Handle ret = ::epoll_wait(hEpollFd, &event, 1, timeoutMs);
            if (ret < 0) {
                throw std::runtime_error(std::strerror(errno));
            }
            if (ret == 0) {
                return 0; // timeout
            }
            if (event.events & (EPOLLERR | EPOLLHUP)) {
                throw std::runtime_error("UART disconnected");
            }

            return ::read(hSerial, buffer, size);
        }

        ssize_t write(const uint8_t* data, const size_t size) const
        {
            return ::write(hSerial, data, size);
        }

    private:

        void open(const std::string& device, const speed_t baud)
        {
            hSerial = ::open(device.c_str(),O_RDWR | O_NOCTTY | O_NONBLOCK);
            if (hSerial < 0) {
                throw std::runtime_error(std::strerror(errno));
            }

            termios tty{};

            tcgetattr(hSerial, &tty);

            cfsetispeed(&tty, baud);
            cfsetospeed(&tty, baud);

            tty.c_cflag |= CLOCAL;
            tty.c_cflag |= CREAD;

            tty.c_cflag &= ~PARENB;
            tty.c_cflag &= ~CSTOPB;
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS8;

            tty.c_iflag = 0;
            tty.c_oflag = 0;
            tty.c_lflag = 0;

            tty.c_cc[VMIN] = 0;
            tty.c_cc[VTIME] = 0;

            tcsetattr(hSerial, TCSANOW, &tty);
        }

        void createEpoll()
        {
            hEpollFd =::epoll_create1(0);
            if (hEpollFd < 0){
                throw std::runtime_error(std::strerror(errno));
            }

            epoll_event event {
                .events = EPOLLIN,
                .data = epoll_data_t{ .fd = hSerial }
            };

            if (InvalidHandle == ::epoll_ctl(hEpollFd,EPOLL_CTL_ADD, hSerial,&event)) {
                throw std::runtime_error(std::strerror(errno));
            }
        }

    private:

        Handle hSerial { InvalidHandle };
        Handle hEpollFd { InvalidHandle };
    };

    static void run()
    {
        const SerialPort uart("/dev/ttyUSB0");
        std::array<uint8_t, 4096> buffer{};
        while (true)
        {
            const ssize_t bytes = uart.read(buffer.data(), buffer.size(), 5000);
            if (bytes == 0)
            {
                std::cout << "timeout\n";
                continue;
            }
            std::cout << "Received " << bytes<< " bytes\n";
        }
    }
}

void uart::experiments::testAll()
{
    using namespace experiments;

    // simple::readBytes();
    poll_seria_port::run();
    // epoll_seria_port::run();
}

