#include <Servo.h>
#include "Wire.h"

//  From Receiver
int ch1, ch2, ch3, ch4, ch6;  //  Ail, Ele, Thr, Rud, Mode


int ch1_R;
int ch2_R;
int ch3_R = 1500; // Rescue Center ch3
int ch4_R = 1500; // Rescue Center ch4

//  To Servo
Servo servo_1;  // Ail
Servo servo_2;  // Eleeator
Servo servo_3;  // Throttle
Servo servo_4;  // Rudder

//  From Gyro
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal=265;
int maxVal=402;

double x; double y; double z;


//  ##########################
//  ####### void Setup #######
//  ##########################

void setup() {

  Serial.begin(9600);

  //  ch s Signals to pins 2,3,4,5,6 as Input 
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  //  servo_s Signals to pins 8,9,10,11 as Output
  servo_1.attach(8);
  servo_2.attach(9);
  servo_3.attach(10);  
  servo_4.attach(11);
  
  //  Gyro
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
}

//  ##########################
//  ####### void Loop ########
//  ##########################

void loop() {
  
  ch1 = pulseIn(2, HIGH);
  ch2 = pulseIn(3, HIGH);
  ch3 = pulseIn(4, HIGH);
  ch4 = pulseIn(5, HIGH);
  ch6 = pulseIn(6, HIGH);
  /*TTT
  Serial.print("ch1 = "); Serial.print(ch1);
  Serial.print("\tch2 = "); Serial.print(ch2);
  Serial.print("\tch3 = "); Serial.print(ch3);
  Serial.print("\tch4 = "); Serial.print(ch4);
  Serial.print("\tch6 = "); Serial.print(ch6);
  Serial.println();
  */
  
// Gyro
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);

  Serial.print("xAng= "); Serial.print(xAng/55);
  Serial.print("\tyAng= "); Serial.print(yAng/55);
  Serial.print("\tzAng= "); Serial.print(zAng/55);
  Serial.println();
  
  //x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  //y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  //z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
  //Serial.print("AngleX= "); Serial.print(x);
  //Serial.print("\t\tAngleY= "); Serial.print(y);
  //Serial.print("\t\tAngleZ= "); Serial.print(z);
  //Serial.println();
  
 if (ch6 < 1500) {
  Serial.println(" ################## Rescue Mode ################## ");
  ch1_R = 1500 + -1*(xAng/55);
  ch2_R = 1500 + (yAng/55);
  servo_1.write(ch1_R);
  servo_2.write(ch2_R);
  servo_3.write(ch3_R);
  servo_4.write(ch4_R);
  
  Serial.print("ch1_R= ");  Serial.print (ch1_R);
  Serial.print("\tch2_R= ");  Serial.print (ch2_R);
  Serial.print("\tch3_R= ");  Serial.print (ch3_R);
  Serial.print("\tch4_R= ");  Serial.print (ch4_R);
  Serial.println();

 }
 else {
  Serial.println(" ################## Normal Mode ################## ");
  servo_1.write(ch1);
  servo_2.write(ch2);
  servo_3.write(ch3);
  servo_4.write(ch4);
  
  Serial.print("ch1_N= ");  Serial.print (ch1);
  Serial.print("\tch2_N= ");  Serial.print (ch2);
  Serial.print("\tch3_N= ");  Serial.print (ch3);
  Serial.print("\tch4_N= ");  Serial.print (ch4);
  Serial.println();
  
 }
  
  Serial.println();
  delay(1000);

}
