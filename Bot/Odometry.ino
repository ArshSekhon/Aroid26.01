
unsigned int counterLF=0, 
             counterRF=0, 
             counterLR=0, 
             counterRR=0;

             
void incCounterLF()
{
  counterLF++;
  counterLF1++;
  
} 
void incCounterRF()
{
  counterRF++;
  counterRF1++;
} 
void incCounterLR()
{
  counterLR++;
  counterLR1++;
} 
void incCounterRR()
{
  counterRR++;
  counterRR1++;
} 

void initOdometry(){
  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(digitalPinToInterrupt(ODOMETER_LEFT_FRONT_PIN), incCounterLF, RISING); 
  attachInterrupt(digitalPinToInterrupt(ODOMETER_LEFT_REAR_PIN), incCounterLR, RISING); 
  attachInterrupt(digitalPinToInterrupt(ODOMETER_RIGHT_FRONT_PIN), incCounterRF, RISING); 
  attachInterrupt(digitalPinToInterrupt(ODOMETER_RIGHT_REAR_PIN), incCounterRR, RISING); 
  Timer1.attachInterrupt(timerOdometry ); // enable the timer
}

void timerOdometry()
{
  Timer1.detachInterrupt();  //stop the timer
  
  robotState.rpmLF = (counterLF * 3);  // divide by number of holes in Disc
  robotState.rpmRF = (counterRF * 3);  // divide by number of holes in Disc
  robotState.rpmLR = (counterLR * 3);  // divide by number of holes in Disc
  robotState.rpmRR = (counterRR * 3);  // divide by number of holes in Disc
  
  //serialprintf("Motor RPM ---> LF: %d rpm RF: %d rpm LR: %d rpm RR: %d rpm",robotState.rpmLF,robotState.rpmRF, robotState.rpmLR, robotState.rpmRR);
  
  counterLF=0, 
  counterRF=0, 
  counterLR=0, 
  counterRR=0;
  
  Timer1.attachInterrupt( timerOdometry );  //enable the timer
}
