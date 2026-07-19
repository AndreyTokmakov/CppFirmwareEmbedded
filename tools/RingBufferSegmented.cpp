/**============================================================================
Name        : RingBufferSegmented.cpp
Created on  : 19.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : RingBufferSegmented.cpp
============================================================================**/

#include "tools.hpp"

#include <iostream>
#include <array>
#include <span>
#include <algorithm>
#include <cstring>
#include <string_view>

namespace
{
    template<typename Ty, std::size_t Capacity>
    class RingBuffer
    {
        using value_type = Ty;
        using size_type  = size_t;

        static_assert(Capacity > 0, "Capacity must be greater than zero");
        static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be power of two");

    public:

        struct Segments
        {
            std::span<value_type> first;
            std::span<value_type> second;

            [[nodiscard]]
            size_type size() const noexcept {
                return first.size() + second.size();
            }
        };

        struct ConstSegments
        {
            std::span<const value_type> first;
            std::span<const value_type> second;

            [[nodiscard]]
            size_type size() const noexcept {
                return first.size() + second.size();
            }
        };


        static constexpr size_type capacity() noexcept {
            return Capacity;
        }

        [[nodiscard]]
        size_type size() const noexcept {
            return mSize;
        }

        [[nodiscard]]
        size_type freeSpace() const noexcept{
            return Capacity - mSize;
        }

        [[nodiscard]]
        bool empty() const noexcept {
            return mSize == 0;
        }

        [[nodiscard]]
        bool full() const noexcept {
            return mSize == Capacity;
        }


        void clear() noexcept
        {
            head = 0;
            tail = 0;
            mSize = 0;
        }

        /*
         * Memory available for writing.
         * Example:
         * +----------------------+
         * |######......##########|
         * +----------------------+
         *        ^
         *        tail
         *
         * returns two spans
         */
        Segments prepareWrite() noexcept
        {
            Segments result {};
            if (full()) {
                return result;
            }

            if (const size_type available = freeSpace(); tail >= head)
            {
                const size_type firstSize = std::min(Capacity - tail, available);
                const size_type secondSize = available - firstSize;

                result.first  = { buffer.data() + tail, firstSize };
                result.second = { buffer.data(), secondSize };
            } else {
                result.first  = { buffer.data() + tail, head - tail };
            }
            return result;
        }

        void commitWrite(const size_type count) noexcept
        {
            // assert(count <= freeSpace());
            tail = (tail + count) & Mask;
            mSize += count;
        }

        /*
         * Data available for reading.
         * Example:
         *   +----------------------+
         *   |######......##########|
         *   +----------------------+
         *   ^
         *   head
         */
        [[nodiscard]]
        ConstSegments prepareRead() const noexcept
        {
            ConstSegments result {};
            if (empty()) {
                return result;
            }

            if (head < tail) {
                result.first  = { buffer.data() + head, tail - head };
            } else {
                result.first  = { buffer.data() + head, Capacity - head };
                result.second = { buffer.data(), tail };
            }
            return result;
        }

        void commitRead(const size_type count) noexcept
        {
            // assert(count <= mSize);
            head = (head + count) & Mask;
            mSize -= count;
        }


    private:
        static constexpr size_type Mask { Capacity - 1 };

        std::array<value_type, Capacity> buffer {};
        size_type head {0};
        size_type tail {0};
        size_type mSize {0};
    };
}


namespace
{
    struct UartSimulator
    {
        explicit UartSimulator(const std::string_view data): mData(data){
        }

        std::size_t read(std::span<std::byte> buffer)
        {
            if (mOffset >= mData.size()){
                return 0;
            }

            constexpr std::size_t ChunkSize = 7;
            const std::size_t count = std::min({ ChunkSize, buffer.size(), mData.size() - mOffset });
            std::memcpy(buffer.data(),mData.data() + mOffset, count);

            mOffset += count;
            return count;
        }

    private:

        std::string_view mData;
        std::size_t mOffset { 0 };
    };


    void processBuffer(RingBuffer<std::byte, 32>& buffer)
    {
        while (true)
        {
            auto readable = buffer.prepareRead();
            std::cout << "\tReadable { first: " << readable.first.size() << " second: " << readable.first.size() << " }\n";

            if (readable.size() == 0) {
                return;
            }

            std::size_t consumed = 0;
            bool found = false;

            auto processSpan = [&](const std::span<const std::byte> span) {
                for (const auto i : span) {
                    ++consumed;
                    if (i == std::byte{'\n'}){
                        found = true;
                        return;
                    }
                }
            };


            processSpan(readable.first);
            if (!found) {
                processSpan(readable.second);
            }
            if (!found) {
                return;
            }

            buffer.commitRead(consumed);
            std::cout << "Complete message received\n";
        }
    }

    void demo()
    {
        RingBuffer<std::byte, 32> rxBuffer;
        UartSimulator uart(
            "Hello\n"
            "World\n"
            "UART RingBuffer Demo\n"
        );


        while (true)
        {
            auto writable = rxBuffer.prepareWrite();
            std::cout << "\tWritable { first: " << writable.first.size() << " second: " << writable.first.size() << " }\n";

            std::size_t bytesRead = 0;

            if (!writable.first.empty()) {
                bytesRead += uart.read(writable.first);
            }

            if (bytesRead == writable.first.size() && !writable.second.empty()) {
                bytesRead += uart.read(writable.second);
            }
            if (bytesRead == 0) {
                break;
            }

            rxBuffer.commitWrite(bytesRead);
            processBuffer(rxBuffer);
        }
    }
}

/**
 * RingBuffer
 * ==========
 *
 * This class implements a fixed-capacity circular buffer optimized for
 * streaming I/O, such as UART, TCP sockets, pipes, or other byte-oriented
 * data sources.
 *
 * Design goals:
 *   - No dynamic memory allocations.
 *   - Zero-copy data flow whenever possible.
 *   - Constant-time operations.
 *   - Compile-time capacity validation (capacity must be a power of two).
 *   - Efficient index wrapping using a bit mask instead of modulo.
 *
 * Unlike a traditional queue with push()/pop(), this RingBuffer exposes
 * contiguous memory regions through prepareWrite() and prepareRead().
 *
 * Write path:
 *   1. prepareWrite() returns up to two writable spans because free space may wrap around the end of the internal array.
 *   2. The caller writes directly into these spans (typically via read()).
 *   3. commitWrite() advances the write position by the number of bytes  actually received.
 *
 * Read path:
 *   1. prepareRead() returns up to two readable spans because valid data may also wrap around.
 *   2. The parser processes the data directly from these spans without copying it into another buffer.
 *   3. commitRead() removes the consumed bytes from the buffer.
 *
 * This design avoids unnecessary memcpy() operations and makes the buffer
 * suitable for high-performance streaming applications.
 *
 *
 * Demo overview
 * -------------
 *
 * The demo simulates a UART device by returning incoming data in small,
 * irregular chunks, similar to how a real serial port behaves.
 *
 * The application repeatedly:
 *
 *   UART Simulator
 *          |
 *          v
 *   prepareWrite()
 *          |
 *          v
 *   read() directly into RingBuffer
 *          |
 *          v
 *   commitWrite()
 *          |
 *          v
 *   prepareRead()
 *          |
 *          v
 *   Parser
 *          |
 *          v
 *   commitRead()
 *
 * The parser searches for complete messages terminated by '\n'. Since the
 * RingBuffer may split data across the end of the internal array, the parser
 * consumes both returned spans as if they formed one continuous stream.
 *
 * The demo demonstrates the intended production workflow:
 *
 *   Kernel/UART
 *        |
 *        v
 *     read()
 *        |
 *        v
 *   RingBuffer
 *        |
 *        v
 *     Parser
 *
 * No intermediate buffers are allocated and no additional copies are made.
 **/

void tools::ring_buffer_segmented::testAll()
{
    demo();

    /*
        Writable { first: 32 second: 32 }
        Readable { first: 7 second: 7 }
    Complete message received
        Readable { first: 1 second: 1 }
        Writable { first: 25 second: 25 }
        Readable { first: 8 second: 8 }
    Complete message received
        Readable { first: 2 second: 2 }
        Writable { first: 18 second: 18 }
        Readable { first: 9 second: 9 }
        Writable { first: 11 second: 11 }
        Readable { first: 16 second: 16 }
        Writable { first: 4 second: 4 }
        Readable { first: 20 second: 20 }
    Complete message received
        Writable { first: 31 second: 31 }
    */
}
