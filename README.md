# 基于 Protopie 与 Arduino 的智能产品原型平台
## 一、平台简介
以往在制作智能产品原型时往往只能通过 Arduino 实现简单的功能，或通过较为复杂笨重的方式连接 Processing 实现带有 GUI 等的智能产品原型。我在进行智能产品设计的时候探索了一种新的解决方案，用于快速链接GUI设计与智能产品的人机交互，更为快速的实现智能产品高保真原型，及硬件与App联动等等功能。
借助 交互设计常用的 Sketch （ Adobe XD ）——  Protopie 数字原型工作流的高级功能 Android Broadcast 实现与 Arduino 联动。这套方案几乎能够完美代替传统的 Processing —— Arduino 智能产品原型方案，同时实现降低代码工作量和封装设备实现无需连接电脑演示。
![workingdemo](https://github.com/StephenJoker/Protopie-Arduino-Robot-Platform/blob/master/Screenshots/workingdemo.gif)

## 二、原理解释
ProtoPie Studio ( 制作原型部分 ) 中有一个“发送”反应模块，和“接收”触发模块。在这两个动作的 属性中有一个叫“Android Broadcast channel”的属性。利用Android broadcast可以帮助安装在同 一设备中的ProtoPie Player(演示原型部分)和App通讯。 
“接收”可以帮助ProtoPie接收外部信号(非ProtoPie发送的信号)，并指导ProtoPie做出相应反 馈。例如，某设备和安卓连接后，从该设备发出的信号;亦或者从服务器发出的事件(Event)信 号，ProtoPie在接收到这些信号后就会作出既定的交互反应。 
“发送”则是反向操控。即ProtoPie也可以反过来操控外部的硬件或服务器。通过点触原型，使外部 硬件或者服务器作出反馈。例如，有一个在安卓上可以通过API控制的智能灯，使用ProtoPie发送 功能，就可以通过控制原型发送信号，控制该智能灯。除了和硬件通讯，ProtoPie也可以和服务 器上的API通信。 

## 三、底层原理
### 发送反应
在ProtoPie Studio上添加“发送”后，渠道部分选择“Android Broadcast” ，在信息ID栏中输入适当 的通信代码。 
ProtoPie执行“发送”反应时，会向同一安卓设备上的 App 发送 Broadcast。Broadcast intent的 action值是 io.protopie.action.ONE_TIME_RESPONSE ，从 intent 的 extra parameter可以读取 message ID。 
为了收到 ProtoPie 发出的信息，需要生成 BroadcastReceiver 的 subclass ，并将其放在 Manifest 文件中。
代码范例：从ProtoPie接收到的messageId值以Toast的形式展现。 

```
package io.protopie.protopiebridgesample;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;
public class ProtoPieReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if ("io.protopie.action.ONE_TIME_RESPONSE".equals(intent.getAction())) {
            // Message received from ProtoPie.
            String messageId = intent.getStringExtra("messageId");
            String toastText = "Received " + messageId + " from ProtoPie";
            Toast.makeText(context, toastText, Toast.LENGTH_LONG).show();
      } 
   } 
} 
```

这样生成的class必须加入到 AndroidManifest.xml。打开 AndroidManifest.xml 文件, 在application 下面添加以下代码。
```
<receiver android:name=".ProtoPieReceiver">
    <intent-filter>
        <action android:name="io.protopie.action.ONE_TIME_RESPONSE"/>
    </intent-filter>
</receiver> 
```
#### 接收触发
接收触发使ProtoPie Player可以收到同一安卓设备上App发送的broadcast。 
在ProtoPie Studio上添加接收触发，渠道选择Android Broadcast ，之后输入相应的Message ID 。 然后在接收触发下面添加需要的交互反应动作。 下面展示的就是向ProtoPie发送信息的代码示范。 
```
private void sendMessageToProtoPie(Context context, String messageId) {
    Intent intent = new Intent("io.protopie.action.ONE_TIME_TRIGGER");
    intent.putExtra("messageId", messageId);
    context.sendBroadcast(intent);
} 
```

## 四、平台功能
1. 通过 Protopie Studio 在局域网或 USB 连接下实现远程控制 安卓设备屏幕内容、播放声音或视频、串口通讯给Arduino。
2. 通过触摸屏幕、旋转设备等 Protopie Player 支持的功能输入实现串口通讯控制 Arduino。
3. 通过串口通讯将 Arduino端状态、传感器反馈等作为输入传递给 Protopie Player。
4. 

## 五、平台项目样例
这里提供了一个可以水平180度旋转并执行探头缩头的桌面机器人原型样例方案，这套方案可以实现  安卓设备 USB 连接 Arduino ，在安卓设备上呈现绿野仙踪法操控后台GUI，在 Arduino 上执行机器人运动。
![Protopie-Arduino-Robot-Platform/Dashboard.PNG](https://github.com/StephenJoker/Protopie-Arduino-Robot-Platform/blob/master/Screenshots/Dashboard.PNG)

#### Arduino 端代码
```
#include <Servo.h>   // 调用舵机库
Servo turn;  // 创建水平舵机对象来控制伺服
Servo rase;  // 创建升降舵机对象来控制伺服

long randNumber;
long ldl;

// 延迟对象配置
int dl = 1500; // 操作延迟
int wdl = 500; // 启动延迟


// 定义不同电器发射信号的角度信息
int rst = 25; // 水平复位
int cond = 60; // 空调
int lamp = 90; // 台灯
int kurt = 170; // 窗帘
int tv = 100; // 电视
int spk = 140; // 音箱
int hlgt = 80; // 大灯

// Protopie 联动指令
// B 运行生日脚本
// U 执行伸头
// D 执行缩头
// R 执行水平复位
// C 控制空调
// L 控制台灯
// K 控制窗帘
// T 控制电视
// S 控制音箱
// H 控制大灯

void setup() {
  Serial.begin(9600);  // For USB communication with Android
  
  turn.attach(9);  // attaches the servo on pin 9 to the servo object
  rase.attach(10);  // attaches the servo on pin 10 to the servo object
  
  delay(wdl); // 延迟防止运动失效
  turn.write(rst); // 水平复位
  rase.write(45); // 升降复位
  delay(wdl); // 延迟防止运动失效
  turn.detach(); // 断开水平电源 避免电机抖动噪音
  rase.detach(); // 断开升降电源 避免电机抖动噪音
}

void loop() {
  // Read from ProtoPie
  while (Serial.available() > 0) {
    int a = Serial.read();

    // 生日程序
    if (a == 'B') {
      // Turn on birthday
      Serial.println("birthday start"); // Send "birthday done" to ProtoPie
      turn.attach(9);  // attaches the servo on pin 9 to the servo object
      delay(wdl);
      turn.write(90);
      delay(dl);
      turn.write(170);
      delay(dl);
      turn.write(60);
      delay(dl);
      turn.write(100);
      delay(dl);
      turn.write(140);
      delay(dl);
      turn.write(170);
      delay(dl);
      turn.write(80);
      delay(dl);
      turn.write(0);
      delay(dl);
      turn.detach();
      Serial.println("birthday done"); // Send "birthday done" to ProtoPie
    }

    // 探头程序
    if (a == 'U') { // Turn on head
      Serial.println("Rasie UP"); // Send "Rasie UP" to ProtoPie
      rase.attach(10);
      delay(wdl);
      rase.write(135);
      delay(wdl);
      rase.detach(); 
    }

    // 缩头程序
    if (a == 'D') { // Turn off head
      Serial.println("Raise DOWN"); // Send "Raise DOWN" to ProtoPie
      rase.attach(10);
      delay(wdl);
      rase.write(45);
      delay(wdl);
      rase.detach();
    }

    // 水平复位程序
    if (a == 'R') { // Turn Reset
      Serial.println("Turn Reset"); // Send "Turn Reset" to ProtoPie
      turn.attach(9);  // attaches the servo on pin 9 to the servo object
      delay(wdl);
      turn.write(rst);
      delay(dl);
      turn.detach();
    }

    // 空调程序
    if (a == 'C') { // Turn Condition
      Serial.println("Air Condition"); // Send "Air Condition" to ProtoPie
      turn.attach(9);  // attaches the servo on pin 9 to the servo object
      delay(wdl);
      turn.write(cond);
      delay(dl);
      turn.detach();
    }

    
    
    // 台灯程序
    if (a == 'L') { // Turn Lamp
      Serial.println("Bed Lamp"); // Send "Bed Lamp" to ProtoPie
      turn.attach(9);  // attaches the servo on pin 9 to the servo object
      delay(wdl);
      turn.write(lamp);
      delay(dl);
      turn.detach();
    }

    // 窗帘程序
    if (a == 'K') { // Turn curtain
      Serial.println("Curtain"); // Send "Curtain" to ProtoPie
      turn.attach(9);  // attaches the servo on pin 9 to the servo object
      delay(wdl);
      turn.write(kurt);
      delay(dl);
      turn.detach();
    }
    
    // 电视程序
    if (a == 'T') { // Turn TV
      Serial.println("TV"); // Send "TV" to ProtoPie
      turn.attach(9);  // attaches the servo on pin 9 to the servo object
      delay(wdl);
      turn.write(tv);
      delay(dl);
      turn.detach();
    }

    // 音箱程序
    if (a == 'S') { // Turn Speaker
      Serial.println("Speaker"); // Send "Speaker" to ProtoPie
      turn.attach(9);  // attaches the servo on pin 9 to the servo object
      delay(wdl);
      turn.write(spk);
      delay(dl);
      turn.detach();
    }
    
    // 大灯程序
    if (a == 'H') { // Turn Head Light
      Serial.println("Head Light"); // Send "Head Light" to ProtoPie
      turn.attach(9);  // attaches the servo on pin 9 to the servo object
      delay(wdl);
      turn.write(hlgt);
      delay(dl);
      turn.detach();
      
    }
  }
}
```
#### Protopie 端 工程文件
[点击下载]()
##### Protopie 端的基本逻辑
用户点击台灯时，判断台灯对应的 lamp 函数数值是否为 0 （ lamp=0 代表当前设备不在执行操作 ）随后依次发送 升起、旋转到台灯、旋转归位、下降四步骤的的指令给 Arduino。这里如果单纯执行可以直接发送给 Arduino，在我的样例中则是首先定义了四个对应的变量（ up、lamp、reset、down），并重复引用使其可以在实现发送信号的同时完成屏幕上的状态反馈。
#### 建议的拼装结构（2mm亚克力激光切割）
[点击下载拼装结构图纸](https://github.com/StephenJoker/Protopie-Arduino-Robot-Platform/blob/master/PlatformDemoFiles/protopie-arduino-2mm.dxf)
![拼装成品图](https://github.com/StephenJoker/Protopie-Arduino-Robot-Platform/blob/master/Screenshots/look.png)
## 五、平台部署指南
1. 为安卓设备安装 Protopie Player [点击跳转下载地址](http://r.protopie.io/player-android-latest-link/) 
2. 为安卓设备安装 BridgeApp [点击下载](https://github.com/StephenJoker/Protopie-Arduino-Robot-Platform/blob/master/Resource/io.protopie.protopiearduinobridge.apk) 
3. 开启安卓设备的 设置——开发者选项——USB调试
4. 启动 BridgeApp
5. 通过OTG转接头（ type C 及 microUSB ）将安卓设备与 Arduino 连接。
6. 此时 BridgeApp 应显示 USB设备已经连接。
7. 保留 BridgeApp 后台，启动 Protopie Player 运行原型。

## 六、参考文档
 [https://github.com/ProtoPie/android-arduino-bridge-sample](https://github.com/ProtoPie/android-arduino-bridge-sample) 