/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 24-06-2018
   Version Info:
    0.1 Base Version
*/
#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "display.h"
#define MOTORDRIVER_UNITTEST

// Motor Parameters
#define RIGHT_MOTOR -1
#define LEFT_MOTOR 1
#define CW -1
#define CCW 1
#define STOP 0

#define RIGHT_TURN -1
#define LEFT_TURN 1

#define DEFAULT_MIN_SPEED 0
#define DEFAULT_MAX_SPEED 255

// Motor Configuration
// Pins
// RIGHT Motor
#define M1  11//8//9//8
#define M2  10//9//8//9
#define EN1 6//5//10

// LEFT Motor
#define M3  9//10//7//12
#define M4  8//11//6 //A0
#define EN2 5//6//11

class motordriver
{
  private:
    // Motor Speed Control
    int m1, m2, m3, m4, en1, en2;
    int min_speed;
    int max_speed;

  public:

    // Initialize Motor Driver
    void initialize_motordriver(int m1 = M1, int m2 = M2, int m3 = M3, int m4 = M4, int en1 = EN1, int en2 = EN2, int min_speed = DEFAULT_MIN_SPEED, int max_speed = DEFAULT_MAX_SPEED) {
      this->m1 = m1;
      this->m2 = m2;
      this->m3 = m3;
      this->m4 = m4;
      this->en1 = en1;
      this->en2 = en2;

      this->min_speed = min_speed;
      this->max_speed = max_speed;

      initialize_pins();

    }

    void initialize_pins()
    {
      pinMode(m1, OUTPUT);
      pinMode(m2, OUTPUT);
      pinMode(m3, OUTPUT);
      pinMode(m4, OUTPUT);
      pinMode(en1, OUTPUT);
      pinMode(en2, OUTPUT);
    }


    motordriver(int m1 = M1, int m2 = M2, int m3 = M3, int m4 = M4, int en1 = EN1, int en2 = EN2, int min_speed = DEFAULT_MIN_SPEED, int max_speed = DEFAULT_MAX_SPEED)
    {
      initialize_motordriver(m1, m2, m3, m4, en1, en2, min_speed, max_speed);
    }
    // Set Motor direction and speed
    void set_motor(int motor_id, int dir = STOP, int speed = DEFAULT_MAX_SPEED)
    {
      int in1 = (motor_id == RIGHT_MOTOR) ? m1 : m3;
      int in2 = (motor_id == RIGHT_MOTOR) ? m2 : m4;
      int en = (motor_id == RIGHT_MOTOR) ? en1 : en2;

      speed = (speed < min_speed) ? min_speed : speed;
      speed = (speed > max_speed) ? max_speed : speed;
      analogWrite(en, speed);

      switch (dir)
      {
        case CCW:
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          break;

        case CW:
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          break;

        default:
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
      }
    }

    // Basic Motion Command Functions: Forward, Reverse, Left, Right, Stop
    void point_turn(int dir, int speed = DEFAULT_MAX_SPEED) {
      switch (dir)
      {
        case LEFT_TURN:
          set_motor(LEFT_MOTOR, CW , speed);
          set_motor(RIGHT_MOTOR, CW , speed);
          displn("\nMOTOR :: LEFT ");
          break;
        case RIGHT_TURN:
          set_motor(LEFT_MOTOR, CCW , speed);
          set_motor(RIGHT_MOTOR, CCW , speed);
          displn("\nMOTOR :: RIGHT ");
          break;
      }
    }

    void forward(int speed = DEFAULT_MAX_SPEED) {
      set_motor(LEFT_MOTOR, CCW , speed);
      set_motor(RIGHT_MOTOR, CW , speed);
      displn("\nMOTOR FWD ");
    }

    void reverse(int speed = DEFAULT_MAX_SPEED) {
      set_motor(LEFT_MOTOR, CW , speed);
      set_motor(RIGHT_MOTOR, CCW , speed);
      displn("\nMOTOR :: REV ");
    }

    void stop() {
      set_motor(LEFT_MOTOR, STOP);
      set_motor(RIGHT_MOTOR, STOP);
      displn("\nMOTOR :: STOP ");
    }
#ifdef MOTORDRIVER_UNITTEST
    void motordriver_unittest()
    {
      int pause = 1000;

      forward();
      delay(pause);
      point_turn(LEFT_TURN);
      delay(pause);
      point_turn(RIGHT_TURN);
      delay(pause);
      reverse();
      delay(pause);
      stop();

      //  for(int i=0;i<255;i+=20)
      //  {
      //    set_motor(LEFT_MOTOR,CW,i);
      //    set_motor(RIGHT_MOTOR,CCW,i);
      //    displn(String(i));
      //    delay(1000);
      //  }
      //  set_motor(LEFT_MOTOR, CW);
    }
#endif
};
#endif
