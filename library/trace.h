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
