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
    motorRun(STOP, 0);
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
  int distance = pulseIn(inputPin, HIGH) / 58; // 除以58计算距离
  distance = min(distance, MAX_DISTANCE); // 距离不超过最大测距距离
  Serial.println(distance);
  return distance;
}

