#include "data.h"
#include "raw.h"

/*
 * The following are program-memory formats for the major types
 * as they will exist from the compiler;
 * they must be parsed and loaded into their counterparts
 */

typedef struct {
  byte channelMask;
  byte mode;
  uint16_t channelOffsets[8];
} CompositionS;

typedef struct {
  uint16_t length;
  uint16_t noteOffset;
} BarS;

typedef struct {
  uint16_t length;
  uint16_t bars[0];
} ChannelS;

typedef struct {
  uint16_t noteCount;
  uint16_t programCount;
  uint16_t programOffset;
  uint16_t barCount;
  uint16_t barOffset;
  uint16_t channelCount;
  uint16_t channelOffset;
  byte data[0];
} RawData;

Data *load_data() {
  
}

