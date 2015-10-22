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

/*
 * This flag is in the spec but not defined for some reason
 */

#define CTC1 3

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
    // This gives us quarter-note beats which
    // I believe is pretty standard
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // set compare match register for increments (must be <65536)
    // clock speed   /   ( desired interval )  * ( prescalar ) - one cycle zero indexed
    // ((((8 * (10**6)) / 64.) * ( 1 / (bpm / 60.))) / 8) - 1
    // (Clock Speed / Prescalar) / (( bpm / spm ) / ( thrity-seconds ) - 1 zero start
    //    OCR1A = 9374; // 100 bpm
    //    OCR1A = 8522; // 110 bpm
    OCR1A = 7812; // 120 bpm
//     OCR1A = 6695; // 140 bpm

    // turn on CTC mode
    TCCR1B |= _BV(CTC1);

    // Set CS11 and CS10 bits for 256 prescalar
    TCCR1B |= _BV(CS11) | _BV(CS10);

    // enable timer compare interrupt
    TIMSK1 |= _BV(OCIE1A);

}

ISR (TIMER1_COMPA_vect) { // Timer 1 16 bit

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

