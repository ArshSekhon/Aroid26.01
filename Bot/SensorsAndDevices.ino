float prevDist=0;
void updateStateFromSensors(){
    readFromMPU9265();
    robotState.lightLevels = getLightLevels();
    robotState.distanceUltrasonic = getDistance();
    robotState.temperatureInC = getTempratureReading(TEMP_UNITS_CELCIUS);    
    
  }

float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time
  //Timer1.stop(); noInterrupts();
  
  digitalWrite(SONAR_TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG_PIN, LOW);

  echoTime = pulseIn(SONAR_ECHO_PIN, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor
  //Timer1.resume();interrupts();
  calculatedDistance = (echoTime/2) / 29.1;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  if(calculatedDistance<1){
    calculatedDistance= prevDist;
    }
    else{
      prevDist= prevDist*0.9+calculatedDistance*0.1;
      
      }

  return calculatedDistance;              //send back the distance that was calculated
}
float getTempratureReading(int tempUnits){

    float voltage = 0;
    float degreesC = 0;
    float degreesF = 0;
    float degreesK = 0;

    
    voltage = analogRead(TEMPERATURE_SENSOR_PIN) * 0.004882814;   //convert the analog reading, which varies from 0 to 1023, back to a voltage value from 0-5 volts
    degreesC = (voltage - 0.5) * 100.0;       //convert the voltage to a temperature in degrees Celsius
    degreesF = degreesC * (9.0/5.0) + 32.0;   //convert the voltage to a temperature in degrees Fahrenheit
    degreesK = degreesC + 273.15;   
  
    
    if(tempUnits==TEMP_UNITS_CELCIUS)
      return degreesC;       
    else if(tempUnits==TEMP_UNITS_FAHRENHEIT)
      return degreesF;  
    else if(tempUnits==TEMP_UNITS_KELVIN)
      return degreesK; 
}

int getLightLevels(){
  return analogRead(PHOTORESITOR_READ_PIN);
}
void playBackingUpSound(){
  
  tone(BUZZER_PIN, 440, 250);   
  delay(1000);
  tone(BUZZER_PIN, 0, 250);  
  delay(50);
  tone(BUZZER_PIN, 440, 250); 
  delay(1000);
  tone(BUZZER_PIN, 0, 250);   
  delay(50);
  
  
}
void playDangerSound(){
  tone(BUZZER_PIN, 440, 1500);   
  
  }
