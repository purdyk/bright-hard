#include <stdint.h>

typedef char byte;

typedef struct {
  byte duration;
  byte intensity_start;
  byte intensity_finish;
  byte bitmask;
} Note;

/* pointer to program memory */

typedef struct {
  uint16_t length;
  uint16_t current;
  Note *notes[0];
} Bar;

/* 
 * allocated
 * helper loadBar maps note offset to first note 
 */

typedef struct {
  uint16_t length;
  uint16_t current;
  Bar *bars[0];
} Channel;

/*
 * allocated
 * helper loadChannel maps all bars to actual pointers
 */

typedef struct {
  byte channelMask;
  byte mode;
  Channel *channels[8];
} Composition;

/*
 * healper loadComposition maps all channels to actual channels
 */

typedef struct {
  uint16_t programCount;
  Composition programs[0];
} Data;

/* loader expects raw.h to include a program */
Data *load_data();

