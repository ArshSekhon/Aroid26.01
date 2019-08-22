// These functions are needed because Aroid uses interrupts for ultrasonic sensor
// send out the signal on TRIG pin and wait for response.
void startUltrasonicSensorReading(){
  ultrasonic_sensor_finished=false;
  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG_PIN, LOW);
}
// get the latest reading that successfully finished
unsigned int getPreviousDistanceFromUltrasonicSensor(){
  return (ultrasonic_sensor_end-ultrasonic_sensor_start)/(58);
}

// serve interrupts on the SONAR_ECHO_PIN
void ultrasonicSensorInterruptHandler() {
  switch(digitalRead(SONAR_ECHO_PIN)){
    case HIGH:
      ultrasonic_sensor_start=micros();
      break;
    case LOW:
      ultrasonic_sensor_end=micros();
      ultrasonic_sensor_finished=true;
      break;
  }
}
