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
    uint16_t current;
    Note *notes;
} Bar;

/* 
 * allocated
 * helper loadBar maps note offset to first note 
 */

typedef struct {
    uint16_t count;
    uint16_t current;
    Bar **bars;
} Channel;

/*
 * allocated
 * helper loadChannel maps all bars to actual pointers
 */

typedef struct {
    char count;
    char mode;
    Channel **channels;
} Program;

/*
 * healper loadComposition maps all channels to actual channels
 */

typedef struct {
    uint16_t programCount;
    Program *programs;
} Data;

/* loader expects raw.h to include a program */
Data *load_data();

void reset_program(Program *prog);

#endif

