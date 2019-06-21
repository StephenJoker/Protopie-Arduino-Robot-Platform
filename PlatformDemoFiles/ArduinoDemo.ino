#include <Servo.h>   // 调用舵机库
Servo turn;  // 创建水平舵机对象来控制伺服
Servo rase;  // 创建升降舵机对象来控制伺服

long randNumber;
long ldl;

// 延迟对象配置
int dl = 1500; // 操作延迟
int wdl = 500; // 启动延迟


// 角度信息
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
  turn.detach(); // 断开水平电源
  rase.detach(); // 断开升降电源
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
