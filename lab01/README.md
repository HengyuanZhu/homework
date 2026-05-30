本文件夹为实验一的内容，其中，single_LED为单个LED闪烁实验，three_LED为三个LED闪烁实验，single_LED_delay100为单个LED闪烁实验将delay(1000)更改为delay(100)，single_LED_delay3000为单个LED闪烁实验将delay(1000)更改为delay(3000).
增加了四个实验对应的实验结果视频，命名方式与对应实验原始代码相同，以及Arduino IDE的串口监视器中的波特率设置为和代码中Serial.begin(115200)不同时的现象图。
观察到，当delay(1000)改为delay(3000)时，LED闪烁频率变慢；当delay(1000)改为delay(100)时，LED闪烁频率变快。
当Arduino IDE的串口监视器中的波特率设置为和代码中Serial.begin(115200)不同时，会出现字符乱码现象。其原因是通信错位，ESP32以115200的速率向外发送信息，但电脑串口监视器中却以74880的速率接受信息，导致数据解析错误，从而输出字符乱码。