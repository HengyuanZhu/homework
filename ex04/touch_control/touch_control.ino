#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 800 


bool ledState = false; //决定亮灭         
bool lastTouchState = false;   
bool currentStableState = false; 
unsigned long lastDebounceTime = 0; //记录触摸持续时间
const unsigned long debounceDelay = 50; //判断是否真的是摸了

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState); 
}

void loop() {
  uint16_t touchValue = touchRead(TOUCH_PIN);//读取当前数值
  bool currentReading = (touchValue < THRESHOLD); //低于阈值则为1，高于阈值则为0

  if (currentReading != lastTouchState)
  {
    lastDebounceTime = millis(); 
  }//只要和上一瞬间状态不同，就开始计时

  if ((millis() - lastDebounceTime) > debounceDelay)//超过50ms就说明不是手抖导致的，是一个稳定状态
  {
    
    if (currentReading != currentStableState) 
    {
      currentStableState = currentReading; //稳定状态只有通过防抖检测以及状态变化才能改变
      
      if (currentStableState == true)
      {
        ledState = !ledState; 
        digitalWrite(LED_PIN, ledState);
      }
    }
  }
  lastTouchState = currentReading;//时刻监视瞬间状态
  delay(10); 
}