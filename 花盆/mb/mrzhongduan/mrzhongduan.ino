#include <MsTimer2.h>               //定时器库的 头文件
 
void flash()                        //中断处理函数，改变灯的状态
{                        
  static boolean output = HIGH;
  digitalWrite(13, output);
  output = !output;
}
 
void setup() 
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
 
  MsTimer2::set(500, flash);        // 中断设置函数，每 500ms 进入一次中断
  MsTimer2::start();                //开始计时
}
 
void loop()
{
Serial.println("hello");
}

