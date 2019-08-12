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

void spinMotorsToRotateClockwise(int degrees){
  readFromMPU9265();
  float initialHeading = robotState.mpu9265Reading.headingDegrees;
  float targetPosition = initialHeading + degrees;
  float prevHeading = initialHeading;
  
  int quadrantAdjustment=0;
  
  while(robotState.mpu9265Reading.headingDegrees < targetPosition-quadrantAdjustment*360){
    serialprintf("init: %f current: %f", initialHeading, robotState.mpu9265Reading.headingDegrees);
    readFromMPU9265();
    spinMotors(175,175,-175,-175);
    if(prevHeading-90>robotState.mpu9265Reading.headingDegrees-90){
      quadrantAdjustment++;
    }
    else{
      if(quadrantAdjustment>0)
        quadrantAdjustment--;
      }
  }

  
    
    killMotors();
  
  }

  
