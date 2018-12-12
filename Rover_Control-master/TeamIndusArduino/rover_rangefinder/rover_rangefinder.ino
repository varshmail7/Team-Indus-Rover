/*
    Filename: Rover Top Module
    Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
    Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
    Date Created: 25-06-2018
    Version Info:
     0.1 Base Version
*/


#include "rover_rangefinder.h"

// Pin Connections
#define TRIG1 5
#define ECHO1 5
#define TRIG2 6
#define ECHO2 6
#define TRIG3 7
#define ECHO3 7

int trig[DEFAULT_NUM_RANGEFINDERS] = {TRIG1, TRIG2, TRIG3};
int echo[DEFAULT_NUM_RANGEFINDERS] = {ECHO1, ECHO2, ECHO3};



rangefinders rf(trig, echo,3);

void setup() {
  Serial.begin(9600);

  rf.initialize_rangefinders(trig, echo,3,100);

}

void loop()
{
  rf.rangefinder_unittest();
}




