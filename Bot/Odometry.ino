#define DISTANCE_PER_ENCODER_SLOT_IN_CM 0.2501
unsigned int counterLF=0, 
             counterRF=0, 
             counterLR=0, 
             counterRR=0;
volatile int odotimerTicks=0;

// interrupt handlers that increment counters when LM393 sensors cause interrupts             
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



// attaches the interrupt handlers for odometry and creates a timer for periodic RPM calculation and setting MPU intFlag 
void initOdometry(){
  Timer1.initialize(10000); // set timer for 1sec
  enableInterrupt((ODOMETER_LEFT_FRONT_PIN), incCounterLF, RISING); 
  enableInterrupt((ODOMETER_LEFT_REAR_PIN), incCounterLR, RISING); 
  enableInterrupt((ODOMETER_RIGHT_FRONT_PIN), incCounterRF, RISING); 
  enableInterrupt((ODOMETER_RIGHT_REAR_PIN), incCounterRR, RISING); 
  Timer1.attachInterrupt(timerOdometryAndMPU ); // enable the timer
  
}

//  function for timer that does RPM calculation and sets MPU intFlag 
void timerOdometryAndMPU()
{
  intFlag=true;
  if(++odotimerTicks>10){
      odotimerTicks=0;
      Timer1.detachInterrupt();  //stop the timer
      robotState.rpmLF = (counterLF * 30);  // divide by number of holes in Disc
      robotState.rpmRF = (counterRF * 30);  // divide by number of holes in Disc
      robotState.rpmLR = (counterLR * 30);  // divide by number of holes in Disc
      robotState.rpmRR = (counterRR * 30);  // divide by number of holes in Disc
      
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
