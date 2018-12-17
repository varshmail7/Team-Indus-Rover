//#include <SpacebrewYun.h>
#include<Wire.h>
#include<time.h>
//#include<SoftwareSerial.h>
//
//int //bt_Tx=11;
//int //bt_Rx=12;

#define RIGHT -1
#define LEFT 1
#define CW -1
#define CCW 1
#define STOP 0
#define MAX_SPEED 255

int trig1 = A0;
int echo1 = A1;
int trig2 = A2;
int echo2 = A3;
int trig3 = 2;
int echo3 = 3;


int m1 = 8;
int m2 = 9;
int m3 = 7;
int m4 = 6;
int en1 = 10;
int en2 = 11;

int mode = 0;

//SoftwareSerial //bt(//bt_Tx,//bt_Rx);

int x,y;  //Current position
int x_g,y_g; //Goal
int phi; // Orientation
long t_prev,t_curr;
float speed=19*3.14*10/60;

long ti;
int s1,s2,s3;

int leftSpeed = 170;
int rightSpeed = 255;

const int MPU_addr=0x68;  // I2C address of the MPU-6050
 int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 float scaled_AcX,scaled_AcY,scaled_AcZ,scaled_GyX,scaled_GyY,scaled_GyZ;
 float scalefactor_Ac=16384;
 float scalefactor_Gy=131;
 unsigned long t,t1,t2;
// float phi = 0,start_phi=0;
 long sum=0;
 int offset=0;
 int s=200;
 
void setup() {
  Serial.begin(9600);
  //bt.begin(9600);
  pinMode(trig1,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2,INPUT);
  pinMode(trig3,OUTPUT);
  pinMode(echo3,INPUT);
  pinMode(13,OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  t2 = micros();
  phi=0;
  calibrate();
  init_pos();
  delay(1000);
  straight();
}

void init_pos()
{
  x=y=phi=0;
  x_g=y_g=0;
  t_prev=micros();
  t_curr=0;
}

void calc_pos()
{
  t_curr=micros();
  long dt=t_curr-t_prev;
  float dist = dt*speed;
  x = x + dist*cos(phi);
  y = y + dist*sin(phi);
}

void calibrate()
{
  calibrate_imu();
}

void calibrate_imu()
{
  
  sum=0;
  offset=0;
  for(int i=0;i<1000;i++)
    {
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x47);  // starting with register 0x3B (ACCEL_XOUT_H)  
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      sum+=GyZ;
    }
    offset=sum/1000;
    Serial.print("Offset: ");
    Serial.println(offset);
}


int get_dist(int trig,int echo)
{
  int distance,t;
digitalWrite(trig,HIGH);
 delayMicroseconds(10);
 digitalWrite(trig,LOW);
 t=pulseIn(echo,HIGH);
 distance = (t/58);
 
 Serial.print(distance);
 Serial.print(" ");
 //bt.print(distance);
 //bt.print(" ");
 return distance;
}

void set_motor(int id,int dir=STOP,int speed=MAX_SPEED)
{
  int in1 = (id==RIGHT)?m1:m3;
  int in2 = (id==RIGHT)?m2:m4;
  
  switch(dir)
  {
    case CCW:
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      break;
      
    case CW:
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);
      break;

    default:
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
  }
}

void disp(String str)
{
  Serial.print(str);
}

void displn(String str)
{
  Serial.println(str);
}

void left(){
  disp("Left");
  digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
  
}
void right(){
  disp("Right");
  //bt.println("Right");
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
}
void straight(){
  disp("Straight");
  //bt.println("Straight");
   digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
}
void diffright()
{
  disp("DRight");
  digitalWrite(m1,LOW);
  digitalWrite(m2,HIGH);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
}
void diffleft()
{
  Serial.println("DLeft");
   digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,HIGH);
}
void stahp(){
  Serial.println("Stop");
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
}
float get_phi(){
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);  // starting with register 0x3B (ACCEL_XOUT_H)  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  GyZ=Wire.read()<<8|Wire.read()-offset;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  t1 = micros();
  t = t1-t2;
  t2=t1;
  scaled_GyZ=GyZ/scalefactor_Gy;
  phi = phi +( scaled_GyZ*t/1000000.0);
  
  Serial.println(phi);
  //bt.println(phi);
  return phi;
}

void telecommand()
{
  if(Serial.available()>0)
  {
    int cmd=Serial.read();
    Serial.println(cmd);
    switch(cmd)
    {

      case '1':
        mode=1; 
        break;
      case '2':
        x_g=Serial.read()-48;
        y_g=Serial.read()-48;
    Serial.println(x_g);
    Serial.println(y_g);
        
        break;
      default:
        stahp();
    }
  }
}

void go_to_goal()
{
  float phi_d = atan((y_g-y)/(x_g-x));
  float e_phi =  phi_d - phi;
  e_phi = atan2(sin(e_phi),cos(e_phi));

  if(abs(e_phi)>5*3.14/180)
  {
    diff_turn(e_phi);
  }
  else
  {
    straight();
  }
}
void obstacle_avoidance()
{
  float e_phi =  5;
  int s1=0,s2=0,s3=0;
  int obs_threshold = 35;
  while(mode=2)
  {
  s1=(get_dist(trig1,echo1)<obs_threshold)?1:0;
 
  s2=(get_dist(trig2,echo2)<obs_threshold)?1:0;
 
  s3=(get_dist(trig3,echo3)<obs_threshold)?1:0;

    if(s1)
      stahp();
      else{
    if(s2)
    {
    Serial.println("L Obs");
      diff_turn(e_phi);
    }
    else if(s3)
    {
    Serial.println("R Obs");
      diff_turn(-e_phi);
    }
    else
    {
      mode = 1;
    Serial.println("NO Obs");
    }
      }
  }
}

void diff_turn(float phi)
{
  if(phi>0)
  {
  float phi_i = get_phi();
  diffleft();
  while(get_phi()-phi_i<phi);
  stahp();
  }
  else if(phi<0)
  {
  float phi_i = get_phi();
  diffright();
  while(get_phi()-phi_i>phi);
  stahp();
  
  }
}

void loop() {

 telecommand();
 analogWrite(en1,leftSpeed);
 analogWrite(en2,rightSpeed);
// get_phi();

 calc_pos();
 
 
 s=200; 
 s1=get_dist(trig1,echo1);
  if(s1<s && s1>0){
    s=s1;
   }
 
 s2=get_dist(trig2,echo2);

 if(s2<s && s2>0){
  s=s2;
 }
 
 s3=get_dist(trig3,echo3);

 
 if(s3<s && s3>0){
  s=s3;
 }
 Serial.println(s);
 //bt.println(s);
// if(s>15)
//  straight();
if(s<15 )
 {
//  t2 = micros();
//  phi=0;
//  diffleft();
//  while(get_phi()<45);
//  straight();
  mode = 2;
 }
//if(s<10 && s == 10)
// stahp();

        Serial.print("  MODE = ");
        Serial.println(mode);
 switch(mode)
 {
  case 0:
    stahp();
    break;
  case 1:
    go_to_goal();
    break;
  case 2:
    stahp();
    obstacle_avoidance();
    break;
 }
 
}
