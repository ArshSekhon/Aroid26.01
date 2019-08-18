#define DISTANCE_PER_ENCODER_SLOT_IN_CM 0.2501
unsigned int counterLF=0, 
             counterRF=0, 
             counterLR=0, 
             counterRR=0;
volatile int odotimerTicks=0;
             
void incCounterLF()
{ 
  counterLF++;
  
} 
void incCounterRF()
{
  counterRF++;
} 
void incCounterLR()
{
  counterLR++;
} 
void incCounterRR()
{
  counterRR++;
} 

void initOdometry(){
  Timer1.initialize(10000); // set timer for 1sec
  attachInterrupt(digitalPinToInterrupt(ODOMETER_LEFT_FRONT_PIN), incCounterLF, RISING); 
  attachInterrupt(digitalPinToInterrupt(ODOMETER_LEFT_REAR_PIN), incCounterLR, RISING); 
  attachInterrupt(digitalPinToInterrupt(ODOMETER_RIGHT_FRONT_PIN), incCounterRF, RISING); 
  attachInterrupt(digitalPinToInterrupt(ODOMETER_RIGHT_REAR_PIN), incCounterRR, RISING); 
  Timer1.attachInterrupt(timerOdometryAndMPU ); // enable the timer
  
}

void timerOdometryAndMPU()
{
  intFlag=true;
  if(++odotimerTicks>100){
      odotimerTicks=0;
      Timer1.detachInterrupt();  //stop the timer
      robotState.rpmLF = (counterLF * 3);  // divide by number of holes in Disc
      robotState.rpmRF = (counterRF * 3);  // divide by number of holes in Disc
      robotState.rpmLR = (counterLR * 3);  // divide by number of holes in Disc
      robotState.rpmRR = (counterRR * 3);  // divide by number of holes in Disc
      
      serialprintf(Serial, "{ \"WheelRPM:\" {\"LF\": %d,\"LR\": %d,\"RF\": %d,\"RR\": %d }, \"WheelDist:\" { \"LF\": %f,\"LR\": %f,\"RF\": %f,\"RR\": %f}}%s", 
      counterLF,counterLR,robotState.rpmRF,robotState.rpmRR,
      robotState.distLF,robotState.distLR,robotState.distRF,robotState.distRR,
      ""
      );/**/
      
      robotState.distLF += (counterLF * DISTANCE_PER_ENCODER_SLOT_IN_CM);
      robotState.distLR += (counterLR * DISTANCE_PER_ENCODER_SLOT_IN_CM); 
      robotState.distRF += (counterRF * DISTANCE_PER_ENCODER_SLOT_IN_CM); 
      robotState.distRR += (counterRR * DISTANCE_PER_ENCODER_SLOT_IN_CM);
      
      
      counterLF=0, 
      counterRF=0, 
      counterLR=0, 
      counterRR=0;
    
    
      
      Timer1.attachInterrupt( timerOdometryAndMPU );  //enable the timer
  }
}
