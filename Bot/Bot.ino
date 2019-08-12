
#include <Wire.h> 
#include "Constants.h"
#include <TimerOne.h>
#include <FaBo9Axis_MPU9250.h>

FaBo9Axis fabo_9axis;

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
    
    }
  
  if (Serial3.available() > 0){          //if the user has entered something in the serial monitor
   
    inputType = Serial3.readStringUntil(',');
    
    Serial.println("Serial avail");
    if(inputType=="W"){
      robotState.motorSpeedLF = Serial3.readStringUntil(',').toInt();       
      robotState.motorSpeedLR = Serial3.readStringUntil(',').toInt();         
      robotState.motorSpeedRF = Serial3.readStringUntil(',').toInt();         
      robotState.motorSpeedRR = Serial3.readStringUntil('\n').toInt(); 
    }
    else if(inputType=="C"){
      robotState.cameraBasePos = Serial3.readStringUntil(',').toInt();          
      robotState.cameraArmPos = Serial3.readStringUntil('\n').toInt(); 
      serialprintf("Servo Moved --> Head: %d Arm %d", robotState.cameraBasePos, robotState.cameraArmPos );
      positionRobotHead(robotState.cameraBasePos, robotState.cameraArmPos);
    }
    
    serialprintf("Motor Speed --> LF: %d LR: %d RF: %d RR: %d", robotState.motorSpeedLF, robotState.motorSpeedLR, robotState.motorSpeedRF, robotState.motorSpeedRR);
  }
     spinMotors(robotState.motorSpeedLF, robotState.motorSpeedLR, robotState.motorSpeedRF, robotState.motorSpeedRR);
     //serialprintf("Degrees C: %f  Light Levels: %d  Sonar Distance: %f cm", getTempratureReading(TEMP_UNITS_CELCIUS), getLightLevels(),  getDistance());  
     readFromMPU9265();
     printMagnetometerReadings();
     
  //spinMotorsToRotate90DegreeClockwise()
  if(rot==1){
    spinMotorsToRotateClockwise(90);
    rot =0;
    }
  //delay(1000);
}
