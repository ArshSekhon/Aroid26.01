
void handleHardwareSerialQueries(HardwareSerial &serial){
  
   if (serial.available() > 0){      
   
        inputType = serial.readStringUntil(';');
        
        Serial.print("Serial Instructions available:");
        Serial.println((&Serial3==&serial)?"Serial3":"Serial");
        Serial.println(inputType); 
        
        if(inputType=="WRITE_MOTORS"){
          robotState.motorSpeedLF = serial.readStringUntil(';').toInt();       
          robotState.motorSpeedLR = serial.readStringUntil(';').toInt();         
          robotState.motorSpeedRF = serial.readStringUntil(';').toInt(); 
          robotState.motorSpeedRR = serial.readStringUntil(';').toInt();
          spinMotors(robotState.motorSpeedLF, robotState.motorSpeedLR, robotState.motorSpeedRF, robotState.motorSpeedRR); 
        }
        else if(inputType=="HEAD_POS"){
          robotState.cameraBasePos = serial.readStringUntil(';').toInt();          
          robotState.cameraArmPos = serial.readStringUntil(';').toInt(); 
          serialprintf(serial, "Servo Moved --> Head: %d Arm %d", robotState.cameraBasePos, robotState.cameraArmPos );
          positionRobotHead(robotState.cameraBasePos, robotState.cameraArmPos);
        }
        else if(inputType=="ROT_C"){
          int degreesToRot = serial.readStringUntil(';').toInt();      
          int speed = serial.readStringUntil(';').toInt();    
          serialprintf(serial, "Rotating Clockwise: %d Degrees", robotState.cameraBasePos, robotState.cameraArmPos );
          spinMotorsToRotateClockwise(degreesToRot,speed);
        }
        else if(inputType=="ROT_A"){
          int degreesToRot = serial.readStringUntil(';').toInt();  
          int speed = serial.readStringUntil(';').toInt();        
          serialprintf(serial, "Rotating Anticlockwise: %d Degrees", robotState.cameraBasePos, robotState.cameraArmPos );
          spinMotorsToRotateAntiClockwise(degreesToRot,speed);
        }
        else if(inputType=="SET_TARGET_DIST"){
          float distance = serial.readStringUntil(';').toInt();  
          int speed = serial.readStringUntil(';').toInt();        
          setTargetDistance(distance, speed);
          serialprintf(serial, "Heading for distance: %f cm", robotState.targetDistance );
        }
        else if(inputType=="READ"){
          String type = serial.readStringUntil(';');
    
          if(type=="MAG"){
              readFromMPU9265();
              printAllMPU9265Readings();
              printAllMPU9265ReadingsToSerial(serial);
            }
          else if(type=="AUX_SENSORS"){
             printRobotSensorReadingsToSerial(serial);
           }
          else if(type=="ORIENTATION"){
             printRobotPositionInSpaceToSerial(serial);
           }
        }
        serial.readStringUntil('\n');
    }
}

void printRobotSensorReadingsToSerial(HardwareSerial &serial){
  serialprintf(serial,"{\"TemperatureCelcius:\" %f,\"DistanceUltrasonic:\" %f, \"LightLevels:\" %f }%s", 
      robotState.temperatureInC,
      robotState.distanceUltrasonic,
      robotState.lightLevels,
      robotState.rpmLF,robotState.rpmLR,robotState.rpmRF,robotState.rpmRR,
      robotState.distLF,1,robotState.distRF,1,
      ""
      );
  
  }
  
void printRobotPositionInSpaceToSerial(HardwareSerial &serial){
  serialprintf(serial, "{ \"WheelRPM:\" {\"LF\": %d,\"LR\": %d,\"RF\": %d,\"RR\": %d }, \"WheelDist:\" { \"LF\": %f,\"LR\": %f,\"RF\": %f,\"RR\": %f}, \"MPU\": {\"ax\": %d, \"ay:\" %d, \"az:\" %d, \"gx\": %d, \"gy\": %d, \"gz\": %d, \"mx\": %d, \"my\": %d, \"mz\": %d, \"Heading\": %f}}%s", 
    robotState.rpmLF,robotState.rpmLR,robotState.rpmRF,robotState.rpmRR,
    robotState.distLF,robotState.distLR,robotState.distRF,robotState.distRR,
    robotState.mpu9265Reading.ax, robotState.mpu9265Reading.ay, robotState.mpu9265Reading.az, 
    robotState.mpu9265Reading.gx, robotState.mpu9265Reading.gy, robotState.mpu9265Reading.gz, 
    robotState.mpu9265Reading.mx, robotState.mpu9265Reading.my, robotState.mpu9265Reading.mz,
    robotState.mpu9265Reading.headingDegrees,
    ""
    );
}
