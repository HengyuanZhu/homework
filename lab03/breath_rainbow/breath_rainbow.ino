// 定义LED引脚
const int ledPin_R= 2;  
const int ledPin_Y= 4;
const int ledPin_G= 16;
// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

void setup() {
  Serial.begin(115200);

  // 【新版用法】直接将引脚、频率和分辨率绑定
  // 它会自动返回一个关联的通道（如果需要的话）
  ledcAttach(ledPin_R, freq, resolution);
  ledcAttach(ledPin_Y, freq, resolution);
  ledcAttach(ledPin_G, freq, resolution);
}

void loop() 
{
  for(int i=0;i<=255;i++)
  {
    ledcWrite(ledPin_R,255-i );
    ledcWrite(ledPin_G,i );
    ledcWrite(ledPin_Y,0);
    delay(10);
  }//实现红变绿
  for(int i=0;i<=255;i++)
  {
    ledcWrite(ledPin_R,0);
    ledcWrite(ledPin_G,255-i);
    ledcWrite(ledPin_Y,i);
    delay(10);
  }//实现绿变黄
  for(int i=0;i<=255;i++)
  {
    ledcWrite(ledPin_R,i);
    ledcWrite(ledPin_G,0);
    ledcWrite(ledPin_Y,255-i);
    delay(10);
  }//实现黄变红
  Serial.println("Breathing cycle completed");
}