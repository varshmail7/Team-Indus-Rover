/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 26-06-2018
   Version Info:
    0.1 Base Version
*/

#ifndef CONTROL_LOGIC_H
#define CONTROL_LOGIC_H

#define CONTROL_UNITTEST

//#define OBSTACLE_AVOIDANCE_ACTIVE

// Header Files
#include "rover_motordriver.h"
#include "rover_position.h"
#include "telecommunication.h"
#include "display.h"

#define ANGLE_ERROR_TOLERANCE 3  //degrees


#define WAYPOINT_RADIUS 5 // In cms



void point_turn_angle(int turn_angle, positioning *ps, motordriver &m)
{
  int turn_direction = (turn_angle > 0) ? LEFT_TURN : RIGHT_TURN;
  float yaw_f = ps->get_yaw() + turn_angle;

  disp("\nCONTROL :: Tolerance: " + String(ANGLE_ERROR_TOLERANCE) + " < e_yaw: " + String((yaw_f - ps->get_yaw())*turn_direction) + ", yaw: " + String(ps->get_yaw() ));
  m.point_turn(turn_direction, 150);

  while (ANGLE_ERROR_TOLERANCE  < (yaw_f - ps->get_yaw())*turn_direction ) {
    disp("\nCONTROL :: Tolerance: " + String(ANGLE_ERROR_TOLERANCE) + " < e_yaw: " + String((yaw_f - ps->get_yaw())*turn_direction) + ", yaw: " + String(ps->get_yaw() ));
    ps->update_pose(STATIONARY);
  }
  m.stop();
  disp("\nCONTROL :: Tolerance: " + String(ANGLE_ERROR_TOLERANCE) + " < e_yaw: " + String((yaw_f - ps->get_yaw())*turn_direction) + ", yaw: " + String(ps->get_yaw() ));
}

void align_heading(positioning *ps, motordriver &m)
{
  float yaw = ps->get_yaw();
  float yaw_d = ps->get_desired_yaw();
  float e_yaw =  yaw_d - yaw;
#ifdef CONTROL_UNITTEST
  disp("\nCONTROL :: ");
  disp(" YAW:" + String(yaw));
  disp(", desYAW:" + String(yaw_d));
  disp(", errYAW i:" + String(e_yaw));
#endif
  e_yaw = atan2(sin(e_yaw * PI / 180), cos(e_yaw * PI / 180)) * 180 / PI;
#ifdef CONTROL_UNITTEST
  disp(", errYAW f:" + String(e_yaw));
#endif

  if (abs(e_yaw) > ANGLE_ERROR_TOLERANCE )
  {
    // 1. STOP
    m.stop();
    // 2. POINT TURN :
#ifdef CONTROL_UNITTEST
    disp("\nCONTROL :: PtTurn: " + String(e_yaw) + " >" + String(ANGLE_ERROR_TOLERANCE) + " , ");
#endif
    point_turn_angle(e_yaw, ps, m);
  }
#ifdef CONTROL_UNITTEST
  else
  {
    // CONTINUE FWD Motion
    disp("\nCONTROL :: Aligned");
    disp(", ");
  }
#endif
}



// TODO:UNIT TESTING
void go_to_goal(positioning *ps, motordriver &m)
{

#ifdef CONTROL_UNITTEST
  displn("\nCONTROL :: GO_TO_GOAL " );
#endif
  align_heading(ps, m);
  m.forward();
  ps->update_pose(MOTION);
}

// TODO:UNIT TESTING
float check_waypoint_reached(positioning *ps)
{
  float distance = ps->distance_to_goal();
#ifdef CONTROL_UNITTEST
  disp("\nCONTROL :: Waypoint Distance: " + String(distance) );
#endif

  return (distance < WAYPOINT_RADIUS);
}

#ifdef OBSTACLE_AVOIDANCE_ACTIVE
// TODO:UNIT TESTING
int obstacle_position(rangefinders &rf, positioning *ps)
{
  int mask = 0;
  rf.get_all_rangefinder_distances();
  mask = rf.check_threshold(OBSTACLE_THRESHOLD);
  return get_obstacle_position(mask, rf.get_num_rangefinders());
}

// TODO:UNIT TESTING
int obstacle_avoidance(rangefinders &rf, positioning *ps)
{
  int obs_pos = obstacle_position(rf, ps);
#ifdef CONTROL_UNITTEST
  disp("ObsPos:");
  disp(String(obs_pos));
#endif
  switch (obs_pos)
  {
    case LEFT:
#ifdef CONTROL_UNITTEST
      disp("OBS at LEFT");
      disp("\t");
#endif
      point_turn_angle(RIGHT_TURN, ps);
      break;

    case RIGHT:
#ifdef CONTROL_UNITTEST
      disp("OBS at RIGHT");
      disp("\t");
#endif
      point_turn_angle(LEFT_TURN, ps);
      break;

    case FORWARD:
#ifdef CONTROL_UNITTEST
      disp("No OBS");
      disp("\t");
#endif
      break;

    default:
#ifdef CONTROL_UNITTEST
      disp("STOP");
      disp("\t");
#endif
      break;
  }
  return obs_pos;
}
#endif

// TODO:UNIT TESTING

#ifdef OBSTACLE_AVOIDANCE_ACTIVE
void control_logic(int &mode, telecommunication &tc,  positioning *ps, rangefinders &rf, motordriver &m)
{
  switch (mode)
  {
    case STANDBY: stop();
    case OBSTACLE_AVOIDANCE:
      if (obstacle_avoidance(rf, ps) == 0)
      {
        mode = GO_TO_GOAL;
        tc.update_delta(MODE);
      }
      break;
    case GO_TO_GOAL:
      go_to_goal(ps);
      if (obstacle_position(rf, ps) != 0)
      {
        stop();
        mode = OBSTACLE_AVOIDANCE;
        tc.update_delta(MODE);
      }
      else if (check_waypoint_reached(ps) == 1)
      {
        mode = STANDBY;
        tc.update_delta(MODE);
      }
      break;
  }
}
#endif

void control_logic(int &mode, telecommunication &tc,  positioning *ps, motordriver &m)
{
#ifdef CONTROL_UNITTEST
  displn("\nCONTROL :: MODE: " + String(mode) );
#endif

  switch (mode)
  {
    case STANDBY: m.stop();
      ps->update_pose(STATIONARY);
      break;
    case GO_TO_GOAL:
      //        ps->update_pose(MOTION);
      go_to_goal(ps, m);
      if (check_waypoint_reached(ps) == 1)
      {
        mode = STANDBY;
        ps->update_pose(MOTION);
        m.stop();
        ps->update_pose(STATIONARY);
#ifdef CONTROL_UNITTEST
        displn("\nCONTROL :: WAYPOINT REACHED: " + String(check_waypoint_reached(ps)) );
#endif
        //        if (flag == 0)
        //        {
        //
        //          ps->update_goal(0, 0);
        //          mode = GO_TO_GOAL;
        //          delay(2000);
        //          flag = 1;
        //        }
        tc.update_delta(MODE);
      }
      break;
    default:
      mode = 0;
  }
}


#endif
