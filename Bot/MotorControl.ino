void spinMotorRightFront(int motorSpeed)                       //function for driving the right motor
{
  Serial.print(RAIN1);
  Serial.print(",");
  Serial.print(RAIN2);
  Serial.print(",");
  Serial.println(RPWMA);
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

/********************************************************************************/
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

/********************************************************************************/
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
  }
int isAround360(int degrees){
    if((degrees<5 || degrees>355) )
      return 1;
    else
      return 0;
  }


int getQuadrant(int degrees){
      return degrees/90;  
  
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
    serialprintf("init: %f current: %f targe: %f", initialHeading, robotState.mpu9265Reading.headingFiltered, targetPosition);
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
    serialprintf("init: %f current: %f targe: %f", initialHeading, robotState.mpu9265Reading.headingFiltered, targetPosition);
    headingQuadrant = getQuadrant(robotState.mpu9265Reading.headingDegrees);
  }

    killMotors();
  
  }
  
 /* 
void spinMotorsToRotateClockwise(int degrees){
    // give some time to the filter 
    for(int i=0; i<100;i++){
      readFromMPU9265();
    }
    
  float initialHeading = robotState.mpu9265Reading.headingFiltered;
  float targetPosition = initialHeading + degrees+20;
  if(targetPosition>360)
    targetPosition=360;
    
  float prevHeading = initialHeading;
  
  int wasHeadingAround360=0;
  int headingAround360 = isAround360(robotState.mpu9265Reading.headingFiltered);
  
  while(
          ((robotState.mpu9265Reading.headingFiltered <5 || robotState.mpu9265Reading.headingFiltered >355) || 
          robotState.mpu9265Reading.headingFiltered < targetPosition) &&
          !(targetPosition==360 && robotState.mpu9265Reading.headingFiltered>5  && robotState.mpu9265Reading.headingFiltered<90)
  
  ){
    wasHeadingAround360 = headingAround360;
    readFromMPU9265();
    spinMotors(175,175,-175,-175);
    headingAround360 = isAround360(robotState.mpu9265Reading.headingFiltered); 
    serialprintf("init: %f current: %f targe: %f", initialHeading, robotState.mpu9265Reading.headingFiltered, targetPosition);
    
  }

    killMotors();
  
  }

  void rotateRobotClockWise(int degrees){
    
        // give some time to the filter 
        for(int i=0; i<100;i++){
          readFromMPU9265();
        }
    float targetHeading=robotState.mpu9265Reading.headingFiltered+degrees;
    serialprintf("\n\n\ntargetHeading: %d\n\n",targetHeading);
    if(targetHeading>360){
        Serial.println("\n\n\ntriggered1 \n\n\n");
            
        // give some time to the filter 
        for(int i=0; i<10;i++){
          readFromMPU9265();
        }
        spinMotorsToRotateClockwise(360-robotState.mpu9265Reading.headingFiltered);
        Serial.println("\n\n\ntriggered \n\n\n");
            
        // give some time to the filter 
        for(int i=0; i<50;i++){
          readFromMPU9265();
        }
        spinMotorsToRotateClockwise(targetHeading-400);
      }
    else{
        Serial.println("\n\n\ntriggered2 \n\n\n");
        spinMotorsToRotateClockwise(degrees);
      }
    
    }

  */
