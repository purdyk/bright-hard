#ifdef DEBUG

#include "data.h"
#include <stdio.h>

void print_bar(Bar *bar) {
    printf("bar size: %d\n", bar->count);

    for (int j = 0; j < bar->count; j++) {
        printf(" note: %x", (unsigned char) bar->notes[j].bitmask);
        printf("\n");
    }
}

void print_channel(Channel *channel) {
    printf("Channel contains %d bars\n", channel->barCount);

    for (int i = 0; i < channel->barCount; i++) {
        printf("Bar %d:", i);
        print_bar(channel->bars[i]);
    }
}

void print_program(Program *prog) {
    printf("Program contains %d channels\n", prog->count);
    for (int i = 0; i < prog->count; i++) {
        printf("Channel %d:\n", i);
        print_channel(prog->channels[i]);
    }
}

void print_data(Data *data) {
    printf("Project contains %d programs\n", data->programCount);
    for (int i = 0; i < data->programCount; i++) {
        printf("Program %d:\n", i);
        print_program(&data->programs[i]);
    }
}

void print_state(Program *program) {
    for (int i = 0; i < program->count; i++) {
        Channel *chan = program->channels[i];
        printf("C:%d tb:%d cb:%d tn:%d cn:%d nd:%d", i, chan->barCount, chan->currentBar, chan->noteCount,
         chan->currentNote, chan->noteDuration);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    puts("Loading data from header.");
    Data *data = load_data();
    puts("data loaded.");
    print_data(data);

    Program *prog = &data->programs[1];
    reset_program(prog);

    for (int i = 0; i < 1000; i++) {
        print_state(prog);
        advance_program(prog);
    }

}

#endif