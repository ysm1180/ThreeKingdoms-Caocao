#include "MemoryStream.h"

namespace three_kingdoms
{
size_t CMemoryBuffer::Length()
{
    return _length;
}

int CMemoryStream::Size()
{
    return _buffer.Length();
}
} // namespace three_kingdoms
