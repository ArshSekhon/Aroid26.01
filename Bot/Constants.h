//SENSOR PINS
#define TEMPERATURE_SENSOR_PIN A0
#define PHOTORESITOR_READ_PIN A1
#define BUZZER_PIN 8
#define SONAR_TRIG_PIN 11
#define SONAR_ECHO_PIN 12

#define ODOMETER_RIGHT_FRONT_PIN 3
#define ODOMETER_RIGHT_REAR_PIN 2 
#define ODOMETER_LEFT_FRONT_PIN 19
#define ODOMETER_LEFT_REAR_PIN 18


//MOTOR PINS
#define LAIN1 43 
#define LAIN2 42
#define LPWMA 5
#define LBIN1 45
#define LBIN2 44
#define LPWMB 4
#define RAIN1 48
#define RAIN2 49
#define RPWMA 7
#define RBIN1 46
#define RBIN2 47
#define RPWMB 6

#define SERVO_BASE_PIN 9
#define SERVO_ARM_PIN 10


#define SWITCH_PIN 41
#define KILL_PIN 51

#define TEMP_UNITS_CELCIUS 0
#define TEMP_UNITS_FAHRENHEIT 1
#define TEMP_UNITS_KELVIN 2

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 

#define ARDUINO_IP "192.168.0.99"
#define CAMERA_IP "192.168.0.100"
#define NETWORK_GATEWAY  "192.168.0.1"
#define SUBNET  "255.255.255.0"


// I2C Addresses
// 0x3C for screen
// 0x68 for gyroscope
#define SCREEN_I2C_ADDRESS 0x3C
#define MPU9265_I2C_ADDRESS 0x68

// struct to store readings from MPU 9250
struct MPU9265_Reading{
    int ax=0,ay=0,az=0,
            gx=0,gy=0,gz=0,
            mx=0,my=0,mz=0;
    float headingDegrees, headingFiltered;
  };

 // struct to store robots state
struct RobotState{
    // variables to hold motor speed
    int motorSpeedLF=0 ,motorSpeedLR=0, motorSpeedRF=0, motorSpeedRR = 0;
    // variables to hold servo positions
    int cameraBasePos=90, cameraArmPos=150;
    // record RPM
    volatile int rpmLF=0, rpmLR=0, rpmRF=0, rpmRR = 0;
    // odometry
    volatile double distLF=0, distLR=0, distRF=0, distRR = 0; 
    
    // temperature
    float temperatureInC=0;
    // distance from ultrasonic sensor
    float distanceUltrasonic=0;
    // light levels
    float lightLevels=0;
    
    //start Distance
    float startDistance=0;
    //target Distance
    float targetDistance=0;
    
    // struct to store the reading from the the MPU 9265
    MPU9265_Reading mpu9265Reading;
};
