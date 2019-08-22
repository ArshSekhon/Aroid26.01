void setup() {
 
  pinMode(SONAR_TRIG_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT_PULLUP);
  
  // set switch pin to INPUT_PULLUP mode
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  
  // FRONT-LEFT MOTOR
  // set control pins for front-left motor to OUTPUT mode
  pinMode(LAIN1, OUTPUT);
  pinMode(LAIN2, OUTPUT);
  // set PWM speed control pin for front-left motor to OUTPUT mode
  pinMode(LPWMA, OUTPUT);
  
  // REAR-LEFT MOTOR
  // set control pins for rear-left motor to OUTPUT mode
  pinMode(LBIN1, OUTPUT);
  pinMode(LBIN2, OUTPUT);
  // set PWM speed control pin for rear-left motor to OUTPUT mode
  pinMode(LPWMB, OUTPUT);
  
  // FRONT-RIGHT MOTOR
  // set control pins for front-right motor to OUTPUT mode
  pinMode(RAIN1, OUTPUT);
  pinMode(RAIN2, OUTPUT);
  // set PWM speed control pin for front-right motor to OUTPUT mode
  pinMode(RPWMA, OUTPUT);

  // REAR-RIGHT MOTOR
  // set control pins for rear-right motor to OUTPUT mode
  pinMode(RBIN1, OUTPUT);
  pinMode(RBIN2, OUTPUT);
  // set PWM speed control pin for rear-right motor to OUTPUT mode
  pinMode(RPWMB, OUTPUT);
  

  pinMode(BUZZER_PIN, OUTPUT);   
  pinMode(KILL_PIN, INPUT_PULLUP); 
  
  cameraBaseServo.attach(SERVO_BASE_PIN);
  cameraArmServo.attach(SERVO_ARM_PIN);

  
  enableInterrupt(SONAR_ECHO_PIN, ultrasonicSensorInterruptHandler, CHANGE);
  //begin Serial communication on 9600 baud
  Serial.begin(9600);
  Serial3.begin(9600);
  
  initOdometry();
  initMPU9265();
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();
  positionRobotHead(robotState.cameraBasePos, robotState.cameraArmPos);
  
  serialprintf(Serial,"Motor Speed: LF %d LR %d RF %d RR %d",robotState.motorSpeedLF,robotState.motorSpeedLR,robotState.motorSpeedRF,robotState.motorSpeedRR);
  Serial.print("Setup Complete");
}
