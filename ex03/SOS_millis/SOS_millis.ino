// 定义LED引脚
const int ledPin = 2; 
const int sos[22] = {200,-200,200,-200,200,-200,-200,500,-600,200,-600,200,-600,-200,500,-200,200,-200,200,-200,-1800};
//变号才反转状态，持续时间为后一个索引所表示的时间
//存储LED亮暗模式时间
unsigned long pre=0;//基准时间
int step=0;//标志进行到哪一步
void setup() 
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  unsigned long cur=millis();//当前时间
  if(step>21)
  {
    step=0;
  }
  else
  {
    if(cur-pre>=abs(sos[step]))
    {
      pre=cur;//更新基准
      if(sos[step]>0)
      {
        digitalWrite(ledPin, HIGH);
      }
      else
      {
        digitalWrite(ledPin, LOW);
      }
      step++;//更新时序
    }

  }
}