#include <MsTimer2.h>
 String rece;
void HeadBeat()//中断函数
{
  Serial.println("hello world");
}

void blink()//中断函数
{
    if(rece[0]=='h')
  Serial.println("hello");
} 

void setup()
{
  Serial.begin(9600);
//  attachInterrupt(0, blink, LOW);//当int.0电平改变时,触发中断函数blink
 // pinMode(2,LOW);

  
  MsTimer2::set(3000,HeadBeat);
  MsTimer2::start();
}
 
void loop()
{  
 
while(Serial.available()>0) 
  {
    rece+=char(Serial.read());
    delay(4);
  }

}
 

