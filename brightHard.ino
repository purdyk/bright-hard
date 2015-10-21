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
}

/* Stub this for now */
void loop() {
  command();
}


void command() {
  for (int i = A; i < E; i++) {
    digitalWrite(i, HIGH);
    delay(200);
    digitalWrite(i, LOW);
  }
}

