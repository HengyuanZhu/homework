#include <WiFi.h>
#include <WebServer.h>

// --- 硬件引脚定义 ---
const int LED_PIN_y = 2;   
const int LED_PIN_r = 16;  
const int TOUCH_PIN = 4;   // 触摸引脚 (GPIO 4)

// 触摸阈值
const int TOUCH_THRESHOLD = 800; 

// --- 全局系统状态变量 ---
bool isArmed = false;     
bool isAlarming = false;  

// 创建 Web 服务器对象，监听 80 端口
WebServer server(80);

// 用于 LED 非阻塞闪烁的时间控制变量
unsigned long previousMillis = 0;
const long blinkInterval = 100; // 狂闪的间隔，100毫秒
bool ledState = false;

// --- 生成安防控制网页界面的函数 ---
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实验5：安防报警器</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    .status-box { padding: 20px; margin: 20px auto; border: 1px solid #ccc; width: 300px; font-size: 20px; background-color: #f9f9f9; border-radius: 8px;}
    .btn { padding: 15px 30px; font-size: 18px; margin: 10px; cursor: pointer; color: white; border: none; border-radius: 5px;}
    .btn-arm { background-color: #4CAF50; }
    .btn-disarm { background-color: #f44336; }
  </style>
  <script>
    // 每隔 1 秒请求一次状态，仅更新文字提示
    setInterval(function() {
      fetch('/status')
        .then(response => response.text())
        .then(state => {
          let statusBox = document.getElementById('statusText');
          if (state == "0") {
            statusBox.innerHTML = "<span style='color: gray;'>未布防 (系统待机)</span>";
          } else if (state == "1") {
            statusBox.innerHTML = "<span style='color: green;'>已布防 (安全巡逻中)</span>";
          } else if (state == "2") {
            statusBox.innerHTML = "<span style='color: red; font-weight: bold;'>!!! 警报触发 !!!</span>";
          }
        });
    }, 1000);
  </script>
</head>
<body>
  <h1>物联网安防主机面板</h1>
  
  <div class="status-box">
    系统状态：<br><br>
    <div id="statusText"><span style='color: gray;'>正在获取状态...</span></div>
  </div>

  <button class="btn btn-arm" onclick="fetch('/arm')">布防 (Arm)</button>
  <button class="btn btn-disarm" onclick="fetch('/disarm')">撤防 (Disarm)</button>

</body>
</html>
)rawliteral";
  return html;
}

// 访问主页
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 状态查询接口
void handleStatus() {
  String currentState;
  if (!isArmed) {
    currentState = "0"; // 0 代表未布防
  } else if (isArmed && !isAlarming) {
    currentState = "1"; // 1 代表已布防，安全
  } else if (isAlarming) {
    currentState = "2"; // 2 代表正在报警
  }
  server.send(200, "text/plain", currentState);
}

// 处理点击“布防”按钮
void handleArm() {
  isArmed = true;
  isAlarming = false; 
  Serial.println("已收到网页指令：系统已布防！");
  server.send(200, "text/plain", "OK"); 
}

// 处理点击“撤防”按钮
void handleDisarm() {
  isArmed = false;
  isAlarming = false; 
  Serial.println("已收到网页指令：系统已撤防！");
  
  digitalWrite(LED_PIN_y, LOW);
  digitalWrite(LED_PIN_r, LOW);
  
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  delay(500); 
  
  pinMode(LED_PIN_y, OUTPUT);
  pinMode(LED_PIN_r, OUTPUT);
  
  digitalWrite(LED_PIN_y, LOW);
  digitalWrite(LED_PIN_r, LOW);

  const char* ap_ssid = "ESP32-225";
  const char* ap_pass = "12345678"; 

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println("AP已开启");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/status", handleStatus); 
  server.on("/arm", handleArm);       
  server.on("/disarm", handleDisarm); 
  server.begin();
}

void loop() {
  // 处理网页请求
  server.handleClient(); 

  // 读取触摸引脚
  int touchValue = touchRead(TOUCH_PIN);
  
  // 报警逻辑判断
  if (isArmed && !isAlarming) {
    if (touchValue < TOUCH_THRESHOLD) {
      isAlarming = true; // 锁定报警状态
      Serial.println("警告：检测到入侵！触发警报！");
    }
  }

  // 执行报警动作
  if (isAlarming) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      ledState = !ledState; 
      
      digitalWrite(LED_PIN_y, ledState);
      digitalWrite(LED_PIN_r, ledState);
    }
  } else if (!isArmed) {
    digitalWrite(LED_PIN_y, LOW);
    digitalWrite(LED_PIN_r, LOW);
  }
}