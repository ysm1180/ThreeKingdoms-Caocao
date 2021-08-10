#include "memory_stream.h"

namespace three_kingdoms {
size_t MemoryBuffer::Length() { return _length; }

int MemoryStream::Size() { return _buffer.Length(); }
}  // namespace three_kingdoms