#include "dc6.h"

DC6::DC6(const std::string& filename)
{
    auto file = BitStream::fromFile(filename);

    header.fillFromBitStream(file);

    // read table of pointers
    auto totalFrames = header.nDirections * header.nFramesPerDirection;
    pointers.resize(totalFrames);
    for(auto& pointer : pointers)
    {
        pointer = file.extract<std::uint32_t>();
    }

    // read frames
    frames.resize(totalFrames);
    for(std::uint32_t i = 0; i < totalFrames; ++i)
    {
        file.seekGlobal(pointers.at(i));
        frames.at(i).fillFromBitStream(file);
    }

    // create a palette
    // TODO: Make own palette class
    palette.resize(3 * 256);
    uint8_t value = 0u;
    for(unsigned int i = 0u; i < 256; i++, value++) {
        palette[i*3] = value;     // B
        palette[i*3+1] = value;   // G
        palette[i*3+2] = value;   // R
    }
}

std::vector<uint8_t> DC6::renderFrameToARGB(uint32_t direction, uint32_t frame) const
{
    auto indices = renderFrameToIndices(direction, frame);

    std::vector<uint8_t> pixels(4 * indices.size());

    for(std::size_t i = 0; i < indices.size(); ++i)
    {
        pixels.at(i*4)   = palette.at(indices.at(i) * 3 + 2); // red
        pixels.at(i*4+1) = palette.at(indices.at(i) * 3 + 1); // green
        pixels.at(i*4+2) = palette.at(indices.at(i) * 3);     // blue
        pixels.at(i*4+3) = 0xFF;                              // alpha
    }

    return pixels;
}

uint32_t DC6::getFrameWidth(uint32_t direction, uint32_t frame) const
{
    auto selectedFrame = direction * header.nFramesPerDirection + frame;
    return frames.at(selectedFrame).width;
}

uint32_t DC6::getFrameHeight(uint32_t direction, uint32_t frame) const
{
    auto selectedFrame = direction * header.nFramesPerDirection + frame;
    return frames.at(selectedFrame).height;
}

uint32_t DC6::getDirections() const
{
    return header.nDirections;
}

uint32_t DC6::getFramesPerDirection() const
{
    return header.nFramesPerDirection;
}

std::vector<std::uint8_t> DC6::renderFrameToIndices(std::uint32_t direction, std::uint32_t frame) const
{
    auto selectedFrame = direction * header.nFramesPerDirection + frame;

    auto length = frames.at(selectedFrame).width * frames.at(selectedFrame).height;
    std::vector<std::uint8_t> indices(length);

    // TODO: clean this up
    auto height = frames.at(selectedFrame).height;
    int x = 0;
    int y = height - 1;
    int p = 0;
    while(y >= 0)
    {
        std::uint8_t currentByte = frames.at(selectedFrame).data[p++];

        if(currentByte < 0x80)
        {
            for(std::uint8_t i = 0u; i < currentByte; ++i, ++p)
            {
                indices[y * height + x + i] = frames[selectedFrame].data[p];
            }

            x += currentByte;
        }
        else if(currentByte > 0x80)
        {
            // skip bytes (transparent pixels)
            x += (currentByte - 0x80);
        }
        else
        {
            x = 0;
            --y;
        }
    }

    return indices;
}

void DC6::Header::fillFromBitStream(BitStream &stream)
{
    version = stream.extract<std::uint32_t>();
    bitFormat = stream.extract<std::uint32_t>();
    encodingFormat = stream.extract<std::uint32_t>();
    padding = stream.extract<std::uint32_t>();
    nDirections = stream.extract<std::uint32_t>();
    nFramesPerDirection = stream.extract<std::uint32_t>();
}

void DC6::Frame::fillFromBitStream(BitStream &stream)
{
    isFlipped = stream.extract<std::uint32_t>();
    width  = stream.extract<std::uint32_t>();
    height  = stream.extract<std::uint32_t>();
    offset_x  = stream.extract<std::int32_t>();
    offset_y  = stream.extract<std::int32_t>(); // scanlines go from the bottom to the top
    unknown  = stream.extract<std::uint32_t>(); // ignore
    next_block  = stream.extract<std::uint32_t>(); // ignore
    length  = stream.extract<std::uint32_t>();

    data.reserve(length);
    for(std::uint32_t i = 0; i < length; ++i)
    {
        data.emplace_back(stream.extract<std::uint8_t>());
    }
}
