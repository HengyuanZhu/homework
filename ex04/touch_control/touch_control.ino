#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 800 
bool ledState = false;         
bool lastState = false;//记录每时每刻引脚的状态   
bool currentStableState = false; //记录当前稳定状态
int reminder = 0; //记录状态变化的时刻
const int gap = 50; //判断是否真的是摸了
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState); 
}
void loop() {
  uint16_t touchValue = touchRead(TOUCH_PIN);//读取当前数值
  bool currentReading = (touchValue < THRESHOLD); //低于阈值则为1，高于阈值则为0

  if (currentReading != lastState)
  {
    reminder = millis(); 
  }//只要和上一瞬间状态不同，就开始计时

  if ((millis() - reminder) > gap)//超过50ms就说明不是手抖或者脉冲信号导致的，是一个稳定状态
  {
    
    if (currentReading != currentStableState) //将稳定状态与上一次的稳定状态做对比
    {
      currentStableState = currentReading;
      
      if (currentStableState == true)
      {
        ledState = !ledState; 
        digitalWrite(LED_PIN, ledState);
      }
    }
  }
  lastState = currentReading;//时刻监视瞬间状态
  delay(10); 
}