#ifndef GOOGLE_PROTOBUF_UTF8_RANGE_H__
#define GOOGLE_PROTOBUF_UTF8_RANGE_H__

#include <cstdint>
#include <cstring>

namespace google {
namespace protobuf {

class Utf8Range {
 public:
  Utf8Range(const char* begin, const char* end)
    : begin_(begin), end_(end) {}

  const char* begin() const { return begin_; }
  const char* end() const { return end_; }

  uint32_t Decode(const char*& ptr) const {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(ptr);
    uint32_t c = *p++;
    int extra_bytes = 0;
    if (c >= 0x80) {
      c = (c & 0x7F) << 6;
      uint32_t d = *p++;
      if ((d & 0xC0) != 0x80) return 0;
      c |= (d & 0x3F);
      extra_bytes = 1;
    }
    if (c >= 0x800) {
      c = (c & 0x7FF) << 6;
      uint32_t d = *p++;
      if ((d & 0xC0) != 0x80) return 0;
      c |= (d & 0x3F);
      extra_bytes = 2;
    }
    if (c >= 0x10000) {
      c = (c & 0xFFFF) << 6;
      uint32_t d = *p++;
      if ((d & 0xC0) != 0x80) return 0;
      c |= (d & 0x3F);
      extra_bytes = 3;
    }
    ptr += extra_bytes + 1;
    return c;
  }

 private:
  const char* begin_;
  const char* end_;
};

}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_UTF8_RANGE_H__
