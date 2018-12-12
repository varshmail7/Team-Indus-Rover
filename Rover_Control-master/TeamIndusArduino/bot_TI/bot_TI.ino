//#include <SpacebrewYun.h>
#include<Wire.h>
#include<time.h>
//#include<SoftwareSerial.h>
//
//int BT_Tx=11;
//int BT_Rx=12;

//SoftwareSerial bt(BT_Tx,BT_Rx);

int trig1=A0;
int echo1=A1;
int trig2 = A2;
int echo2 = A3;
int trig3 = 2;
int echo3 = 3;

 long ti,s1,s2,s3;

int m1 = 8;
int m2 = 9;
int m3 = 7;
int m4 = 6;
int en1 = 11;
int en2 = 10;
int leftSpeed = 100;
int rightSpeed = 255;
const int MPU_addr=0x68;  // I2C address of the MPU-6050
 int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 float scaled_AcX,scaled_AcY,scaled_AcZ,scaled_GyX,scaled_GyY,scaled_GyZ;
 float scalefactor_Ac=16384;
 float scalefactor_Gy=131;
 unsigned long t,t1,t2;
 float ang = 0,start_ang=0;
 long sum=0;
 int offset=0;
 int s=200;
void setup() {
  Serial.begin(9600);
//  bt.begin(9600);
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
  ang=0;
//  calibrate();
//  delay(1000);
  straight();
}

void calibrate()
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
// bt.print(distance);
// bt.print(" ");
 return distance;
}

void left(){
  Serial.println("Left");
//  bt.println("Left");
  digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
}
void right(){
  Serial.println("Right");
//  bt.println("Right");
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
}
void straight(){
  Serial.println("Straight");
//  bt.println("Straight");
   digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
}
void diffright()
{
  Serial.println("DRight");
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
float get_ang(){
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);  // starting with register 0x3B (ACCEL_XOUT_H)  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  GyZ=Wire.read()<<8|Wire.read()-offset;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  t1 = micros();
  t = t1-t2;
  t2=t1;
  scaled_GyZ=GyZ/scalefactor_Gy;
  ang = ang +( scaled_GyZ*t/1000000.0);
  
//  Serial.println(ang);
//  bt.println(ang);
  return ang;
}

void loop() {

 analogWrite(en2,leftSpeed);
 analogWrite(en1,rightSpeed);
// s=200;
// s1=get_dist(trig1,echo1);
//  if(s1<s && s1>0){
//    s=s1;
//   }
// 
// s2=get_dist(trig2,echo2);
//
// if(s2<s && s2>0){
//  s=s2;
// }
// 
// s3=get_dist(trig3,echo3);
//
// 
// if(s3<s && s3>0){
//  s=s3;
// }
// Serial.println(s);
// bt.println(s);
// if(s>35)
//  straight();
//if(s<35 && s >10)
// {
//  t2 = micros();
//  ang=0;
//  diffleft();
//  while(get_ang()<45);
//  straight();
// }
//if(s<10 && s == 10)
// stahp();
 
}
