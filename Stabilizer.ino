
// Gy
#include<Wire.h>
const int MPU=0x68; 
//int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t GyX,GyY,GyZ;


// Rx
int ch1; // Here's where we'll keep our channel values
int ch2;
int ch7;

// Tx
#include<Servo.h>
Servo ail;
Servo ele;
int txAil;
int txEle;
int max_X;
int min_X;

int GyX180;
int fMode;

void setup() {
  
  // Gy
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);

  // Rx
  pinMode(6, INPUT); // Set our input pins as such
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  //Serial.begin(9600); // Pour a bowl of Serial

  // Tx
  ail.attach(9);
  //ele.attach(10);

}

void loop() {

  // Rx
  ch1 = pulseIn(6, HIGH, 25000); // Read the pulse width of each channel
  //ch2 = pulseIn(7, HIGH, 25000); // Read the pulse width of each channel
  ch7 = pulseIn(8, HIGH, 25000);  // Read pulse of channel 7 for Mode
                                  // UP=Normal - Center=Gyro - Down=Recovery
  //Serial.print("Channel 1:"); // Print the value of 
  //Serial.println(ch1);        // each channel
  //Serial.print("Channel 2:"); // Print the value of 
  //Serial.println(ch2);        // each channel

  // Gy
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);
  
  //GyX=Wire.read()<<8|Wire.read();  
  //GyY=Wire.read()<<8|Wire.read();  
  //GyZ=Wire.read()<<8|Wire.read();  

  //GyX180 = (GyX/135);
  //Serial.print(" \t X:"); Serial.println(GyX180);
  //Serial.print(" \t Y:"); Serial.print(GyY/10);
  //Serial.print(" \t Z:"); Serial.println(GyZ/10);
  Serial.print("\t ch7 = "); Serial.print(ch7);

  // Flight Mode  - Condition from ch7
  if (ch7 < 1300){
    fMode = 0;
  }
  else if (1300 < ch7 < 1700){
    fMode = 1;
  }
  else{
    fMode = 2;
  }
  Serial.print("\t Flight Mode =\t");Serial.println(fMode);
  
  switch (fMode) {
  case 0:
    txAil = ch1;
    break;
  case 1:
    txAil = gyro2();
    break;
  case 2:
    //txAil = gyro1();
    break;
}
  
  
  // Tx
  //txAil = (GyX/10);
  //txEle = (GyY/10);

  //txAil = (ch1);
  //txEle = (ch2);
  // Ail Signal from 935 to 2075
  //txAil = map(ch1, 935, 2075, 0, 160);

  //txAil = map((ch1/10), 93, 207, 0, 160);
  //Serial.println(txAil);
  //ail.write(txAil); // set servo to mid-point
  //ele.write(txEle);

  //Gyro Signal X from -21292 to 27104 = 
  /*
  min_X = min(min_X, GyX);
  Serial.print(" \t Min_X:"); Serial.print(min_X);
  
  max_X = max(max_X, GyX);
  Serial.print(" \t Min_X:"); Serial.print(max_X);

  Serial.println(" ");
  */

  delay(50); // I put this here just to make the terminal 
              // window happier
}

/*
gyro1(){
  //
}
*/
int gyro2(){
  while(GyX != 0){
    if(GyX > 0){
      txAil = 1200;
    }
    else if(GyX < 0){
      txAil = 1750;
    }
    else{
      break;
    }
  }
  return 0;
}
