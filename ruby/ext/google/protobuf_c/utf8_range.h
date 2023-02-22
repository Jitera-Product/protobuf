#ifndef UTF8_RANGE_H
#define UTF8_RANGE_H


namespace utf8 {

  // Define some constants
  const uint32_t kMask[] = {0x7F, 0x1F, 0x0F, 0x07};
  const uint32_t kMagic[] = {0x00, 0xC0, 0xE0, 0xF0};

  // Define the utf8_range class
  class utf8_range {
  public:
    utf8_range(const char* str) : str_(str) {}
    ~utf8_range() {}

    const char* begin() const { return str_; }
    const char* end() const { return str_ + std::strlen(str_); }

    uint32_t decode(const char*& ptr) const {
      const unsigned char* p = reinterpret_cast<const unsigned char*>(ptr);
      uint32_t c = *p++;
      int extra_bytes = 0;
      if (c >= 0x80) {
        c = (c & kMask[1]) << 6;
        uint32_t d = *p++;
        if ((d & 0xC0) != 0x80) return 0;
        c |= (d & 0x3F);
        extra_bytes = 1;
      }
      if (c >= 0x800) {
        c = (c & kMask[2]) << 6;
        uint32_t d = *p++;
        if ((d & 0xC0) != 0x80) return 0;
        c |= (d & 0x3F);
        extra_bytes = 2;
      }
      if (c >= 0x10000) {
        c = (c & kMask[3]) << 6;
        uint32_t d = *p++;
        if ((d & 0xC0) != 0x80) return 0;
        c |= (d & 0x3F);
        extra_bytes = 3;
      }
      ptr += extra_bytes + 1;
      return c;
    }

  private:
    const char* str_;
  };

}  // namespace utf8

#endif  // UTF8_RANGE_H
