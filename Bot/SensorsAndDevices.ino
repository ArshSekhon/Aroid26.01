float prevDist=0;
// perform the sensory readings and update the robot state
void updateStateFromSensors(){
    readFromMPU9265();
    robotState.lightLevels = getLightLevels();

    if(ultrasonic_sensor_finished){ 
      if(getPreviousDistanceFromUltrasonicSensor()!=0)
        robotState.distanceUltrasonic = getPreviousDistanceFromUltrasonicSensor(); 
      startUltrasonicSensorReading();
      delay(100);
    }
  
    robotState.temperatureInC = getTempratureReading(TEMP_UNITS_CELCIUS);    
    
  }
// get readings from the temperature sensor
float getTempratureReading(int tempUnits){

    float voltage = 0;
    float degreesC = 0;
    float degreesF = 0;
    float degreesK = 0;

    
    voltage = analogRead(TEMPERATURE_SENSOR_PIN) * 0.004882814;   
    degreesC = (voltage - 0.5) * 100.0;   
    degreesF = degreesC * (9.0/5.0) + 32.0;
    degreesK = degreesC + 273.15;   
  
    
    if(tempUnits==TEMP_UNITS_CELCIUS)
      return degreesC;       
    else if(tempUnits==TEMP_UNITS_FAHRENHEIT)
      return degreesF;  
    else if(tempUnits==TEMP_UNITS_KELVIN)
      return degreesK; 
}

// get readings from the photoresistor
int getLightLevels(){
  return analogRead(PHOTORESITOR_READ_PIN);
}

// play sound on the buzzer
void playDangerSound(){
  tone(BUZZER_PIN, 440, 1500);   
  }
