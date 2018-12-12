/*
    Filename: Rover Top Module
    Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
    Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
    Date Created: 25-06-2018
    Version Info:
     0.1 Base Version
*/

#ifndef RANGEFINDER_H
#define RANGEFINDER_H

//Header Files
#include "display.h"



//#define RANGEFINDER_UNITTEST

// Range Finder Configuration

#define DEFAULT_MAX_RANGEFINDER_DISTANCE 100 // In cms
#define DEFAULT_NUM_RANGEFINDERS 3 // Number of Rangefinders
#define SPEED_OF_SOUND 34000 // in cm/s

#define OBSTACLE_THRESHOLD 50

class rangefinders
{
  private:
    int pulse_timeout; // Pulse Timeout (in seconds)
    int max_rangefinder_distance;
    int num_rangefinders;
    int * rangefinder_distances;
    int * trig;
    int * echo;

  public:
    void update_max_rangefinder_distance(int max_distance)
    {
      this->max_rangefinder_distance = max_distance;
      this->pulse_timeout = 2 * max_rangefinder_distance / SPEED_OF_SOUND;
    }

    void initialize_rangefinders(int trig[], int echo[], int num_rangefinders = DEFAULT_NUM_RANGEFINDERS, int max_rangefinder_distance = DEFAULT_MAX_RANGEFINDER_DISTANCE)
    {
      this->num_rangefinders = num_rangefinders;
      this->rangefinder_distances = new int[num_rangefinders];
      this->trig = new int[num_rangefinders];
      this->echo = new int[num_rangefinders];
      for (int i = 0; i < num_rangefinders; ++i)
      {
        this->trig[i] = trig[i];
        pinMode(trig[i], OUTPUT);
        this->echo[i] = echo[i];
        pinMode(echo[i], INPUT);
        disp(" i :");
        disp(String(i));
        disp(" T :");
        disp(String(this->trig[i]));
        disp(" E :");
        disp(String(this->echo[i]));
      }

      update_max_rangefinder_distance(max_rangefinder_distance);
    }

    // Constructor
    rangefinders(int trig[], int echo[], int num_rangefinders = DEFAULT_NUM_RANGEFINDERS, int max_rangefinder_distance = DEFAULT_MAX_RANGEFINDER_DISTANCE)
    {
      initialize_rangefinders(trig, echo, num_rangefinders, max_rangefinder_distance);
    }

    ~rangefinders()
    {
      delete [] rangefinder_distances;
      delete [] trig;
      delete [] echo;
    }

    long readUltrasonicDistance(int pin)
    {
      pinMode(pin, OUTPUT);  // Clear the trigger
      digitalWrite(pin, LOW);
      delayMicroseconds(2);
      // Sets the pin on HIGH state for 10 micro seconds
      digitalWrite(pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(pin, LOW);
      pinMode(pin, INPUT);
      // Reads the pin, and returns the sound wave travel time in microseconds
      return pulseIn(pin, HIGH);
    }

    // Get distance from rangefinder sensor

    int get_rangefinder_distance(int Trig, int Echo)
    {
      int distance, t;
      //  digitalWrite(Trig, HIGH);
      //  delayMicroseconds(10);
      //  digitalWrite(Trig, LOW);
      //  t = pulseIn(Echo, HIGH, pulse_timeout);
      //  distance = (t / 58);
      distance = (readUltrasonicDistance(Trig) / 58);

      return distance;
    }

    void get_all_rangefinder_distances()
    {
      for (int i = 0; i < this->num_rangefinders; ++i)
      {
        this->rangefinder_distances[i] = this->get_rangefinder_distance(this->trig[i], this->echo[i]);

#ifdef RANGEFINDER_UNITTEST
        disp(String(this->rangefinder_distances[i]));
        disp("\t");
#endif
      }
    }


    int16_t check_threshold(int threshold)
    {
      int16_t mask = 0;
      for (int i = 0; i < this->num_rangefinders; ++i)
      {
        mask |= (this->rangefinder_distances[i] < threshold) ? (1 << i) : 0;
      }
      return mask;
    }

    int get_num_rangefinders()
    {
      return this->num_rangefinders;
    }


#ifdef RANGEFINDER_UNITTEST
    void rangefinder_unittest() {
      int mask = 0;
      this->get_all_rangefinder_distances();
      mask = this->check_threshold(OBSTACLE_THRESHOLD);
      Serial.print("\tmask: ");
      Serial.print(mask);
      Serial.print("\t");
      if (mask)
      {
        Serial.print("\t");
        for (int i = 0; i < num_rangefinders; ++i)
        {
          Serial.print(":");
          Serial.print(String(i));
          Serial.print("-");
          if (mask & (1 << i )) {
            Serial.print(String(1));
          }
          Serial.print("\t");
        }
      }
      else
      {
        Serial.print("CLEAR");
      }
      Serial.print("\n");
    }
#endif
};


#endif
