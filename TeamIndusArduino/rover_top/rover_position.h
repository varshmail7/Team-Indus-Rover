/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 12-06-2018
   Version Info:
    0.1 Base Version
*/

#ifndef POSITION_H

#define POSITION_H


#ifndef POSITION_UNIT_TEST
#define POSITION_UNIT_TEST // Uncomment for Rover Position Unit Testing using IMU
#endif

// Header Files
#include "display.h"
#include<Wire.h>
#include<time.h>



// IMU Configuration
#define DEFAULT_BUFFER_SIZE   100
#define DEFAULT_MPU_ADDRESS 0x68
#define PWR_MGMT_1    0x6B
#define ACCEL_XOUT_H 0x47
#define DEFAULT_SPEED 4.4

// Motion Status
#define MOTION 1
#define STATIONARY 0

class positioning
{
  private:
    int buffer_size;
    int MPU_addr; // I2C address of the MPU-6050
    int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
    float scaled_AcX, scaled_AcY, scaled_AcZ, scaled_GyX, scaled_GyY, scaled_GyZ;
    float scalefactor_Ac = 16384;
    float scalefactor_Gy = 131;
    unsigned long dt, t_a_prev, t_a_curr;
    int16_t GyZ_buffer[DEFAULT_BUFFER_SIZE];
    int16_t GyZ_index = 0;

    int16_t offset_yaw = 0;




    // Positioning & Navigation Configuration

    // Positioning & Navigation Configuration
    float x, y; //Current position Coordintate
    float x_g, y_g; //Goal Coordintate
    float yaw; // Orientation: Yaw

    float speed ;  // in cm/s
    long t_p_prev, t_p_curr;

    // Reset buffer
    void buffer_reset(int16_t buffer[], int16_t buffer_size) {
      for (int i = 0; i < buffer_size; ++i) {
        buffer[i] = 0;
      }
    }

    // Insert data into buffer
    void buffer_insert(int16_t buffer[], int16_t buffer_size, int &index, int16_t data) {
      buffer[index] = data;
      index = (index + 1) % buffer_size;
    }

    // Mean of data in the buffer
    float buffer_mean(int16_t buffer[], int16_t buffer_size) {
      long sum = 0;
      for (int i = 0; i < buffer_size; ++i) {
        //    disp(String(buffer[i]));
        //    disp(" ");
        sum += buffer[i];
      }
      return ((float)sum) / buffer_size;
    }


  public:
    void update_speed(float speed = DEFAULT_SPEED)
    {
      this->speed = speed;
    }

    void update_goal(int x_g, int y_g)
    {
      this->x_g = x_g;
      this->y_g = y_g;
    }

    float get_desired_yaw()
    {
      float yaw_d = atan2((y_g - y) , (x_g - x)) * 180 / PI;
      return yaw_d;
    }


    float get_x()
    {
      return this->x;
    }


    float get_y()
    {
      return this->y;
    }

    float get_x_g()
    {
      return this->x_g;
    }


    float get_y_g()
    {
      return this->y_g;
    }

    float get_yaw()
    {
      update_heading();
      //      displn("\nPOSITION :: " + String(this->yaw));
      return this->yaw;
    }

    float distance_to_goal()
    {
      return sqrt(sq(x - x_g) + sq(y - y_g));
    }

    // Initialize IMU
    void initialize_imu(int MPU_addr = DEFAULT_MPU_ADDRESS) {

#ifdef POSITION_UNIT_TEST
      displn("\nPOSITION :: Initialize IMU");
#endif
      this->MPU_addr = MPU_addr;
      Wire.begin();
      Wire.beginTransmission(this->MPU_addr);
      Wire.write(PWR_MGMT_1);  // PWR_MGMT_1 = 0x6B register
      Wire.write(0);     // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
    }

    // Initialise Positioning system

    void initialize_position()
    {
#ifdef POSITION_UNIT_TEST
      displn("\nPOSITION :: Initialize POSITION X,Y,YAW,Xg,Yg");
#endif
      x = y = yaw = 0;
      x_g = y_g = 0;
      t_p_prev = t_p_curr = micros();

    }

    // Initialize Heading
    void initialize_heading(int buffer_size = DEFAULT_BUFFER_SIZE)
    {
#ifdef POSITION_UNIT_TEST
      displn("\nPOSITION :: Initialize heading");
#endif
      this->buffer_size = buffer_size;
      buffer_reset(GyZ_buffer, buffer_size);
      t_a_prev = micros();
      yaw = 0;
    }

    void intitialize_positioning( int MPU_addr = DEFAULT_MPU_ADDRESS , int buffer_size = DEFAULT_BUFFER_SIZE, float speed = DEFAULT_SPEED)
    {
#ifdef POSITION_UNIT_TEST
      displn("\nPOSITION :: Initialize POSITIONING");
#endif
      initialize_imu(MPU_addr);
      initialize_position();
      initialize_heading(buffer_size);
      update_speed(speed);
#ifdef POSITION_UNIT_TEST
      displn("\nPOSITION ::  Initialization Complete");
#endif
    }

    positioning( int MPU_addr = DEFAULT_MPU_ADDRESS , int buffer_size = DEFAULT_BUFFER_SIZE, float speed = DEFAULT_SPEED)
    {
      intitialize_positioning( MPU_addr , buffer_size, speed);
    }

    //Calculate & Update Position

    void calculate_position(int motion_status = STATIONARY)
    {
      t_p_curr = micros();
      long dt = t_p_curr - t_p_prev;
      float dist = (motion_status == MOTION) ? (dt * speed / 1000000) : 0;
      x = x + dist * cos(yaw * PI / 180);
      y = y + dist * sin(yaw * PI / 180);
      t_p_prev = t_p_curr;
    }


    // Calibrate IMU
    void calibrate_yaw()
    {


      long sum = 0;
      offset_yaw = 0;
      int calibration_samples = 5000;
      for (int i = 0; i < calibration_samples; i++)
      {
        Wire.beginTransmission(MPU_addr);
        Wire.write(ACCEL_XOUT_H);  // starting with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_addr, 2, true); // request a total of 14 registers
        GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
        buffer_insert(GyZ_buffer, buffer_size, GyZ_index, GyZ);
        sum += GyZ;
      }
      offset_yaw = sum / calibration_samples;
      displn("\nPOSITION :: Offset: " + String(offset_yaw)) ;
    }


    // Calibrate Modules
    void calibrate()
    {
      initialize_imu();
      buffer_reset(GyZ_buffer, buffer_size);
      calibrate_yaw();
    }

    // Get Heading
    void update_heading() {

      Wire.beginTransmission(MPU_addr);
      Wire.write(ACCEL_XOUT_H);  // starting with register 0x47 (GYRO_ZOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr, 2, true); // request a total of 2 registers
      GyZ = Wire.read() << 8 | Wire.read() ; // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      buffer_insert(GyZ_buffer, buffer_size, GyZ_index, GyZ);
      t_a_curr = micros();
      dt = t_a_curr - t_a_prev;
      t_a_prev = t_a_curr;
      scaled_GyZ = (buffer_mean(GyZ_buffer, buffer_size) - offset_yaw) / scalefactor_Gy;
      yaw = yaw + ( scaled_GyZ * dt / 1000000.0);
      yaw = atan2(sin(yaw * PI / 180), cos(yaw * PI / 180)) * 180 / PI;
    }

    void update_pose(int motion_status = STATIONARY)
    {

      this->update_heading();
      this->calculate_position(motion_status);

#ifdef POSITION_UNIT_TEST
      displn("\nPOSITION :: Update Pose");
      position_telemetry();
#endif
    }


#ifdef POSITION_UNIT_TEST
    void position_telemetry()
    {
      disp("\nPOSITION :: ");
      disp(String(x));
      disp(" ");
      disp(String(y));
      disp(" ");
      disp(String(yaw));
      disp(" ");
      disp("\n");
    }


    void intialize_position_unittest() {
      disp("UNITTEST...CALIBRATING...");
      intitialize_positioning( MPU_addr , buffer_size);
      calibrate();


      delay(1000);

    }

    void position_unittest() {
      update_pose();

      position_telemetry();
    }
#endif
};
#endif
