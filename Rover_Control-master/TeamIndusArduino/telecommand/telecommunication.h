/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 26-06-2018
   Version Info:
    0.1 Base Version
*/
#ifndef TELECOMMUNICATION_H
#define TELECOMMUNICATION_H

//Header Files
#include "display.h"


#define TELECOMMUNICATION_UNITTEST

// Commands
#define NO_COMMAND -1
#define STOP 0
#define START 1
#define GOAL_COORDINATES 2

// Heartbeat Configuration
#define HEARTBEAT 99
#define NO_HEARTBEAT 0

// Telemetry Data Configuration
#define NUM_TELEMETRY_DATA 11

#define MODE 0
#define X_G 1
#define Y_G 2
#define X 3
#define Y 4
#define AO_X 5
#define AO_Y 6
#define AO_Z 7
#define RANGE_1 8
#define RANGE_2 9
#define RANGE_3 10

class telecommunication
{
  private:
    int delta_data = 0xFF;
  public:
    void initialize_telecommunication(int baud = 9600)
    {
      Serial.begin(baud);
      delta_data = 0xFF;
    }
    // Constructor
    telecommunication()
    {
      initialize_telecommunication();
    }

    // Read Command
    int read_command()
    {
      int command = NO_COMMAND;
      if (Serial.available() > 0)
      {
        command = Serial.parseInt();
#ifdef TELECOMMUNICATION_UNITTEST
        disp("RX:");
        disp(String(command));
        disp(", ");
#endif
      }
      return command;
    }
    // Read Goal Coordinates
    void read_goal_coordinates(float &x, float &y)
    {
      x = Serial.parseFloat();
      y = Serial.parseFloat();

#ifdef TELECOMMUNICATION_UNITTEST
      displn(String(x));
      displn(String(y));
#endif

    }
    //
    void update_delta(int data_pos)
    {
      delta_data |= (1 << data_pos);
    }

    void transmit_data(int data)
    {
      //Serial.write(data);

#ifdef TELECOMMUNICATION_UNITTEST
      disp(" TX:");
      disp(String(data));
      disp(", ");
#endif
    }
    void transmit_data(float data)
    {
      //Serial.write((int)data);

#ifdef TELECOMMUNICATION_UNITTEST
      disp(" TX:");
      disp(String(data));
      disp(", ");
#endif
    }

    int telecommand(float &x_g, float &y_g)
    {
      int command = read_command();
      if (command != NO_COMMAND)
      {
        switch (command)
        {
          case STOP:

#ifdef TELECOMMUNICATION_UNITTEST
            disp(" Stop Command ");
#endif
            update_delta(MODE);
            break;
          case START:

#ifdef TELECOMMUNICATION_UNITTEST
            disp(" Start Command ");
#endif
            update_delta(MODE);
            break;
          case GOAL_COORDINATES:
            read_goal_coordinates(x_g, y_g);
            update_delta(X_G);
            update_delta(Y_G);
            break;
          default:

#ifdef TELECOMMUNICATION_UNITTEST
            disp(" Stop Command ");
#endif
            update_delta(MODE);
        }
      }
      return command;
    }


    void telemetry(int mode, float x, float y, float x_g, float y_g, float yaw, float pitch, float roll, float rangefinder_distance_1, float rangefinder_distance_2, float rangefinder_distance_3)
    {
      transmit_data(this->delta_data);
      for (int index = 0; index < NUM_TELEMETRY_DATA; ++index)
      {
        if ((this->delta_data & (1 << index)) > 0)
        {
          switch (index)
          {
            case MODE     : transmit_data(mode); break;
            case X_G      : transmit_data(x_g); break;
            case Y_G      : transmit_data(y_g); break;
            case X        : transmit_data(x); break;
            case Y        : transmit_data(y); break;
            case AO_X     : transmit_data(roll); break;
            case AO_Y     : transmit_data(pitch); break;
            case AO_Z     : transmit_data(yaw); break;
            case RANGE_1  : transmit_data(rangefinder_distance_1); break;
            case RANGE_2  : transmit_data(rangefinder_distance_2); break;
            case RANGE_3  : transmit_data(rangefinder_distance_3); break;
          }
        }
      }
      delta_data = 0;
    }

    int heartbeat()
    {
      transmit_data(HEARTBEAT);
      //delay(1);
      if (read_command() != NO_COMMAND)
      {
#ifdef TELECOMMUNICATION_UNITTEST
        disp(String(HEARTBEAT));
#endif
        return HEARTBEAT;
      }
#ifdef TELECOMMUNICATION_UNITTEST
      disp(String(NO_HEARTBEAT));
#endif
      return NO_HEARTBEAT;
    }



#ifdef TELECOMMUNICATION_UNITTEST
    void telecommunication_unittest()
    {
      int mode;
      float yaw, x, y, x_g, y_g;
      float pitch,  roll, rangefinder_distance_1, rangefinder_distance_2, rangefinder_distance_3;

      telecommand(x_g,y_g);
      telemetry(mode, x, y, x_g, y_g, yaw, pitch, roll, rangefinder_distance_1, rangefinder_distance_2, rangefinder_distance_3);
      //heartbeat();
      displn(" ");
    }
#endif

};


#endif
