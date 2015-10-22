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
//    pinMode(1, INPUT);
//    pinMode(20, INPUT);
//    pinMode(19, INPUT);

    // Load and prepare data
    data = load_data();
    program = &data->programs[1];
    reset_program(program);

    // Using a Timer at (1 / (bpm / 60)) / 32
    // set timer interrupt at 64Hz
    TCCR0A = 0;// set entire TCCR1A register to 0
    TCCR0B = 0;// same for TCCR1B
    TCNT0 = 0;//initialize counter value to 0

    // set compare match register for 64hz increments (120 bpm) (must be <65536)
    // (16 * (10**6)) / (((bpm / 60.) * 32) * 1024) - 1
    //  OCR0A = 243; // 120 bpm
    OCR0A = 208; // 140 bpm

    // turn on CTC mode
    TCCR0B |= (1 << WGM02);

    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR0B |= (1 << CS02) | (1 << CS00);

    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);
}

ISR (TIMER0_COMPA_vect) { // Timer 0

    if (program->dirty) {
        load_mask();
        write_mask();
    }
    advance_program(program);
}

void loop() {
    /* :D */
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
        if (mask & 1 << (i - FIRST)) {
            digitalWrite(i, HIGH);
        } else {
            digitalWrite(i, LOW);
        }

    }
}

