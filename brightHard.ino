#include "data.h"

#define EL_A  2
#define EL_B  3
#define EL_C  4
#define EL_D  5
#define EL_E  6
#define EL_F  7
#define EL_G  8
#define EL_H  9


/*
 * These pins are set up for headers on the side of the board
 * Ideal for input, but could be used for output if ...
 * who knows?
 */

/* Analog or Digital pins */
#define EL_A2 21
#define EL_A3 21
#define EL_A4 21
#define EL_A5 21

/* Analog only pins */
#define EL_A6 21
#define EL_A7 21

/*
 *  Makes looping through the pins a little
 *  nicer to read
 */
#define FIRST 2
#define LAST 9
#define LOOP 10

#define STATUS_1 10 // ???
#define STATUS_2 13 // Board

Data *data;
Program *program;
char mask;
unsigned short status_counter;

void setup() {
    // Initialize the pins as outputs
    pinMode(EL_A, OUTPUT);
    pinMode(EL_B, OUTPUT);
    pinMode(EL_C, OUTPUT);
    pinMode(EL_D, OUTPUT);
    pinMode(EL_E, OUTPUT);
    pinMode(EL_F, OUTPUT);
    pinMode(EL_G, OUTPUT);
    pinMode(EL_H, OUTPUT);

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
    status_counter = 0;

    // Using a Timer at (1 / (bpm / 60)) / 8
    // This gives us quarte-note beats which
    // I believe is pretty standard

    // set timer interrupt at 64Hz
    TCCR0A = 0;// set entire TCCR1A register to 0
    TCCR0B = 0;// same for TCCR1B
    TCNT0 = 0;//initialize counter value to 0

    // set compare match register for increments (must be <65536)
    // (16 * (10**6)) / (((bpm / 60.) * 8) * 1024) - 1
    // 1171 // 100 bpm
    OCR0A = 976; // 120 bpm
    // OCR0A = 837; // 140 bpm

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

    char mod = status_counter % 16;
    if (mod == 0) {
        digitalWrite(STATUS_2, HIGH);
    }

    if (mod == 4) {
        digitalWrite(STATUS_2, LOW);
    }

    status_counter++;
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

