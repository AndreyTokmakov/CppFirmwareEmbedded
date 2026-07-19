/**============================================================================
Name        : SerialPortReader.cpp
Created on  : 19.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : SerialPortReader.cpp
============================================================================**/

#include "uart.hpp"

#include <iostream>
#include <print>
#include <format>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <iomanip>


#include <cerrno>
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <cstring>

namespace
{
    using Handle    = int32_t;
    using Byte      = uint8_t;
    using size_type = size_t;
    constexpr Handle InvalidHandle { -1 };
}

namespace
{
    struct Buffer
    {
        constexpr static size_type BufferSize { 32 * 1024 };

        size_type head { 0 };
        size_type tail { 0 };
        std::array<Byte, BufferSize> data{};
    };

    // TODO: Decoder --> use concepts
    //   1. Constructor(Buffer)
    //   2. Parse
    template<typename Decoder>
    struct SerialPort
    {
        explicit SerialPort(const std::string& device,
                            const speed_t baud = B115200): decoder(buffer)
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

        void eventLoop()
        {
            pollfd pfd {
                .fd = hSerial,
                .events = POLLIN,
            };
            while (true)
            {
                pfd.fd = hSerial;
                pfd.events = POLLIN;

                if (const int ret = ::poll(&pfd, 1, PollTimeoutMs); InvalidHandle == ret){
                    throw std::runtime_error(std::strerror(errno));
                } else if (ret == 0) {
                    continue;
                }

                if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)){
                    throw std::runtime_error("UART disconnected");
                }
                if (pfd.revents & POLLIN)
                {
                    const ssize_t bytes =::read(hSerial, buffer.data.data() + buffer.head,
                                                Buffer::BufferSize - buffer.head);
                    if (0 >= bytes) {
                        continue;
                    }
                    decoder.parse(bytes);
                }
            }
        }

        [[nodiscard]]
        ssize_t write(const Byte* buffer, const size_t size) const
        {
            return ::write(hSerial, buffer, size);
        }

    private:

        void open(const std::string& device, const speed_t baud)
        {
            hSerial = ::open(device.c_str(),O_RDWR | O_NOCTTY | O_NONBLOCK);
            if (InvalidHandle == hSerial) {
                throw std::runtime_error(std::strerror(errno));
            }

            termios tty{};
            if (::tcgetattr(hSerial, &tty) != 0){
                throw std::runtime_error(std::strerror(errno));
            }

            ::cfsetispeed(&tty, baud);
            ::cfsetospeed(&tty, baud);

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

            ::tcflush(hSerial, TCIOFLUSH);
            if (::tcsetattr(hSerial, TCSANOW, &tty) != 0) {
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

        constexpr static int32_t PollTimeoutMs { 3'000 };

        Handle hSerial { InvalidHandle };
        Buffer buffer;
        Decoder decoder;
    };
}

namespace
{
    struct TestDecoder
    {
        Buffer& buffer;

        void parse( const ssize_t bytes)
        {
            std::cout << "Received " << bytes << " bytes\n";
            std::cout << "Haad: " << buffer.head << ": ";
            for (ssize_t idx = 0; idx < bytes; ++idx)
            {
                std::print("{:02x} ", buffer.data[idx + buffer.head]);
            }
            buffer.head += bytes;
            std::cout << '\n';
        }
    };
}

namespace
{
    void run()
    {

        SerialPort<TestDecoder> serial("/dev/ttyUSB0", B115200);
        serial.eventLoop();
    }
}

void uart::serial_port_reader::testAll()
{
    run();
}
