// 定义LED引脚
const int ledPin = 2;  

// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

void setup() {
  Serial.begin(115200);

  // 【新版用法】直接将引脚、频率和分辨率绑定
  // 它会自动返回一个关联的通道（如果需要的话）
  ledcAttach(ledPin, freq, resolution);
}

void loop() 
{
  
  for(float alpha=0; alpha<= PI; alpha=alpha+0.01)
  {   
    
    ledcWrite(ledPin, sin(alpha)*sin(alpha)*255);   
    delay(10);
  }

  Serial.println("Breathing cycle completed");
}