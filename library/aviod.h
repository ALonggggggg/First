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
void motorRun(int cmd,int value)
{
  analogWrite(leftPWM, value);  //设置PWM输出，即设置速度
  analogWrite(rightPWM, value);
  switch(cmd){
    case FORWARD:
      Serial.println("FORWARD"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
     case BACKWARD:
      Serial.println("BACKWARD"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
     case TURNLEFT:
      Serial.println("TURN  LEFT"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
     case TURNRIGHT:
      Serial.println("TURN  RIGHT"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
     default:
      Serial.println("STOP"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}
void avoidance()
{
  int pos;
  int dis[3];//距离
  motorRun(FORWARD,200);
  myServo.write(90);
  dis[1]=getDistance(); //中间
  
  if(dis[1]<30)
  {
    motorRun(STOP,0);
    for (pos = 90; pos <= 150; pos += 1) 
    {
      myServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    dis[2]=getDistance(); //左边
    for (pos = 150; pos >= 30; pos -= 1) 
    {
      myServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      if(pos==90)
        dis[1]=getDistance(); //中间
    }
    dis[0]=getDistance();  //右边
    for (pos = 30; pos <= 90; pos += 1) 
    {
      myServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    if(dis[0]<dis[2]) //右边距离障碍的距离比左边近
    {
      //左转
      motorRun(TURNLEFT,250);
      delay(500);
    }
    else  //右边距离障碍的距离比左边远
    {
      //右转
      motorRun(TURNRIGHT,250);
      delay(500);
    } 
  }
}
int getDistance()
{
  digitalWrite(outputPin, LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW); // 保持发出超声波信号接口低电平
  int distance = pulseIn(inputPin, HIGH); // 读出脉冲时间
  distance= distance/58; // 将脉冲时间转化为距离（单位：厘米）
  Serial.println(distance); //输出距离值
 
  if (distance >=50)
  {
    //如果距离小于50厘米返回数据
    return 50;
  }//如果距离小于50厘米小灯熄灭
  else
    return distance;
}
