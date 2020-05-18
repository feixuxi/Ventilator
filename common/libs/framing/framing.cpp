#include "framing.h"

#include <stdint.h>

uint32_t UnescapeFrame(uint8_t *source, uint32_t sourceLength, uint8_t *dest,
                       uint32_t destLength) {
  uint32_t i = 0;
  bool isEsc = false;
  for (uint32_t j = 0; j < sourceLength; j++) {
    switch (source[j]) {
    case FRAMING_MARK:
      break;
    case FRAMING_ESC:
      isEsc = true;
      break;
    default:
      if (i >= destLength) {
        return 0;
      }
      if (isEsc) {
        isEsc = false;
        dest[i++] = source[j] ^ 0x20;
      } else {
        dest[i++] = source[j];
      }
      break;
    }
  }
  return i;
}
