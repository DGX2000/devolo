#ifndef DC6_H
#define DC6_H

#include <string>
#include <vector>

#include "bitstream.h"

class DC6
{
public:
    DC6(const std::string& filename);

    std::vector<std::uint8_t> renderFrameToARGB(std::uint32_t direction, std::uint32_t frame) const;

    std::uint32_t getFrameWidth(std::uint32_t direction, std::uint32_t frame) const;
    std::uint32_t getFrameHeight(std::uint32_t direction, std::uint32_t frame) const;
    std::uint32_t getDirections() const;
    std::uint32_t getFramesPerDirection() const;

private:
    std::vector<std::uint8_t> renderFrameToIndices(std::uint32_t direction, std::uint32_t frame) const;

private:
    std::string filename;

    struct Header
    {
        std::uint32_t version;
        std::uint32_t bitFormat;
        std::uint32_t encodingFormat;
        std::uint32_t padding;
        std::uint32_t nDirections;
        std::uint32_t nFramesPerDirection;

        void fillFromBitStream(BitStream& stream);
    } header;

    struct Frame
    {
        std::uint32_t isFlipped;
        std::uint32_t width;
        std::uint32_t height;
        std::int32_t offset_x;
        std::int32_t offset_y; // scanlines go from the bottom to the top
        std::uint32_t unknown; // ignore
        std::uint32_t next_block; // ignore
        std::uint32_t length;
        std::vector<std::uint8_t> data;

        void fillFromBitStream(BitStream& stream);
    };

    std::vector<std::uint32_t> pointers;
    std::vector<Frame> frames;

    // TODO: make palette its own class later on
    std::vector<std::uint8_t> palette;
};

#endif // DC6_H
