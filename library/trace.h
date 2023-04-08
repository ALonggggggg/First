#include <Servo.h>

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4

int leftMotor1 = 16;
int leftMotor2 = 17;
int rightMotor1 = 18;
int rightMotor2 = 19;

int trac1 = 10; //从车头方向的最右边开始排序 
int trac2 = 11; 
int trac3 = 12; 
int trac4 = 13; 

int leftPWM = 5;
int rightPWM = 6;

Servo myServo;  //舵机

int inputPin=7;   // 定义超声波信号接收接口
int outputPin=8;  // 定义超声波信号发出接口

void setup() {
  // put your setup code here, to run once:
  //串口初始化
  Serial.begin(9600); 
  //舵机引脚初始化
  myServo.attach(9);
  //测速引脚初始化
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftPWM, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  //寻迹模块D0引脚初始化
  pinMode(trac1, INPUT);
  pinMode(trac2, INPUT);
  pinMode(trac3, INPUT);
  pinMode(trac4, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 
  tracing();
      
  
}

void setMotorPinState(int pin1, int pin2, int state) {
  digitalWrite(pin1, state);
  digitalWrite(pin2, !state);
}

void motorRun(int cmd, int value) 
{
  analogWrite(leftPWM, value);
  analogWrite(rightPWM, value);
  switch (cmd) 
  {
    case FORWARD:
      Serial.println("FORWARD");
      setMotorPinState(leftMotor1, leftMotor2, HIGH);
      setMotorPinState(rightMotor1, rightMotor2, HIGH);
      break;
    case BACKWARD:
      Serial.println("BACKWARD");
      setMotorPinState(leftMotor1, leftMotor2, LOW);
      setMotorPinState(rightMotor1, rightMotor2, LOW);
      break;
    case TURNLEFT:
      Serial.println("TURN LEFT");
      setMotorPinState(leftMotor1, leftMotor2, HIGH);
      setMotorPinState(rightMotor1, rightMotor2, LOW);
      break;
    case TURNRIGHT:
      Serial.println("TURN RIGHT");
      setMotorPinState(leftMotor1, leftMotor2, LOW);
      setMotorPinState(rightMotor1, rightMotor2, HIGH);
      break;
    default:
      Serial.println("STOP");
      setMotorPinState(leftMotor1, leftMotor2, LOW);
      setMotorPinState(rightMotor1, rightMotor2, LOW);
  }
}





void tracing()
{
  int data[4];
  for(int i=0;i<4;i++)
  {
    data[i]=digitalRead(trac1+i);
  }

   if(data[0]==0&& data[3]==0)  //左右都没有检测到黑线
  {
    motorRun(FORWARD,110);
  }

  if(data[0]==1 || data[1]==1)  //左边检测到黑线
  {
    motorRun(TURNLEFT, 100);
  }
  

  if(data[2]==1 || data[3]==1)  //右边检测到黑线
  {
    motorRun(TURNRIGHT, 100);
  }
 

  if( data[1]==1 && data[2]==1 &&data[3]==1&&data[4]==1 )  //左右都检测到黑线是停止
  {
    motorRun(STOP, 0);
    while(1);
  }

  
  Serial.print(data[0]);
  Serial.print("---");
  Serial.print(data[1]);
  Serial.print("---");
  Serial.print(data[2]);
  Serial.print("---");
  Serial.println(data[3]);
}
