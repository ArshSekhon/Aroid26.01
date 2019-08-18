
#include <Wire.h> 
#include "Constants.h"
#include <TimerOne.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h> 
 
Servo cameraBaseServo;
Servo cameraArmServo;

// OLED Display
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RobotState robotState;

String inputType;

char buf[100];
int rot=1;

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
  
  //Serial.println(getDistance());
  
  //if obstacle avoidance is active
  if(digitalRead(SWITCH_PIN) == LOW)
      avoidObstacles();
  
  // if robot is heading for a target distance then kill motors if has accomplished it
  if(robotState.targetDistance!=0)
      killIfTargetDistanceAcheived(); 
   printAllMPU9265ReadingsToSerial(Serial);   
   displayInfoScreen();
}

void avoidObstacles(){

    if(getDistance()<15){
        playDangerSound();
        killMotors();
        spinMotorsToRotateClockwise(180,200);
      }
  }
