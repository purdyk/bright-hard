#include <stdint.h>

#ifndef DATA

#define DATA

typedef struct {
  char duration;
  char intensity_start;
  char intensity_finish;
  char bitmask;
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
  char channelMask;
  char mode;
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

#endif

