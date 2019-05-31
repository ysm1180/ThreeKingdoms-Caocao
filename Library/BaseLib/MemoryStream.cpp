#include "MemoryStream.h"

namespace jojogame
{
size_t CMemoryBuffer::Length()
{
    return _length;
}

int CMemoryStream::Size()
{
    return _buffer.Length();
}
} // namespace jojogame
