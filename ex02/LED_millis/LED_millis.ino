// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2; 
unsigned long pre=0;
int sta=LOW;
const unsigned long half_T=500;//半个周期变化一次
void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  // 将LED引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  unsigned long cur=millis();//获取当前时间
  if(cur-pre>=half_T)
  {
    pre=pre+half_T;
    sta=!sta;//翻转高低电平
    digitalWrite(ledPin, sta);//输出
  }
}