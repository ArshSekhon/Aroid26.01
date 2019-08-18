
#include <Wire.h>
#include <TimerOne.h>
 
#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C
 
#define GYRO_FULL_SCALE_250_DPS 0x00 
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18
 
#define ACC_FULL_SCALE_2_G 0x00 
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

#define SMOOTHING_FACTOR 10
int mag_read_index =0;
float heading_readings[SMOOTHING_FACTOR];
float total_heading;

// This function read Nbytes bytes from I2C device at address Address. 
// Put read bytes starting at register Register in the Data array. 
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
    // Set register address
    Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.endTransmission();
     
    // Read Nbytes
    Wire.requestFrom(Address, Nbytes);
    uint8_t index=0;
    while (Wire.available())
    Data[index++]=Wire.read();
}
 
// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
    // Set register address
    Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.write(Data);
    Wire.endTransmission();
}
 
// Initial time
long int ti;
volatile bool intFlag=false;

void initMPU9265(){
      
    // Arduino initializations
    Wire.begin();
    // Set accelerometers low pass filter at 5Hz
    I2CwriteByte(MPU9250_ADDRESS,29,0x06);
    // Set gyroscope low pass filter at 5Hz
    I2CwriteByte(MPU9250_ADDRESS,26,0x06);
     
     
    // Configure gyroscope range
    I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_1000_DPS);
    // Configure accelerometers range
    I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_4_G);
    // Set by pass mode for the magnetometers
    I2CwriteByte(MPU9250_ADDRESS,0x37,0x02);
     
    // Request continuous magnetometer measurements in 16 bits
    I2CwriteByte(MAG_ADDRESS,0x0A,0x16);
    //Timer1.initialize(10000); // initialize timer1, and set a 1/2 second period
    //Timer1.attachInterrupt(callback); // attaches callback() as a timer overflow interrupt
     
    // Store initial time
    ti=millis();
     
}
 
// Counter
long int cpt=0; 

void readFromMPU9265(){
    while (!intFlag);
    intFlag=false;
    // ____________________________________
    // ::: Accelerometer and Gyroscope :::
     
    // Read accelerometer and gyroscope
    uint8_t Buf[14];
    I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
     
    // Create 16 bits values from 8 bits data
     
    // Accelerometer
    robotState.mpu9265Reading.ax=-(Buf[0]<<8 | Buf[1]);
    robotState.mpu9265Reading.ay=-(Buf[2]<<8 | Buf[3]);
    robotState.mpu9265Reading.az=Buf[4]<<8 | Buf[5];
    
    robotState.mpu9265Reading.gx=-(Buf[8]<<8 | Buf[9]);
    robotState.mpu9265Reading.gy=-(Buf[10]<<8 | Buf[11]);
    robotState.mpu9265Reading.gz=Buf[12]<<8 | Buf[13];
     
    // Read register Status 1 and wait for the DRDY: Data Ready
     
    uint8_t ST1;
    do
    {
    I2Cread(MAG_ADDRESS,0x02,1,&ST1);
    }
    while (!(ST1&0x01));
     
    // Read magnetometer data 
    uint8_t Mag[7];
    I2Cread(MAG_ADDRESS,0x03,7,Mag);
     
    // Create 16 bits values from 8 bits data
    
    // Magnetometer
    int16_t mx=(Mag[3]<<8 | Mag[2]);
    int16_t my=(Mag[1]<<8 | Mag[0]);
    int16_t mz=(Mag[5]<<8 | Mag[4]);
    
    robotState.mpu9265Reading.mx= mx * 42.0/(1630.0-1430.0) + 21 - 1630*42.0/(1630.0-1430.0);
    robotState.mpu9265Reading.my= my *42.0/(263.0-47.0) + 21.0 -(263.0*42.0/(263.0-47.0));
    
    
    // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
    // Calculate heading when the magnetometer is level, then correct for signs of axis.
    float heading = atan2(robotState.mpu9265Reading.my,robotState.mpu9265Reading.mx);
    
    // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
    // Find yours here: http://www.magnetic-declination.com/
    // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
    // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
    //float declinationAngle = 0.27611109;
    //heading += declinationAngle;
    
    // Correct for when signs are reversed.
    if(heading < 0)
      heading += 2*PI;
      
    // Check for wrap due to addition of declination.
    if(heading > 2*PI)
      heading -= 2*PI;
     
    // Convert radians to degrees for readability.
    robotState.mpu9265Reading.headingDegrees = heading * 180/M_PI - 90; 
    robotState.mpu9265Reading.headingFiltered = robotState.mpu9265Reading.headingFiltered*0.25 + robotState.mpu9265Reading.headingDegrees*0.75; 
    /*
    total_heading = total_heading - heading_readings[mag_read_index];
    // read from the sensor:
    heading_readings[mag_read_index] = robotState.mpu9265Reading.headingDegrees;
    // add the reading to the total:
    total_heading = total_heading + heading_readings[mag_read_index];
    
    // advance to the next position in the array:
    mag_read_index++;
  
    // if we're at the end of the array...
    if (mag_read_index >= SMOOTHING_FACTOR) {
      // ...wrap around to the beginning:
      mag_read_index = 0;
    }
  
    // calculate the average:
    robotState.mpu9265Reading.headingFiltered = total_heading / SMOOTHING_FACTOR;  
     */
}

void printAccelerometerReadings(){
  serialprintf(Serial,"ax: %d, ay: %d, az: %d", 
                robotState.mpu9265Reading.ax, robotState.mpu9265Reading.ay, robotState.mpu9265Reading.az);
  
  }

void printGyroscopeReadings(){
  serialprintf(Serial,"gx: %d, gy: %d, gz: %d", 
                robotState.mpu9265Reading.gx, robotState.mpu9265Reading.gy, robotState.mpu9265Reading.gz);
  
  }

void printMagnetometerReadings(){
  serialprintf(Serial,"mx: %d, my: %d, mz: %d, Heading: %f degrees, HeadingFiltered: %f Quadrant %d", 
                robotState.mpu9265Reading.mx, robotState.mpu9265Reading.my, robotState.mpu9265Reading.mz, robotState.mpu9265Reading.headingDegrees, robotState.mpu9265Reading.headingFiltered, isAround360(robotState.mpu9265Reading.headingFiltered));
  
  }
  
void printAllMPU9265Readings(){
  serialprintf(Serial, "ax: %d, ay: %d, az: %d, gx: %d, gy: %d, gz: %d, mx: %d, my: %d, mz: %d, Heading: %f degrees", 
                robotState.mpu9265Reading.ax, robotState.mpu9265Reading.ay, robotState.mpu9265Reading.az, 
                robotState.mpu9265Reading.gx, robotState.mpu9265Reading.gy, robotState.mpu9265Reading.gz, 
                robotState.mpu9265Reading.mx, robotState.mpu9265Reading.my, robotState.mpu9265Reading.mz,
                robotState.mpu9265Reading.headingDegrees);
  
  }
 void printAllMPU9265ReadingsToSerial(HardwareSerial &serial){
  serialprintf(serial, "{\"ax\": %d, \"ay:\" %d, \"az:\" %d, \"gx\": %d, \"gy\": %d, \"gz\": %d, \"mx\": %d, \"my\": %d, \"mz\": %d, \"Heading\": %f} %s", 
                robotState.mpu9265Reading.ax, robotState.mpu9265Reading.ay, robotState.mpu9265Reading.az, 
                robotState.mpu9265Reading.gx, robotState.mpu9265Reading.gy, robotState.mpu9265Reading.gz, 
                robotState.mpu9265Reading.mx, robotState.mpu9265Reading.my, robotState.mpu9265Reading.mz,
                robotState.mpu9265Reading.headingDegrees,  "");
  
  }
