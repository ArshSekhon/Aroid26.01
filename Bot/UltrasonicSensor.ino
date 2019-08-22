
void startUltrasonicSensorReading(){
  ultrasonic_sensor_finished=false;
  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG_PIN, LOW);
}

unsigned int getPreviousDistanceFromUltrasonicSensor(){
  return (ultrasonic_sensor_end-ultrasonic_sensor_start)/(58);
}

// serve interrupts per pin da 8 a 13
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
