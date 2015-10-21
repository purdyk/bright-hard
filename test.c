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
    printf("Channel contains %d bars\n", channel->count);

    for (int i = 0; i < channel->count; i++) {
        printf("Bar %d:", i);
        print_bar(channel->bars[i]);
    }
}

void print_program(Composition *prog) {
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

int main(int argc, char **argv) {
    puts("Loading data from header.");
    Data *data = load_data();
    puts("data loaded.");
    print_data(data);
    reset_program(&data->programs[0]);
}

#endif