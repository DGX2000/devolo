#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <cinttypes>
#include <string>
#include <vector>

class BitStream
{
public:
    static BitStream fromFile(const std::string& filename);

    template<typename T, bool bigEndian = false>
    T extract(std::uint64_t bits = 0)
    {
        // Only works for less than or equal to 32 bits
        // (this is fine for Diablo 2, as there are no 64-bit datatypes)
        if constexpr(sizeof(T) <= 4)
        {
            unsigned long bitsToExtract{0};
            if(bits > 0)
            {
                bitsToExtract = bits;
            }
            else
            {
                bitsToExtract = sizeof(T) * 8;
            }

            auto currentByte = currentBit / 32;
            scratchpad.first = data.at(currentByte);
            if(currentByte < data.size()-1)
            {
                scratchpad.next = data.at(currentByte+1);
            }
            else
            {
                scratchpad.next = 0;
            }

            if(bigEndian)
            {
                scratchpad.first = swapByteOrder(scratchpad.first);
                scratchpad.next = swapByteOrder(scratchpad.next);
            }

            // discard already read bits
            scratchpad.whole <<= 64 - (currentBit + bitsToExtract);
            scratchpad.whole >>= 64 - bitsToExtract;


            // advance
            currentBit = (currentBit + bitsToExtract) % 32;

            return static_cast<T>(scratchpad.first);
        }
        else
        {
            // TODO: Error handling (better use a static assert instead of if constexpr)
            return T{};
        }
    }


private:
    BitStream(std::vector<std::uint32_t>&& data, std::uint64_t firstBit, std::uint64_t lastBit);

    // helper functions
    static std::uint64_t getFileSize(const std::string& filename);
    static std::uint64_t getNumberOfPaddingBytes(std::uint64_t numberOfBytes);
    static std::uint32_t swapByteOrder(std::uint32_t bytes);

private:
    std::vector<std::uint32_t> data;

    std::uint64_t firstBit;
    std::uint64_t lastBit;
    std::uint64_t currentBit;

private:
    union
    {
        struct
        {
            std::uint32_t first;
            std::uint32_t next;
        };
        std::uint64_t whole;
    } scratchpad;
};

#endif // BITSTREAM_H
