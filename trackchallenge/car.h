#include <Arduino.h>
#include "car.h"

/*pin allocation*/
#define IN1 13
#define IN2 12
#define IN3 A4
#define IN4 A5
#define ENA 11
#define ENB 3

#define RIR A1  //right IR sensor input
#define LIR A2  //left IR sensor input
#define ROT A3  //rotary encoder input

/*function definitions*/
void send_to_driver(unsigned char bit, int PWML, int PWMR);
void forward(int t, int PWM);
void backward(int t, int PWM);
void stop(int t, int PWM);
void turn(int t, int PWML, int PWMR);
void rampAngle();
void countDistance();
void countTime();
void stopFor(int t);
