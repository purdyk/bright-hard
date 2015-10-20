#include "data.h"

void setup() {
  // Initialize the pins as outputs
  pinMode(2, OUTPUT);  // channel A  
  pinMode(3, OUTPUT);  // channel B   
  pinMode(4, OUTPUT);  // channel C
  pinMode(5, OUTPUT);  // channel D    
  pinMode(6, OUTPUT);  // channel E
  pinMode(7, OUTPUT);  // channel F
  pinMode(8, OUTPUT);  // channel G
  pinMode(9, OUTPUT);  // channel H
  // We also have two status LEDs, pin 10 on the Escudo, 
  // and pin 13 on the Arduino itself
  pinMode(10, OUTPUT);     
  pinMode(13, OUTPUT); 
  
  // Utilize a few more pins for input
  pinMode(1, INPUT);
  pinMode(20, INPUT);
  pinMode(19, INPUT);
}

void loop() {
  command();
  step();
}

void command() {

}

void step() {
  
}

void speedUp() {

}

void speedDown() {

}

void compile() {

}
