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
  //超声波控制引脚初始化
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  avoidance();
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

const int SERVO_MIDDLE = 90; // 舵机中间位置
const int SERVO_LEFT = 150; // 舵机左转位置
const int SERVO_RIGHT = 30; // 舵机右转位置
const int DISTANCE_THRESHOLD = 30; // 距离阈值
const int MAX_DISTANCE = 50; // 最大测距距离

void adjustServo(int from, int to, int step)
{
  for (int pos = from; pos <= to; pos += step) 
  {
    myServo.write(pos);              
    delay(15);                       
  }
}

void avoidance()
{
  int pos;
  int dis[3]; // 距离
  motorRun(FORWARD, 120);
  myServo.write(SERVO_MIDDLE);
  dis[1] = getDistance(); // 中间


  if (dis[1] < DISTANCE_THRESHOLD)
  {
    motorRun(BACKWARD, 110);
    delay(500);
    motorRun(STOP,0);
    adjustServo(SERVO_MIDDLE, SERVO_LEFT, 1);
    dis[2] = getDistance(); // 左边
    adjustServo(SERVO_LEFT, SERVO_RIGHT, -1);
    dis[0] = getDistance(); // 右边
    adjustServo(SERVO_RIGHT, SERVO_MIDDLE, 1);
    if (dis[0] < dis[2])
    {
      // 左转
      motorRun(TURNLEFT, 250);
      delay(500);
    }
    else
    {
      // 右转
      motorRun(TURNRIGHT, 250);
      delay(500);
    }
  }
}

int getDistance()
{
  digitalWrite(outputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);
  int distance = pulseIn(inputPin, HIGH);
  distance = distance /58;
  Serial.println(distance);

if (distance >=50)
  {
    //如果距离小于50厘米返回数据
    return 50;
  }//如果距离小于50厘米小灯熄灭
  else
    return distance;
}