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
    Program *programs = (Program *) malloc(sizeof(Program) * PROGRAM_COUNT);

    for (i = 0; i < BAR_COUNT; i++) {
        bars[i].count = raw_bars[i].count;
        bars[i].notes = &notes[raw_bars[i].noteOffset];
    }

    int counter = 0;
    for (i = 0; i < CHANNEL_COUNT; i++) {
        char cnt = CHANNELS[counter];
        counter += sizeof(uint16_t);
        channels[i].barCount = cnt;
        channels[i].bars = (Bar **) malloc(sizeof(Bar *) * cnt);
        for (j = 0; j < cnt; j++) {
            channels[i].bars[j] = &bars[CHANNELS[counter]];
            counter += sizeof(uint16_t);
        }
    }

    counter = 0;
    for (i = 0; i < PROGRAM_COUNT; i++) {
        uint16_t cnt = PROGRAMS[counter];
        counter += sizeof(uint16_t);
        programs[i].count = cnt;
        programs[i].mode = 0;
        programs[i].dirty = 1;
        programs[i].channels = (Channel **) malloc(sizeof(Channel *) * cnt);
        for (j = 0; j < cnt; j++) {
            programs[i].channels[j] = &channels[PROGRAMS[counter]];
            counter += sizeof(uint16_t);
        }
    }

    data->programCount = PROGRAM_COUNT;
    data->programs = programs;

    return data;
}

void advance_program(Program *prog) {
    int i;
    prog->dirty = 0;
    for (i = 0; i < prog->count; i++) {
        prog->dirty |= advance_channel(prog->channels[i]);
    }
}

char advance_channel(Channel *channel) {
    channel->noteDuration--;
    if (channel->noteDuration == 0) {
        channel->currentNote++;

        if (channel->currentNote == channel->noteCount) {
            channel->currentNote = 0;
            channel->currentBar++;

            if (channel->currentBar == channel->barCount) {
                channel->currentBar = 0;
            }

            channel->noteCount = channel->bars[channel->currentBar]->count;
        }
        channel->noteDuration = 1 << channel->bars[channel->currentBar]->notes[channel->currentNote].duration;

        return 1;
    }

    return 0;
}

void reset_channel(Channel *channel) {
    channel->currentBar = 0;
    channel->currentNote = 0;
    channel->noteCount = channel->bars[0]->count;
    channel->noteDuration = 1 << channel->bars[0]->notes[0].duration;
}

void reset_program(Program *prog) {
    int i;
    prog->dirty = 0;
    for (i = 0; i < prog->count; i++) {
        Channel *chan = prog->channels[i];
        reset_channel(chan);
    }
}

