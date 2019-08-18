void setTargetDistance(float distance, int speed){
    robotState.targetDistance = distance;
    robotState.startDistance = max4(robotState.distLF,robotState.distLR,robotState.distRF,robotState.distRR);
    spinMotors(speed,speed,speed,speed);
 }

void killIfTargetDistanceAcheived()
{
     if(max4(robotState.distLF,robotState.distLR,robotState.distRF,robotState.distRR)-robotState.startDistance>robotState.targetDistance){
        killMotors();
        robotState.targetDistance = 0;
      }
}


void spinMotorRightFront(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(RAIN1, HIGH);                         //set pin 1 to high
    digitalWrite(RAIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(RAIN1, LOW);                          //set pin 1 to low
    digitalWrite(RAIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(RAIN1, LOW);                          //set pin 1 to low
    digitalWrite(RAIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(RPWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

void spinMotorLeftFront(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(LAIN1, HIGH);                         //set pin 1 to high
    digitalWrite(LAIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(LAIN1, LOW);                          //set pin 1 to low
    digitalWrite(LAIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(LAIN1, LOW);                          //set pin 1 to low
    digitalWrite(LAIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(LPWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

void spinMotorLeftRear(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(LBIN1, HIGH);                         //set pin 1 to high
    digitalWrite(LBIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(LBIN1, LOW);                          //set pin 1 to low
    digitalWrite(LBIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(LBIN1, LOW);                          //set pin 1 to low
    digitalWrite(LBIN1, LOW);                          //set pin 2 to low
  }
  analogWrite(LPWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

void spinMotorRightRear(int motorSpeed)                       
{
  if (motorSpeed > 0)
  {
    digitalWrite(RBIN1, HIGH);
    digitalWrite(RBIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(RBIN1, LOW);                          //set pin 1 to low
    digitalWrite(RBIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(RAIN1, LOW);                          //set pin 1 to low
    digitalWrite(RAIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(RPWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

void positionRobotHead(int baseServoPosition, int armServoPosition){
      cameraBaseServo.write(baseServoPosition);
      cameraArmServo.write(armServoPosition);
}
void spinMotors(int lfMotorSpeed, int lrMotorSpeed, int rfMotorSpeed, int rrMotorSpeed){
      spinMotorLeftFront(lfMotorSpeed);
      spinMotorLeftRear(lrMotorSpeed);
      spinMotorRightFront(rfMotorSpeed);
      spinMotorRightRear(rrMotorSpeed);
  }
void killMotors(){
  
    spinMotors(0,0,0,0);
    robotState.motorSpeedLF = 0;       
    robotState.motorSpeedLR = 0;         
    robotState.motorSpeedRF = 0;         
    robotState.motorSpeedRR = 0; 
  }
  

void spinMotorsToRotateClockwise(int degrees){
    // give some time to the filter 
    for(int i=0; i<100;i++){
      readFromMPU9265();
    }
    
  float initialHeading = robotState.mpu9265Reading.headingFiltered;
  float targetPosition = ((int)initialHeading + degrees)%360;
  int headingQuadrant = getQuadrant(robotState.mpu9265Reading.headingDegrees);
  int targetQuadrant = getQuadrant(targetPosition);
   
  
  while(
          (robotState.mpu9265Reading.headingDegrees < targetPosition) || (headingQuadrant != targetQuadrant)
  
  ){
    readFromMPU9265();
    spinMotors(175,175,-175,-175); 
    serialprintf(Serial,"init: %f current: %f targe: %f", initialHeading, robotState.mpu9265Reading.headingFiltered, targetPosition);
    headingQuadrant = getQuadrant(robotState.mpu9265Reading.headingDegrees);
  }

    killMotors();
  
  }

void spinMotorsToRotateAntiClockwise(int degrees){
    // give some time to the filter 
    for(int i=0; i<100;i++){
      readFromMPU9265();
    }
    
  float initialHeading = robotState.mpu9265Reading.headingFiltered;
  float targetPosition = ((int)initialHeading - degrees+20);
  if(targetPosition<0)
    targetPosition=360+targetPosition;
  int headingQuadrant = getQuadrant(robotState.mpu9265Reading.headingDegrees);
  int targetQuadrant = getQuadrant(targetPosition);
   
  
  while(
          (robotState.mpu9265Reading.headingDegrees > targetPosition) || (headingQuadrant != targetQuadrant)
  
  ){
    readFromMPU9265();
    spinMotors(-175,-175,175,175); 
    serialprintf(Serial, "init: %f current: %f targe: %f", initialHeading, robotState.mpu9265Reading.headingFiltered, targetPosition);
    headingQuadrant = getQuadrant(robotState.mpu9265Reading.headingDegrees);
  }

    killMotors();
  
  }
  
