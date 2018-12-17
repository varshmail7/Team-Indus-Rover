/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 26-06-2018
   Version Info:
    0.1 Base Version
*/


// Header Files
#include "rover_motordriver.h"
#include "rover_obstacle_avoidance.h"
#include "rover_position.h"
#include "rover_rangefinder.h"
#include "telecommunication.h"
#include "display.h"
#include "control_logic.h"


#ifdef OBSTACLE_AVOIDANCE_ACTIVE
#include "rover_obstacle_avoidance.h"
#include "rover_rangefinder.h"
// Ultrasonic Rangefinders Pin Connections
#define TRIG1 5
#define ECHO1 5
#define TRIG2 6
#define ECHO2 6
#define TRIG3 7
#define ECHO3 7

int trig[DEFAULT_NUM_RANGEFINDERS] = {TRIG1, TRIG2, TRIG3};
int echo[DEFAULT_NUM_RANGEFINDERS] = {ECHO1, ECHO2, ECHO3};

rangefinders rf(trig, echo, 3, 100);
#endif

telecommunication tc;
positioning *ps;
motordriver m;



int mode;

int flag=0;
float x_g = 0, y_g = 0,speed=0;
float pitch = 0,  roll = 0, rangefinder_distance_1 = 0, rangefinder_distance_2 = 0, rangefinder_distance_3 = 0;

void setup() {
    tc.initialize_telecommunication(115200);
  Serial.begin(115200);
  ps = new positioning;
  ps->intialize_position_unittest();
  m.initialize_motordriver();

//  ps->update_goal(10,34);
//  mode = GO_TO_GOAL;
  //  Serial.println(ps->get_desired_yaw());
}
void loop() {
  // Telecommunication System
      if(tc.telecommand(x_g, y_g, mode,speed)!=STOP)
      {
        ps->update_goal(x_g,y_g);
        ps->update_speed(speed);
      }
    //
      tc.telemetry(mode, ps->get_x(), ps->get_y(), ps->get_x_g(),ps->get_y_g(), ps->get_yaw(), pitch, roll, rangefinder_distance_1, rangefinder_distance_2, rangefinder_distance_3);
  //  tc.heartbeat();

  // Positioning System

  // Control System
    control_logic(mode,tc,ps,m);


    
  //  displn("\nTOP :: TURN START");
//    point_turn_angle(90, ps);
//    stop();
  //    align_heading(ps);
  //      go_to_goal(ps);
  //
  //  displn("\nTOP ::TURN COMPLETE");
//  delay(10000);

    displn("");

}
