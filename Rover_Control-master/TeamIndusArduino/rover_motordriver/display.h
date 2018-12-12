#ifndef DISPLAY_H
#define DISPLAY_H

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
