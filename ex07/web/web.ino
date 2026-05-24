#include <WiFi.h>
#include <WebServer.h>

const int LED_PIN_y = 2; 
const int LED_PIN_g = 4; 
const int LED_PIN_r = 16; 
const int freq=5000;
const int resolution=8;
WebServer server(80);
int currentPWM = 0; // 增加一个全局变量来记录当前的占空比 (0-255)

String makePage() {
  // 使用 HTML 表单配合 JavaScript 实现滑动实时显示和自动提交
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实验3</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>第三部分：滑动调节 LED 亮度</h1>
  <p>当前占空比：<b id="pwmValue">)rawliteral" + String(currentPWM) + R"rawliteral(</b></p>
  
  <form action="/set" method="GET" id="pwmForm">
    <label for="pwm">滑动调节占空比 (0 - 255):</label><br><br>
    <!-- 
      oninput: 拖动过程中实时更新显示的数字 
      onchange: 松开滑块后自动提交表单
    -->
    <input type="range" id="pwm" name="pwm" min="0" max="255" value=")rawliteral" + String(currentPWM) + R"rawliteral(" 
           style="padding: 5px; width: 250px;" 
           oninput="document.getElementById('pwmValue').innerText = this.value"
           onchange="document.getElementById('pwmForm').submit()">
  </form>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 处理表单提交的函数
void handleSet() {
  // 检查 URL 中是否带有参数 "pwm" (例如: /set?pwm=128)
  if (server.hasArg("pwm")) {
    String pwmStr = server.arg("pwm");
    Serial.println(server.arg("pwm"));
    currentPWM = pwmStr.toInt(); // 将字符串转换为整数
    
    // 安全起见，限制数值范围在 0 到 255 之间
    if (currentPWM < 0) currentPWM = 0;
    if (currentPWM > 255) currentPWM = 255;
    
    // 使用 analogWrite 输出 PWM 信号调节亮度
    ledcWrite(LED_PIN_y, currentPWM); 
    ledcWrite(LED_PIN_g, currentPWM);
    ledcWrite(LED_PIN_r, currentPWM);
  }
  
  // 处理完成后，重定向回主页 ("/") 以刷新页面状态
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN_y, OUTPUT);
  ledcAttach(LED_PIN_y, freq, resolution);
  pinMode(LED_PIN_g, OUTPUT);
  ledcAttach(LED_PIN_g, freq, resolution);
  pinMode(LED_PIN_r, OUTPUT);
  ledcAttach(LED_PIN_r, freq, resolution);
  
  const char* ap_ssid = "ESP32-225";
  const char* ap_pass = "12345678"; // 至少8位

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println("AP已开启");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP()); // 通常 192.168.4.1

  server.on("/", handleRoot);
  server.on("/set", handleSet); // 注册 /set 路由，指向 handleSet 函数
  server.begin();
}

void loop() {
  server.handleClient();
}