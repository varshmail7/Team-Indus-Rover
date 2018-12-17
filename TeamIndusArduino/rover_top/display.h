#ifndef DISPLAY_H
#define DISPLAY_H

//Control Mode Parameters
#define STANDBY 0
#define OBSTACLE_AVOIDANCE 1
#define GO_TO_GOAL 2

// Display Functions

void disp(String str)
{
  Serial.print(str);
}

void displn(String str)
{
  Serial.println(str);
}
#endif
