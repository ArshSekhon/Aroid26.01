String inputType;

// this handles commands received via the specified Serial Input
void handleHardwareSerialQueries(HardwareSerial &serial){
  
   if (serial.available() > 0){      
   
        inputType = serial.readStringUntil(';');
        
        Serial.print("Serial Instructions available:");
        Serial.println((&Serial3==&serial)?"Serial3":"Serial");
        Serial.println(inputType); 
        
        // Command: WRITE_MOTORS;{MOTOR_SPEED_LEFT_FRONT};{MOTOR_SPEED_LEFT_READ};{MOTOR_SPEED_LEFT_FRONT};{MOTOR_SPEED_LEFT_READ};
        // Action:  Starts spinning motors at specified speed
        if(inputType=="WRITE_MOTORS"){
          robotState.motorSpeedLF = serial.readStringUntil(';').toInt();       
          robotState.motorSpeedLR = serial.readStringUntil(';').toInt();         
          robotState.motorSpeedRF = serial.readStringUntil(';').toInt(); 
          robotState.motorSpeedRR = serial.readStringUntil(';').toInt();
          spinMotors(robotState.motorSpeedLF, robotState.motorSpeedLR, robotState.motorSpeedRF, robotState.motorSpeedRR); 
        }
        
        // Command: HEAD_POS;{BASE_POS_DEG};{ARM_POS_DEG};
        // Action:  Positions head using the specified position in degrees for both arm and base
        else if(inputType=="HEAD_POS"){
          robotState.cameraBasePos = serial.readStringUntil(';').toInt();          
          robotState.cameraArmPos = serial.readStringUntil(';').toInt(); 
          serialprintf(Serial, "Servo Moved --> Head: %d Arm %d", robotState.cameraBasePos, robotState.cameraArmPos );
          positionRobotHead(robotState.cameraBasePos, robotState.cameraArmPos);
        }
        
        // Command: ROT_C;{DEGREES_TO_ROTATE};
        // Action:  Rotates Aroid Clockwise by specified angle in degrees
        else if(inputType=="ROT_C"){
          int degreesToRot = serial.readStringUntil(';').toInt();      
          int speed = serial.readStringUntil(';').toInt();    
          serialprintf(Serial, "Rotating Clockwise: %d Degrees", robotState.cameraBasePos, robotState.cameraArmPos );
          spinMotorsToRotateClockwise(degreesToRot,speed);
        }

        // Command: ROT_A;{DEGREES_TO_ROTATE};
        // Action:  Rotates Aroid AntiClockwise by specified angle in degrees
        else if(inputType=="ROT_A"){
          int degreesToRot = serial.readStringUntil(';').toInt();  
          int speed = serial.readStringUntil(';').toInt();        
          serialprintf(Serial, "Rotating Anticlockwise: %d Degrees", robotState.cameraBasePos, robotState.cameraArmPos );
          spinMotorsToRotateAntiClockwise(degreesToRot,speed);
        }

        
        // Command: SET_TARGET_DIST;{DISTANCE_TO_TRAVEL};{MOTOR_SPEED_TO_USE};
        // Action:  Moves Aroid by specified distance using given motor speed
        else if(inputType=="SET_TARGET_DIST"){
          float distance = serial.readStringUntil(';').toInt();  
          int speed = serial.readStringUntil(';').toInt();        
          setTargetDistance(distance, speed);
          serialprintf(Serial, "Heading for distance: %f cm", robotState.targetDistance );
        }

        else if(inputType=="READ"){
          String type = serial.readStringUntil(';');
    
          // Command: READ;MAG;
          // Action:  Responds with a JSON objet that contains readings from MPU-9250
          if(type=="MAG"){
              readFromMPU9265();
              printAllMPU9265Readings();
              printAllMPU9265ReadingsToSerial(serial);
            }
            
          // Command: READ;AUX_SENSORS;
          // Action:  Responds with a JSON objet that contains readings from  Temperature sensor, photoresistor and ultrasonic sensor
          else if(type=="AUX_SENSORS"){
             printRobotSensorReadingsToSerial(serial);
           }

           
          // Command: READ;ORIENTATION;
          // Action:  Responds with a JSON objet that contains odometry and heading data
          else if(type=="ORIENTATION"){
             printRobotPositionInSpaceToSerial(serial);
           }
        }
        
        // Command: RESET_ODO;
        // Action:  Resets the odometer for all the wheels to zero
        else if(inputType=="RESET_ODO"){
          
            robotState.distLF = 0;
            robotState.distLR = 0; 
            robotState.distRF = 0; 
            robotState.distRR = 0;
          }
        serial.readStringUntil('\n');
    }
}

// prints  a JSON object containing readings from Temperature sensor, photoresistor and ultrasonic sensor to the specified Serial port
void printRobotSensorReadingsToSerial(HardwareSerial &serial){
  serialprintf(serial,"{\"TemperatureCelcius\": %f,\"DistanceUltrasonic\": %f, \"LightLevels\": %f }%s", 
      robotState.temperatureInC,
      robotState.distanceUltrasonic,
      robotState.lightLevels,
      robotState.rpmLF,robotState.rpmLR,robotState.rpmRF,robotState.rpmRR,
      robotState.distLF,robotState.distLR,robotState.distRF,robotState.distRR,
      ""
      );
  
  }
 // prints  a JSON object containing information regarding Aroid's position in space
void printRobotPositionInSpaceToSerial(HardwareSerial &serial){
  serialprintf(serial, "{ \"WheelRPM\": {\"LF\": %d,\"LR\": %d,\"RF\": %d,\"RR\": %d }, \"WheelDist\": { \"LF\": %f,\"LR\": %f,\"RF\": %f,\"RR\": %f}, \"MPU\": {\"ax\": %d, \"ay\": %d, \"az\": %d, \"gx\": %d, \"gy\": %d, \"gz\": %d, \"mx\": %d, \"my\": %d, \"mz\": %d, \"Heading\": %f}}%s", 
    robotState.rpmLF,robotState.rpmLR,robotState.rpmRF,robotState.rpmRR,
    robotState.distLF,robotState.distLR,robotState.distRF,robotState.distRR,
    robotState.mpu9265Reading.ax, robotState.mpu9265Reading.ay, robotState.mpu9265Reading.az, 
    robotState.mpu9265Reading.gx, robotState.mpu9265Reading.gy, robotState.mpu9265Reading.gz, 
    robotState.mpu9265Reading.mx, robotState.mpu9265Reading.my, robotState.mpu9265Reading.mz,
    robotState.mpu9265Reading.headingDegrees,
    ""
    );
}
