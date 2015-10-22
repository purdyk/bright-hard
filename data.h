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
    uint16_t count;
    Note *notes;
} Bar;

typedef struct {
    uint16_t barCount;
    uint16_t currentBar;
    uint16_t noteCount;
    uint16_t currentNote;
    uint16_t noteDuration;
    Bar **bars;
} Channel;

typedef struct {
    char count;
    char mode;
    char dirty;
    Channel **channels;
} Program;

typedef struct {
    uint16_t programCount;
    Program *programs;
} Data;

/* loader expects raw.h to include a program */
Data *load_data();

void advance_program(Program *prog);
char advance_channel(Channel *channel);

void reset_program(Program *prog);
void reset_channel(Channel *channel);

#endif

