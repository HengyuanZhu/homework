// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin_R= 2; 
const int ledPin_Y= 4; 

void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  // 将LED引脚设置为输出模式
  pinMode(ledPin_R, OUTPUT);
  pinMode(ledPin_Y, OUTPUT);
}

void loop() {
  digitalWrite(ledPin_R, HIGH);
  digitalWrite(ledPin_Y, LOW);   // 红亮黄灭
  Serial.println("LED_R ON");
  Serial.println("LED_Y OFF");    // 串口输出提示
  delay(1000);                 // 保持1秒
  
  digitalWrite(ledPin_Y, HIGH);
  digitalWrite(ledPin_R, LOW);  // 红灭黄亮
  Serial.println("LED_R OFF");
  Serial.println("LED_Y ON");    // 串口输出提示
  delay(1000);                 // 保持1秒
}
