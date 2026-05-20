const int ledPin_y = 2; 
const int ledPin_r = 4; 

const int freq = 5000;       // 频率 5000Hz
const int resolution = 8;    // 分辨率 8位 (0-255)

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin_r, freq, resolution);
  ledcAttach(ledPin_y, freq, resolution);
}

void loop() {
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++)
  {
    ledcWrite(ledPin_r, dutyCycle);
    ledcWrite(ledPin_y, 255 - dutyCycle); 
    delay(10);
  }
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++)
  {
    ledcWrite(ledPin_y, dutyCycle);
    ledcWrite(ledPin_r, 255 - dutyCycle); 
    delay(10);
  }
}