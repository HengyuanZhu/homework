#include <WiFi.h>
#include <WebServer.h>

// --- 硬件引脚定义 ---
const int TOUCH_PIN = 4;   // 触摸引脚 (GPIO 4)

// 创建 Web 服务器对象，监听 80 端口
WebServer server(80);

// --- 生成实时监控网页界面的函数 ---
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实时传感器Web仪表盘</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; margin-top: 50px; background-color: #f0f2f5; color: #333;}
    
    /* 仪表盘核心样式 */
    .dashboard { padding: 40px; margin: 20px auto; width: 350px; background-color: #222; border-radius: 15px; box-shadow: 0 8px 16px rgba(0,0,0,0.2); color: white;}
    .sensor-title { font-size: 20px; color: #aaa; margin-bottom: 20px; }
    .sensor-value { font-size: 80px; font-weight: bold; color: #00FF00; font-family: 'Courier New', Courier, monospace; text-shadow: 0 0 15px #00FF00; }
    
    /* 底部提示文字 */
    .hint { margin-top: 30px; color: #666; font-size: 16px; }
  </style>
  <script>
    // 使用 AJAX (fetch) 每隔 150 毫秒向 ESP32 请求一次最新的传感器数值
    setInterval(function() {
      fetch('/sensor')
        .then(response => response.text())
        .then(val => {
          // 将获取到的数值更新到网页中央的元素中
          document.getElementById('sensorValue').innerText = val;
        })
        .catch(err => console.error(err));
    }, 150); 
  </script>
</head>
<body>
  <h1>物联网数据上报监控面板</h1>
  
  <div class="dashboard">
    <div class="sensor-title">触摸引脚 (GPIO 4) 实时数值</div>
    <div id="sensorValue" class="sensor-value">----</div>
  </div>

  <div class="hint">提示：将手指逐渐靠近引脚，观察上方数值的变化。</div>

</body>
</html>
)rawliteral";
  return html;
}

// --- 路由处理函数 ---

// 1. 访问主页
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 2. 传感器数据查询接口（供网页 AJAX 调用）
void handleSensor() {
  // 读取当前的触摸引脚模拟量
  int currentTouch = touchRead(TOUCH_PIN);
  // 以纯文本形式返回给前端
  server.send(200, "text/plain", String(currentTouch));
}

void setup() {
  Serial.begin(115200);
  delay(500); 

  // --- 开启 AP 热点 ---
  const char* ap_ssid = "ESP32-225"; 
  const char* ap_pass = "12345678"; 

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println("\n--- ESP32 启动成功 ---");
  Serial.println("AP热点已开启");
  Serial.print("请连接 WiFi: ");
  Serial.println(ap_ssid);
  Serial.print("浏览器访问 IP: ");
  Serial.println(WiFi.softAPIP());

  // --- 绑定路由与处理函数 ---
  server.on("/", handleRoot);
  server.on("/sensor", handleSensor); 
  server.begin();
}

void loop() {
  // 持续监听和处理网页请求
  server.handleClient(); 
}