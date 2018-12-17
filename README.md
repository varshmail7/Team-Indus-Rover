# Project Title

Control of TeamIndus Rover

## Objective of the Rover

To design and develop an autonomous lunar rover with hazard avoidance and way-point navigation. This rover shall traverse a minimum of 3048 cm through a set of predetermined way-points by avoiding obstacles more than 15 cm in height.

## Objective of the control algorithm
1. Way-point navigation<br/>
2. Obstacle and hazard avoidance using sensors<br/>
The objective is to achieve both points (1) and (2) without accelerometer data.<br/>

## Requirements of the Rover
The rover shall have cognizance of its relative position from the starting point, at all times.<br/>
It must have the following:<br/>
1.Ultrasonic sensor (project specific) for detecting hazards.<br/>
2.Gyroscope for tracking the heading of the robot.<br/>
3.Ability to receive telecommand at the rate of 9 Bps or higher and transmit telemetry at the rate of 37 Bps or higher.<br/>
4.On-Board Computer for data processing.<br/>
5.Motor driver card to control motor speed.<br/>
6.Power supply of 5V for the onboard electronics.<br/>

## Control Modes

![block diagram](https://user-images.githubusercontent.com/25247909/50090665-4cfedb80-022f-11e9-97fb-bbfad6506444.png)
      
      




### Standby Mode:
In this mode, the rover is stationary. All the motors are given BRAKE command. This condition is executed when:<br/>
1.The goal position is reached<br/>
2.The kill switch is opened (due to emergency reasons)<br/>

### Go-to-goal Mode:
In this mode, the Rover shall orient itself toward the goal coordinates (the predetermined way point) and proceed in that direction using the information obtained from the IMU’s accelerometer and gyroscope. Before proceeding towards the waypoint the system will generate a failsafe timeout, using the following formula:<br/>
timeout=(min. rover speed) * (displacement to goal)<br/>
timeout=(min. rover speed) * (dtimeout=(min. rover speed) * (displacement to goal)<br/>


 ### Obstacle Avoidance Mode:
If any of the the Rover’s forward facing range sensors detect an object within a range of 35cm, the Rover will switch to this mode. It will identify the distance between itself and the nearest obstacle and proceed to take a point turn in the opposite direction until none of the range finders detect an obstacle within the 35 cm threshold. Then the Rover will move forward by a fixed distance (80 cm) and switch back to Go-to-Goal mode.

## Mode Switching
The Rover’s mode will switch from Standby to Go-to-goal mode at the beginning of the mission when provided a set of goal coordinates and commanded to START.<br/> 
The mode will switch from Go-to-goal to Obstacle-avoidance mode when it detects an obstacle.<br/>
The mode will switch from Obstacle-Avoidance to Go-to-goal mode when it is clear of the obstacle and Go-to-goal to Standby mode when the mission is complete.<br/>
At any point of time if communication with the ground station fails, the Rover will take a 180 degree point turn and set its last saved position checkpoint with a stable communication as its goal coordinates and switch to Go-To-Goal mode.<br/>
Each waypoint is associated with a timeout. Incase the rover neither encounter any obstacle nor reach the goal within the timeout period, it will switch to Standby mode.<br/>

## Navigation
The goal point is specified in terms of x and y coordinates. The desired heading is calculated from the rover’s current position. The rover aligns itself in the direction of the desired angle. <br/>
The distance between the goal coordinates and the current coordinates are measured to compute distance from the goal. The Rover then moves to cover this distance. If an obstacle is in its path, the rover switches to the obstacle avoidance mode. Finally, when it reaches the goal, it goes to standby mode and stops. <br/>

## Built With
* [Arduino IDE - ](https://www.arduino.cc/en/Guide/Environment)It connects to the Arduino and Genuino hardware to upload programs and communicate with them.


