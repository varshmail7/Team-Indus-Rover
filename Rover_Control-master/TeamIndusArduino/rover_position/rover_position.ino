/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 12-06-2018
   Version Info:
    0.1 Base Version
*/
#define POSITION_UNIT_TEST // Uncomment for Rover Position Unit Testing using IMU

// Header Files
#include "rover_position.h"


  positioning *ps;

void setup() {
  Serial.begin(115200);
  ps = new positioning;
  ps->intialize_position_unittest();
}

void loop() {
  //  disp("LOOP...");
    ps->position_unittest();
}
