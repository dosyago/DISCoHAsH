#pragma once

template <bool bswap>
static inline uint64_t GET_U64(const uint8_t* data, size_t index) {
  uint64_t result;
  std::memcpy(&result, data + index, sizeof(result));
  if(bswap) {
    result = ((result & 0x00000000000000FF) << 56) |
             ((result & 0x000000000000FF00) << 40) |
             ((result & 0x0000000000FF0000) << 24) |
             ((result & 0x00000000FF000000) << 8) |
             ((result & 0x000000FF00000000) >> 8) |
             ((result & 0x0000FF0000000000) >> 24) |
             ((result & 0x00FF000000000000) >> 40) |
             ((result & 0xFF00000000000000) >> 56);
  }
  return result;
}

template <bool bswap>
static inline void PUT_U64(uint64_t value, uint8_t* data, size_t index) {
  if(bswap) {
    value = ((value & 0x00000000000000FF) << 56) |
            ((value & 0x000000000000FF00) << 40) |
            ((value & 0x0000000000FF0000) << 24) |
            ((value & 0x00000000FF000000) << 8) |
            ((value & 0x000000FF00000000) >> 8) |
            ((value & 0x0000FF0000000000) >> 24) |
            ((value & 0x00FF000000000000) >> 40) |
            ((value & 0xFF00000000000000) >> 56);
  }
  std::memcpy(data + index, &value, sizeof(value));
}

