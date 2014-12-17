#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

MPU6050 accelgyro;

unsigned long last_read_time;
float         last_x_angle;
float         last_y_angle;
float         last_z_angle;  

float base_x_acc, base_y_acc, base_z_acc;
float base_x_gyr, base_y_gyr, base_z_gyr;

int16_t ax, ay, az;
int16_t gx, gy, gz;
double Ax, Ay, Az, Gx, Gy, Gz;

void calibrate_sensors(){
  int    num_readings = 10;
  float  x_acc = 0;
  float  y_acc = 0;
  float  z_acc = 0;
  float  x_gyr = 0;
  float  y_gyr = 0;
  float  z_gyr = 0;

  for(int i=0; i < num_readings; i++){
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    x_acc += ax;
    y_acc += ay;
    z_acc += az;
    x_gyr += gz;
    y_gyr += gy;
    z_gyr += gz;
    delay(100);
  }
  x_acc /= num_readings;
  y_acc /= num_readings;
  z_acc /= num_readings;
  x_gyr /= num_readings;
  y_gyr /= num_readings;
  z_gyr /= num_readings;

  // Store the raw calibration values globally
  base_x_acc = x_acc;
  base_y_acc = y_acc;
  base_z_acc = z_acc;
  base_x_gyr = x_gyr;
  base_y_gyr = y_gyr;
  base_z_gyr = z_gyr;
}

void set_last_read_angle_data (unsigned long time, float x, float y, float z){
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
}

void setup(){
  Wire.begin();

  Serial.begin(19200);
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? 
  "MPU6050 connection successful" : "MPU6050 connection failed");
  calibrate_sensors();
  set_last_read_angle_data(millis(), 0, 0, 0);
}

void loop(){
  double dT;
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // Get the time of reading for rotation computations
  unsigned long t_now = millis();
  
  Ax = 2*9.8*ax/32768;
  Ay = 2*9.8*ay/32768;
  Az = 2*9.8*az/32768;
  Gx = (gx-base_x_gyr)/131.0;
  Gy = (gy-base_y_gyr)/131.0;
  Gz = (gz-base_z_gyr)/131.0;
  
  // Get angle values from accelerometer
  float RADIANS_TO_DEGREES = 180/3.14159;
  float angAccY = atan(-1*Ax/sqrt(pow(Ay,2) + pow(Az,2)))*RADIANS_TO_DEGREES;
  float angAccX = atan(Ay/sqrt(pow(Ax,2) + pow(Az,2)))*RADIANS_TO_DEGREES;
  
  float dt =(t_now - last_read_time)/1000.0;
  float angGyrX = (Gx*dt + last_x_angle);
  float angGyrY = (Gy*dt + last_y_angle);
  float angGyrZ = (Gz*dt + last_z_angle);
  
  float alpha = 0.96;
  float angle_x = alpha*angGyrX + (1.0 - alpha)*angAccX;
  float angle_y = alpha*angGyrY + (1.0 - alpha)*angAccY;
  float angle_z = angGyrZ;  //Accelerometer doesn't give z-angle
  
  set_last_read_angle_data(t_now, angle_x, angle_y, angle_z);
  
  Serial.print(F("DEL:"));   Serial.print(dt, DEC);
  Serial.print(F("#ACC:"));              
  Serial.print(angAccX, 2);       Serial.print(F(","));
  Serial.print(angAccY, 2);       Serial.print(F(","));
  Serial.print(0, 2);
  Serial.print(F("#GYR:"));
  Serial.print(Gx, 2);       Serial.print(F(","));
  Serial.print(Gy, 2);       Serial.print(F(","));
  Serial.print(Gz, 2);
  Serial.print(F("#FIL:"));             
  Serial.print(angle_x, 2);  Serial.print(F(","));
  Serial.print(angle_y, 2);  Serial.print(F(","));
  Serial.print(angle_z, 2);  Serial.println(F(""));
  
  delay(5);
}
