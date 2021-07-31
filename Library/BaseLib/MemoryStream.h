#pragma once
#include <cstdint>
#include <streambuf>
#include <iostream>

namespace three_kingdoms
{

class CMemoryBuffer : public std::basic_streambuf<char>
{
public:
    CMemoryBuffer(const uint8_t *bytes, size_t len)
    {
        _length = len;
        setg((char *)bytes, (char *)bytes, (char *)bytes + len);
    }

    size_t Length();

private:
    size_t _length;
};

class CMemoryStream : public std::istream
{
public:
    CMemoryStream(const uint8_t *p, size_t l) : std::istream(&_buffer),
                                                _buffer(p, l)
    {
        rdbuf(&_buffer);
    }

    int Size();

private:
    CMemoryBuffer _buffer;
};

} // namespace jojogame
