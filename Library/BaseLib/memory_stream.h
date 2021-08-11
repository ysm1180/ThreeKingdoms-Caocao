#pragma once
#include <cstdint>
#include <iostream>
#include <streambuf>

namespace three_kingdoms {
class MemoryBuffer : public std::basic_streambuf<char> {
 public:
  MemoryBuffer(const uint8_t *bytes, size_t len) {
    _length = len;
    setg((char *)bytes, (char *)bytes, (char *)bytes + len);
  }

  size_t Length();

 private:
  size_t _length;
};

class MemoryStream : public std::istream {
 public:
  MemoryStream(const uint8_t *p, size_t l)
      : std::istream(&_buffer), _buffer(p, l) {
    rdbuf(&_buffer);
  }

  int Size();

 private:
  MemoryBuffer _buffer;
};
}  // namespace three_kingdoms
