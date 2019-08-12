float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(SONAR_TRIG_PIN, LOW);

  echoTime = pulseIn(SONAR_ECHO_PIN, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0 * 2.54;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

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


 void play()
{
  char note='A';
  int beats = 10;
  int numNotes = 14; 

  //this array is used to look up the notes
  char notes[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B', ' '};
  //this array matches frequencies with each letter (e.g. the 4th note is 'f', the 4th frequency is 175)
  int frequencies[] = {131, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 0};

  int currentFrequency = 0;    //the frequency that we find when we look up a frequency in the arrays
  int beatLength = 150;   //the length of one beat (changing this will speed up or slow down the tempo of the song)

  //look up the frequency that corresponds to the note
  for (int i = 0; i < numNotes; i++)  // check each value in notes from 0 to 14
  {
    if (notes[i] == note)             // does the letter passed to the play function match the letter in the array?
    {
      currentFrequency = frequencies[i];   // Yes! Set the current frequency to match that note
    }
  }

  //play the frequency that matched our letter for the number of beats passed to the play function
  tone(BUZZER_PIN, currentFrequency, beats * beatLength);   
  delay(beats* beatLength);   //wait for the length of the tone so that it has time to play
  delay(50);                  //a little delay between the notes makes the song sound more natural

}
