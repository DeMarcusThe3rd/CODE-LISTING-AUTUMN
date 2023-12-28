#include <Arduino.h>
#include "car.h"
#include <Adafruit_Sensor.h>
#include <math.h>
#include <LiquidCrystal.h>
#include <Adafruit_MPU6050.h>

/*
 *   Hex to Binary Cheat Sheet
 *  ==========================================
 *   A = 1010 -> both motors forward
 *   9 = 1001 -> right forward, left backward
 *   6 = 0110 -> left forward, right backward
 *   5 = 0101 -> both motors backward
 */

/* accepts input as a user defined literal hex operator and converts to 4 bit binary, each bit for each of the 4 motor driver pins  */
void send_to_driver(unsigned char bit, int PWML, int PWMR){
    digitalWrite(IN1, ((bit >> 3) & 0x1)); //right forward
    digitalWrite(IN2, ((bit >> 2) & 0x1)); //right backward
    digitalWrite(IN3, ((bit >> 1) & 0x1)); //left forward
    digitalWrite(IN4, ((bit >> 0) & 0x1)); //left backward

    analogWrite(ENA, PWMR); //right pwm
    analogWrite(ENB, PWML); //left pwm
}

/* forward for time t in milliseconds */
void forward(int t, int PWM){
    send_to_driver(0xA, PWM, PWM);
    delay(t);
}

/* backward for time t in milliseconds */
void backward(int t, int PWM){
    send_to_driver(0x5, PWM, PWM);
    delay(t);
}

/* stop for time t in milliseconds */
void stop(int t, int PWM){
    send_to_driver(0x0, PWM, PWM);
    delay(t);
}

/* turns the vehicle for time t in milliseconds */
void turn(int t, int PWML, int PWMR){
    if(PWML < 0){   //left motor backward, right motor forward
        send_to_driver(0x9, -PWML, PWMR);
        delay(t);
    }
    else if(PWMR < 0){  //right motor backward, left motor forward
        send_to_driver(0x6, PWML, -PWMR);
        delay(t);
    }
    else if((PWML < 0) && (PWMR < 0)){  //both motor backward
        send_to_driver(0x5, -PWML, -PWMR);
        delay(t);
    }
    else{   //both motor forward
        send_to_driver(0xA, PWML, PWMR);
        delay(t);
    }
}

void followLine(int IRL, int IRR, int LT, int RT, int t, int v){
    if((IRL < LT) && (IRR > RT)){
        turn(t, -255, 255);
    }
    else if((IRL > LT) && (IRR < RT)){
        turn(t, 255, -255);
    }
    else if((IRL < LT) && (IRR < RT)){
        stop(t, 0);
    }
    else{
        forward(t, v);
    }
}

void followLine(int IRL, int IRR, int LT, int RT, int t, int PWM){
    if((IRL < LT) && (IRR > RT)){
        turn(t, -255, 255);
    }
    else if((IRL > LT) && (IRR < RT)){
        turn(t, 255, -255);
    }
    else if((IRL < LT) && (IRR < RT)){
        stop(t, 0);
    }
    else{
        forward(t, PWM);
    }
}

void countDistance(){
    int pulse = digitalRead(ROT);

    if(pulse != initPulse){
        counter++;
    }
    initPulse = pulse;

    distance = counter*0.05*PI*radius;
    lcd.clear();
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print("m");
}

void countTime(){
    currenttime = millis();  //records current time
    int time = (currenttime - starttime)/1000;   //time elapsed
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    lcd.print(time);
    lcd.print("s");
}

void stopFor(int t){
    for(int i=1;i<t;i++){ //stop for t seconds and display time
        lcd.setCursor(0,1);
        lcd.print("Stop: ");
        lcd.print(i);
        lcd.print("s");
        stop(1000, 0);
    }
}

void rampAngle(){   //calculates and displays ramp angle
    ax = a.acceleration.x;

    float currentAngle = asin(ax/9.81)*180/PI;  //coverts to degrees
    lcd.clear();
    lcd.print("Ramp Angle: ");
    lcd.setCursor(0,1);
    lcd.print(currentAngle);
    lcd.print("deg");

    delay(3000);  
}

void avoidObstacle(){   //detects obstacle
    if (hc.dist()>50.00){
        forward(0,150);
    }

    else{
        turn(0,255,-255);
    }
}