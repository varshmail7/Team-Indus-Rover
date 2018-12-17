/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 24-06-2018
   Version Info:
    0.1 Base Version
*/

#include "rover_motordriver.h"

motordriver m;

void setup() {
  Serial.begin(115200);

  m.initialize_motordriver();

  m.stop();
  delay(1000);


}

void loop() {
  m.motordriver_unittest();
  delay(1000);

}
