/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 26-06-2018
   Version Info:
    0.1 Base Version
*/


// Header Files
#include "telecommunication.h"

telecommunication tc;

void setup() {
  tc.initialize_telecommunication();
}

void loop() {
  tc.telecommunication_unittest();

}
