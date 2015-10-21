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

#define MILLIS_PER_LOOP 16 // around 120 bpm.  good for testing for now.

Data *data;
Program *program;
char mask;
unsigned long loopStart;

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
    reset_program(program);
}

void loop() {

    loopStart = millis();

    if (program->dirty) {
        load_mask();
        write_mask();
    }
    advance_program(program);

    /* Hacky inaccurate delay code */
    delay(MILLIS_PER_LOOP - (millis() - loopStart));
}

void load_mask() {
    mask = 0;
    int i;
    for (i = 0; i < program->count; i++) {
        Channel *chan = program->channels[i];
        mask |= chan->bars[chan->currentBar]->notes[chan->currentNote].bitmask;
    }
}

void write_mask() {
    int i;
    for (i = FIRST; i < LOOP; i++) {
        if (mask & 1<<(i - FIRST)) {
            digitalWrite(i, HIGH);
        } else {
            digitalWrite(i, LOW);
        }

    }
}

