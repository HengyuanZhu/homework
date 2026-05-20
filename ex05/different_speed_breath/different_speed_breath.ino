#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 800
bool lastState = false;        // 上一次的瞬时状态
bool currentStableState = false;    // 当前稳定的触摸状态
int reminder= 0; // 记录状态变化时刻
const int gap = 50; // 判断是否真的摸了

int speed = 1; // 速度档位：1(慢速), 2(中速), 3(快速)
const int freq = 5000;       
const int resolution = 8;    
int dutyCycle = 0;           
int Step = 1;            // 占空比每次变化的步长
int check = 0; // 记录上一次亮度更新的时间
void setup() 
{
  Serial.begin(115200);
  ledcAttach(LED_PIN, freq, resolution);
}

void loop() {
  uint16_t touchValue = touchRead(TOUCH_PIN);
  bool currentReading = (touchValue < THRESHOLD); // 低于阈值为1，否则为0
  if (currentReading != lastState) 
  {
    reminder = millis();
  } // 只要和上一瞬间状态不同，就开始计时
  if ((millis() - reminder) > gap)  // 超过50ms说明不是手抖，是一个稳定状态
  {
    if (currentReading != currentStableState) 
    {
      currentStableState = currentReading; // 如果稳定状态发生了改变
      if (currentStableState == true) //将稳定状态与上一次的稳定状态做对比
      {
        speed++;
        if (speed > 3)
        {
          speed = 1;
        }
      }
    }
  }
  lastState = currentReading;  // 时刻监视瞬时状态

  
 
  int lasting;
  if (speed == 1) 
  {
    lasting = 15;
  } 
  if (speed == 2) 
  {
    lasting = 7;
  } 
  if (speed == 3)
  {
    lasting = 2; 
  } // 根据当前的档位变量，改变每次更新亮度的间隔时长 (相当于改变 delay)
  if (millis() - check >= lasting)  // 使用 millis() 检查是否到达了该更新亮度的时间
  {
    check = millis(); // 更新时间节点
    dutyCycle=dutyCycle+Step; // 按照步长改变亮度
    if (dutyCycle <= 0 || dutyCycle >= 255)
    {
      if (dutyCycle <= 0) dutyCycle = 0;
      if (dutyCycle >= 255) dutyCycle = 255; // 防止越界
      Step = -Step; // 第一次默认从暗变亮，故超出阈值后先变负
    }
    ledcWrite(LED_PIN, dutyCycle);// 将计算好的占空比写入LED
  }
}