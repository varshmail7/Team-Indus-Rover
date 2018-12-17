/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 26-06-2018
   Version Info:
    0.1 Base Version
*/

#include "rover_obstacle_avoidance.h"

// Display Functions
//
//void disp(String str)
//{
//  Serial.print(str);
//}
//
//void displn(String str)
//{
//  Serial.println(str);
//}

void setup() {
  Serial.begin(9600);
}

void loop() {

  ultrasonic_unittest();

}
