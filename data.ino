#include "data.h"
#include "raw.h"
#include "stdlib.h"

/*
 * The following are program-memory formats for the major types
 * as they will exist from the compiler;
 * they must be parsed and loaded into their counterparts
 */

typedef struct {
    uint16_t count;
    uint16_t noteOffset;
} BarS;

Data *load_data() {
    Data *data = (Data *) malloc(sizeof(Data));
    int noteLen = sizeof(NOTES) / 4;

    int i, j = 0;

    Note *notes = (Note *) &NOTES;
    BarS *raw_bars = (BarS *) &BARS;

    Bar *bars = (Bar *) malloc(sizeof(Bar) * BAR_COUNT);

    Channel *channels = (Channel *) malloc(sizeof(Channel) * CHANNEL_COUNT);
    Composition *programs = (Composition *) malloc(sizeof(Composition) * PROGRAM_COUNT);

    for (i = 0; i < BAR_COUNT; i++) {
        bars[i].count = raw_bars[i].count;
        bars[i].notes = &notes[raw_bars[i].noteOffset];
        bars[i].current = 0;
    }

    int counter = 0;
    for (i = 0; i < CHANNEL_COUNT; i++) {
        char cnt = CHANNELS[counter++];
        //counter += sizeof(uint16_t);
        channels[i].count = cnt;
        channels[i].current = 0;
        channels[i].bars = (Bar **) malloc(sizeof(Bar *) * cnt);
        for (j = 0; j < cnt; j++) {
            channels[i].bars[j] = &bars[CHANNELS[counter++]];
            //counter += sizeof(uint16_t);
        }
    }

    counter = 0;
    for (i = 0; i < PROGRAM_COUNT; i++) {
        uint16_t cnt = PROGRAMS[counter++];
        //counter += sizeof(uint16_t);
        programs[i].count = cnt;
        programs[i].mode = 0;
        programs[i].channels = (Channel **) malloc(sizeof(Channel *) * cnt);
        for (j = 0; j < cnt; j++) {
            programs[i].channels[j] = &channels[PROGRAMS[counter++]];
            //counter += sizeof(uint16_t);
        }
    }

    data->programCount = PROGRAM_COUNT;
    data->programs = programs;

    return data;
}

void reset_program(Composition *prog) {
    int i,j;
    for (i = 0; i < prog->count; i++) {
        Channel *chan = prog->channels[i];
        chan->current = 0;
        for (j = 0; j < chan->count; j++) {
            Bar *bar = chan->bars[j];
            bar->current = 0;
        }
    }
}

