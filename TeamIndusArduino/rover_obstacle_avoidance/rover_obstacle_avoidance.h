/*
   Filename: Rover Top Module
   Authors: Shilpa R. , Niladri Dutta, Varshini K, Rahul Rathode
   Description: Code written for autonomous rover during a Team Indus Rover Workshop, June 2018
   Date Created: 26-06-2018
   Version Info:
    0.1 Base Version
*/

// Header Files
#include "display.h"
#define OBSTACLE_AVOIDANCE_UNITTEST // Uncomment to activate Obstacle Avoidance

#define LEFT -1
#define RIGHT 1
#define FORWARD 0
#define STOP 2


float get_weight(int n, int num_rangefinders)
{
  float offset = ((num_rangefinders - 1.0) / 2.0);
  float weight = 0;

  weight = n - offset;

  return weight;
}

int get_obstacle_position(int16_t mask, int num_rangefinders)
{
  float weighted_sum = 0;
  int obstacle_position = 0;

  disp("Wt Sum:");
  for (int i = 0; i < num_rangefinders; ++i)
  {
    weighted_sum += ((mask & (1 << i)) > 0) * get_weight(i, num_rangefinders);
  }
#ifdef OBSTACLE_AVOIDANCE_UNITTEST
  disp(String(weighted_sum));
  disp("\t");
#endif

  if (weighted_sum == 0)
  {
    obstacle_position = (mask > 0) ? RIGHT : FORWARD; // Hardcoded RIGHT obstacle if obstacle at center i.e. LEFT turn
#ifdef OBSTACLE_AVOIDANCE_UNITTEST
    disp("MidMsk:");
    disp(String((mask > 0)));
    disp("\t");
#endif
  }
  else
  {
    obstacle_position = weighted_sum / abs(weighted_sum);
#ifdef OBSTACLE_AVOIDANCE_UNITTEST
    disp("\t");
    disp("\t");
#endif
  }

  return obstacle_position;
}



#ifdef OBSTACLE_AVOIDANCE_UNITTEST
void ultrasonic_unittest()
{
  int num_rangefinders = 3, obstacle_position = 0;
  //  mask = check_threshold(threshold);
  for (int16_t i = 0; i < pow(2, num_rangefinders); ++i)
  {

    disp(String(i));
    disp("\t");
    obstacle_position = get_obstacle_position(i, num_rangefinders);


    disp("ObsPos:");
    disp(String(obstacle_position));
    switch (obstacle_position)
    {
      case LEFT:
        disp("OBS LEFT");
        disp("\t");
        break;

      case RIGHT:
        disp("OBS RIGHT");
        disp("\t");
        break;
        
      case FORWARD:
        disp("No OBS");
        disp("\t");
        break;

      default:
        disp("STOP");
        disp("\t");
        break;
    }

    delay(500);
    disp("\n");
  }
}
#endif

