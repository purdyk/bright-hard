# bright-hard
Driver and compiler for SparkFun's EL Sequencer module.

Programs are written in JSON and compiled with compiler.py
This will generate a raw.h file which will be compiled into the code for the arduino.

Sample .bthd file included as 'basic.bthd'.

Before running code on the EL Sequencer, do './compile.py \<file.bthd>' to generate the headers.
