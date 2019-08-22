#include <EnableInterrupt.h>
#include <Wire.h> 
#include "Constants.h"
#include <TimerOne.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>  

//servos incharge of positioning the  camera
Servo cameraBaseServo;
Servo cameraArmServo;

// OLED Display
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//struct to store robots state
RobotState robotState;


char buf[100];
int rot=1;
volatile bool ultrasonic_sensor_finished=true;
volatile unsigned long ultrasonic_sensor_start, ultrasonic_sensor_end;


void loop() {
   
  if(digitalRead(KILL_PIN) == LOW){
    robotState.motorSpeedLF = 0;       
    robotState.motorSpeedLR = 0;         
    robotState.motorSpeedRF = 0;         
    robotState.motorSpeedRR = 0; 
    killMotors();
    }
  updateStateFromSensors(); 
  handleHardwareSerialQueries(Serial);
  handleHardwareSerialQueries(Serial3);
  
  Serial.println(robotState.distanceUltrasonic);
  
  avoidObstacles();
  
  Serial.println(robotState.distanceUltrasonic);
  
  // if robot is heading for a target distance then kill motors if has accomplished it
  if(robotState.targetDistance!=0)
      killIfTargetDistanceAcheived(); 

   //display content on the OLED
   displayContentOnScreen();
}

void avoidObstacles(){
    if(ultrasonic_sensor_finished && robotState.distanceUltrasonic<25){
        playDangerSound();
        killMotors();
        spinMotorsToRotateClockwise(200,200);
        robotState.targetDistance=0;
        startUltrasonicSensorReading();
      }
  }
