void setup() {
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
  cameraBaseServo.write(90);

  
  //begin Serial communication on 9600 baud
  Serial.begin(9600);
  Serial3.begin(9600);
  
  initOdometry();
  initMPU9265();
  
  serialprintf("INTERRUPTS TABLE \nLF: %d, LR: %d, RF: %d, RR: %d", digitalPinToInterrupt(ODOMETER_LEFT_FRONT_PIN), digitalPinToInterrupt(ODOMETER_LEFT_REAR_PIN), digitalPinToInterrupt(ODOMETER_RIGHT_FRONT_PIN), digitalPinToInterrupt(ODOMETER_RIGHT_REAR_PIN));
  

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
  
  serialprintf("Motor Speed: LF %d LR %d RF %d RR %d",robotState.motorSpeedLF,robotState.motorSpeedLR,robotState.motorSpeedRF,robotState.motorSpeedRR);
  Serial.print("Setup Complete");
}


/*
  // Draw a single pixel in white
  display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
  display.clearDisplay();

  display.setTextSize(1.5);
  display.setCursor(0,20);
  display.setTextColor(WHITE);
  display.print("Hello World!");
  display.display();
  
*/
