// 定义板载LED引脚，避免使用"魔法数字"
#define LED_PIN_r 2
#define LED_PIN_g 4
#define LED_PIN_y 16

void setup() {
  // 初始化串口通信
  Serial.begin(115200);
  // 初始化板载LED引脚为输出模式
  pinMode(LED_PIN_r, OUTPUT); 
  pinMode(LED_PIN_g, OUTPUT);
  pinMode(LED_PIN_y, OUTPUT);
}

void loop() {
  Serial.println("Hello ESP32!");
  digitalWrite(LED_PIN_r, HIGH);
  digitalWrite(LED_PIN_g, HIGH);
  digitalWrite(LED_PIN_y, HIGH);   // 点亮LED
  delay(1000);                   // 等待1秒
  digitalWrite(LED_PIN_r, LOW); 
  digitalWrite(LED_PIN_g, LOW);
  digitalWrite(LED_PIN_y, LOW);   // 熄灭LED
  delay(1000);              // 等待1秒
}