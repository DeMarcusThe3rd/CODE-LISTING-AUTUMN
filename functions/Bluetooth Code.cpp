#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  char bt = Serial.read();

  if(bt == 'F'){
    forward(0, 150);
  }
  else if(bt == 'G'){
    backward(0, 150);
  }
  else if(bt == 'L'){
    turn(0, -150, 150);
  }
  else if(bt == 'R'){
    turn(0, 150, -150);
  }
  else if(bt == 'S'){
    stop(0, 0);
  }
}
