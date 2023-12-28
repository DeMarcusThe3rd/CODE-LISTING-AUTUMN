#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int initPulse = digitalRead(ROT);
int counter = 0;
float radius = 0.0325;
float distance = 0;
    
unsigned long starttime;
unsigned long currenttime;
int movetime = 0;


Adafruit_MPU6050 mpu;
float ax;
int rampswitch = 0;
float theta = 0;
float rotstarttime = 0;
float rotcurrenttime = 0;
float rottime = 0;


void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(LIR, INPUT);
  pinMode(RIR, INPUT);
  pinMode(ROT, INPUT);

/*initialize mpu*/
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setInterruptPinLatch(true);	
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);   // set accelerometer range to +-8G
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);	// set gyro range to +- 500 deg/s
}

void loop() {
  /* Motion Initialisation */
  int IRL = analogRead(LIR);
  int IRR = analogRead(RIR);

  int LT = 500; //left IR threshold
  int RT = 500; //right IR threshold
  int t = 0;  //period between each reading  
  
  
  if(rampswitch == 0){  //1st part of the track
    forward(1500, 150);
    stop(0,0);
    rampswitch = 1;  //goes on the ramp
  }
  else if(rampswitch == 1){  //2nd part of the track
    delay(2500);  //stops for 2.5 seconds to obtain an accurate reading

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    rampAngle(3);  //calculate and display ramp angle
    rampswitch = 2;  //continues up the ramp
  }
  else if(rampswitch == 2){  //3rd part of the ramp
    turn(1000, 170, 150); // going up the ramp

    stopFor(5);   //stop for 5 seconds
    turn(1000, -255, 255);  //spin 360 degrees at full speed
    stop(1000, 0);  
  
    rampswitch = 3;  //finish spinning
  }
  else if(rampswitch == 3){// going down slope
    followLine(IRL, IRR, LT, RT, t, 80);

    if(movetime <= 1000){   //goes down slope for 1 second
      movetime++;
      delay(1);
    }
    else{
      movetime = 0;  //exits the slope sequence
      forward(500, 100); //jumpstarts the motor
      rampswitch = 4;
    }
  }
  else if(rampswitch == 4){
    followLine(IRL, IRR, LT, RT, t, 60);
    
    if(distance <= 0.8){ //move for 80cm
      countDistance();
    else{
      rampswitch = 5;  //finish 80cm
    }
    }
  }
  else if(rampswitch == 5){  
    stopFor(4);  //stops for 4 seconds
    forward(100, 200);

    starttime = millis();  //starts counting start time
    rampswitch = 6;
  
    countDistance(); 
    countTime();
    followLine(IRL, IRR, LT, RT, t, 60);

    }
}
