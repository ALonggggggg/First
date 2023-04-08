
#define STOP        0
#define FORWARD     1
#define BACKWARD    2
#define TURNLEFT    3
#define TURNRIGHT   4
#define CHANGESPEED 5

int leftMotor1 = 16;
int leftMotor2 = 17;
int rightMotor1 = 18;
int rightMotor2 = 19;
bool speedLevel=0;

int leftPWM = 5;
int rightPWM = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  attachInterrupt(0,RightCount_CallBack, FALLING);
  attachInterrupt(1,LeftCount_CallBack, FALLING);

  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftPWM, OUTPUT);
  pinMode(rightPWM, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  SpeedDetection();
 
  if(Serial.available()>0)
  {
    char cmd = Serial.read();
    
    Serial.print(cmd);
    motorRun(cmd);
    if(speedLevel)  //根据不通的档位输出不同速度
    {
      analogWrite(leftPWM, 120);
      analogWrite(rightPWM, 120);
    }
    else
    {
      analogWrite(leftPWM, 250);
      analogWrite(rightPWM, 250);
    }
  }  
}

unsigned long time = 0; // 记录当前时间
unsigned long old_time = 0; // 记录上一次测速的时间节点
unsigned long leftCounter = 0; // 左编码器脉冲计数
unsigned long rightCounter = 0; // 右编码器脉冲计数

bool SpeedDetection()//速度计算
{
  time = millis(); // 以毫秒为单位，计算当前时间
  if (abs(time - old_time) >= 1000) // 如果计时时间已达1秒
  {
    detachInterrupt(0); // 关闭外部中断0
    detachInterrupt(1); // 关闭外部中断1

    // 把每一秒钟编码器码盘计得的脉冲数，换算为当前转速值
    // 转速单位是每分钟多少转，即r/min。这个编码器码盘为20个空洞。
    Serial.print("left:");
    float lv = (float)leftCounter * 60 / 20; // 小车车轮电机转速
    float rv = (float)rightCounter * 60 / 20; // 小车车轮电机转速
  }
  
}


void setMotorPinState(int pin1, int pin2, int state) //引脚设置
{
  digitalWrite(pin1, state);
  digitalWrite(pin2, !state);
}

void motorRun(int cmd, int value) //运动控制
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