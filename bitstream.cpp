#include "bitstream.h"

#include <fstream>

BitStream BitStream::fromFile(const std::string &filename)
{
    std::uint64_t fileSize = getFileSize(filename);
    std::uint64_t fileSizeWithPadding = fileSize + getNumberOfPaddingBytes(fileSize);

    std::vector<uint32_t> data;
    data.resize(fileSizeWithPadding / sizeof(std::uint32_t));

    std::ifstream file(filename, std::ios::binary);
    file.read(reinterpret_cast<char*>(data.data()), fileSize);

    return BitStream(std::move(data), 0, fileSize * 8);
}

BitStream::BitStream(std::vector<uint32_t> &&data, uint64_t firstBit, uint64_t lastBit)
    : data(data), firstBit(firstBit), lastBit(lastBit)
{
    currentBit = firstBit;
    scratchpad.whole = 0;
}

uint64_t BitStream::getFileSize(const std::string &filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    std::uint64_t beginningIndex = file.tellg();
    file.seekg(0, std::ios::end);
    std::uint64_t endIndex = file.tellg();

    return endIndex - beginningIndex;
}

uint64_t BitStream::getNumberOfPaddingBytes(uint64_t numberOfBytes)
{
    std::uint64_t neededPaddingBytes =
            sizeof(std::uint32_t) - (numberOfBytes % sizeof(std::uint32_t));
    return (neededPaddingBytes == sizeof(std::uint32_t)) ? 0 : neededPaddingBytes;
}

uint32_t BitStream::swapByteOrder(uint32_t bytes)
{
    return (bytes >> 24) | ((bytes << 8) & 0x00FF0000) |
            ((bytes >> 8) & 0x0000FF00) | (bytes << 24);
}
