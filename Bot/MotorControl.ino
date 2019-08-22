// sets the Target distance for the robot and make it move until:
//    1. Aroid covers the required target distance
//    2. Aroid detects an obstacle in front of it
//    3. Kill switch is pressed
void setTargetDistance(float distance, int speed){
    robotState.targetDistance = distance;
    robotState.startDistance = max4(robotState.distLF,robotState.distLR,robotState.distRF,robotState.distRR);
    spinMotors(speed,speed,speed,speed);
    killIfTargetDistanceAcheived();
 }

 
void killIfTargetDistanceAcheived()
{     
      float currDist = max4(robotState.distLF,robotState.distLR,robotState.distRF,robotState.distRR);
      serialprintf(Serial,"Curr: %f Start: %f Target: %f", currDist, robotState.startDistance, robotState.targetDistance);
     if(currDist-robotState.startDistance>robotState.targetDistance){
        killMotors();
        robotState.targetDistance = 0;
      }
}


void spinMotorRightFront(int motorSpeed)                       
{
  if (motorSpeed > 0)                                 
  {
    digitalWrite(RAIN1, HIGH);
    digitalWrite(RAIN2, LOW);               
  }
  else if (motorSpeed < 0)         
  {
    digitalWrite(RAIN1, LOW);               
    digitalWrite(RAIN2, HIGH); 
  }
  else                                           
  {
    digitalWrite(RAIN1, LOW);                        
    digitalWrite(RAIN2, LOW);                          
  }
  analogWrite(RPWMA, abs(motorSpeed));                 
}

void spinMotorLeftFront(int motorSpeed)     
{
  if (motorSpeed > 0)                                
  {
    digitalWrite(LAIN1, HIGH);   
    digitalWrite(LAIN2, LOW);     
  }
  else if (motorSpeed < 0)      
  {
    digitalWrite(LAIN1, LOW); 
    digitalWrite(LAIN2, HIGH);
  }
  else                                
  {
    digitalWrite(LAIN1, LOW);        
    digitalWrite(LAIN2, LOW);             
  }
  analogWrite(LPWMA, abs(motorSpeed));   
}

void spinMotorLeftRear(int motorSpeed)               
{
  if (motorSpeed > 0)                                
  {
    digitalWrite(LBIN1, HIGH);               
    digitalWrite(LBIN2, LOW);                 
  }
  else if (motorSpeed < 0)            
  {
    digitalWrite(LBIN1, LOW);          
    digitalWrite(LBIN2, HIGH);            
  }
  else                         
  {
    digitalWrite(LBIN1, LOW);            
    digitalWrite(LBIN1, LOW);                   
  }
  analogWrite(LPWMB, abs(motorSpeed));     
}

void spinMotorRightRear(int motorSpeed)                       
{
  if (motorSpeed > 0)
  {
    digitalWrite(RBIN1, HIGH);
    digitalWrite(RBIN2, LOW);                    
  }
  else if (motorSpeed < 0)   
  {
    digitalWrite(RBIN1, LOW);             
    digitalWrite(RBIN2, HIGH);       
  }
  else                
  {
    digitalWrite(RAIN1, LOW);  
    digitalWrite(RAIN2, LOW);             
  }
  analogWrite(RPWMB, abs(motorSpeed));     
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
  

void spinMotorsToRotateClockwise(int degrees, int speed){
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
    spinMotors(speed,speed,-speed,-speed); 
    serialprintf(Serial,"init: %f current: %f targe: %f", initialHeading, robotState.mpu9265Reading.headingFiltered, targetPosition);
    headingQuadrant = getQuadrant(robotState.mpu9265Reading.headingDegrees);
  }

    killMotors();
  
  }

void spinMotorsToRotateAntiClockwise(int degrees, int speed){
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
          (robotState.mpu9265Reading.headingFiltered > targetPosition) || (headingQuadrant != targetQuadrant)
  
  ){
    readFromMPU9265();
    spinMotors(-speed,-speed,speed,speed); 
    serialprintf(Serial, "init: %f current: %f targe: %f", initialHeading, robotState.mpu9265Reading.headingFiltered, targetPosition);
    headingQuadrant = getQuadrant(robotState.mpu9265Reading.headingDegrees);
  }

    killMotors();
  
  }
  
