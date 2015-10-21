#include "data.h"

#define A  2
#define B  3
#define C  4
#define D  5
#define E  6
#define F  7
#define G  8
#define H  9

#define FIRST 2
#define LAST 9
#define LOOP 10

#define STATUS_1 10
#define STATUS_2 13

Data *data;
Program *program;
char mask;

void setup() {
    // Initialize the pins as outputs
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(H, OUTPUT);

    // We also have two status LEDs, pin 10 on the Escudo,
    // and pin 13 on the Arduino itself
    pinMode(STATUS_1, OUTPUT);
    pinMode(STATUS_2, OUTPUT);

    // Utilize a few more pins for input
    pinMode(1, INPUT);
    pinMode(20, INPUT);
    pinMode(19, INPUT);

    data = load_data();
    program = &data->programs[1];
}

void loop() {
    load_mask();
    write();
    advance();
    delay(100);
}

void load_mask() {
    mask = 0;
    int i;
    for (i = 0; i < program->count; i++) {
        Channel *chan = program->channels[i];
        Bar *bar = chan->bars[chan->current];
        mask |= bar->notes[bar->current].bitmask;
        bar->current += 1;
    }
}

void advance() {
    int i;
    for (i = 0; i < program->count; i++) {
        Channel *chan = program->channels[i];
        Bar *bar = chan->bars[chan->current];
        if (bar->current == bar->count) {
            bar->current = 0;
            chan->current += 1;
            if (chan->current == chan->count) {
                chan->current = 0;
            }
        }
    }
}

void write() {
    int i;
    for (i = FIRST; i < LOOP; i++) {
        if (mask & 1<<(i - FIRST)) {
            digitalWrite(i, HIGH);
        } else {
            digitalWrite(i, LOW);
        }

    }
}

