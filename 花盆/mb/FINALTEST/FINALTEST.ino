#include <MsTimer2.h>
#include <EEPROM.h>
#include <dht.h>
#include <DS3231.h>
#include <Wire.h>
#include<stdlib.h>

int id=1;
dht DHT;
DS3231 Clock;
bool h12;
bool PM;
bool Century=false;
String rece;
byte value;
double waterLevel;
static int controltime=0;
int day1[7]={0,0,0},day2[7],addressTime[6];
int mark=0,numtime=0,WaterDosage=0,address=0;
int year, month, date, DoW, hour, minute, second;
int waterOpenPin=3,nutritionPin=7,hydrovalvePin=6;
int photosensor,humidity,temperature,soilHumidityAll;

void jsjiance() 
{
  if(rece[0]=='J') 
  {
    String thedata="";
    int soilHumidity[6];
    int second,minute,hour,date,month,year;
    second=Clock.getSecond();
    minute=Clock.getMinute();
    hour=Clock.getHour(h12, PM);
    date=Clock.getDate();
    month=Clock.getMonth(Century);
    year=Clock.getYear();
    soilHumidity[0]=(long)analogRead(A0);
    delay(3);
    soilHumidity[1]=(long)analogRead(A0);
    delay(3);
    soilHumidity[2]=(long)analogRead(A0);
    delay(3);
    soilHumidity[3]=(long)analogRead(A0);
    delay(3);
    soilHumidity[4]=(long)analogRead(A0);
    delay(3);
    soilHumidity[5]=(long)analogRead(A0);
    int soilHumidityPinjun,soilcom[6],chazhi;
    soilHumidityPinjun=(soilHumidity[0]+soilHumidity[5]+soilHumidity[4]+soilHumidity[3]+soilHumidity[2]+soilHumidity[1])/6;
    for (int i=0;i<6;i++) 
    {
      soilcom[i]=soilHumidityPinjun-soilcom[i];
      if(soilcom[i]<0)
                   soilcom[i]=-soilcom[i];
      if(i==0)
                   chazhi=i;
      if(soilcom[i]>soilcom[chazhi])
                   chazhi=i;
    }
    soilHumidityPinjun=(soilHumidityPinjun*6-soilHumidity[chazhi])/5;
    soilHumidityPinjun=map(soilHumidityPinjun,0,1023,0,100);
    soilHumidityPinjun=100-soilHumidityPinjun;
    int chk = DHT.read11(4);
    switch (chk) 
    {
      case 0:humidity=DHT.humidity;
      temperature=DHT.temperature;
      break;
      case -1:
            case -2:
            default:break;
    }
    photosensor=analogRead(A1);
    waterLevel=(long)analogRead(2);
    double da=(waterLevel/650)*4;
    /*  if(da>0) 
    {
      waterLevel=1;
    } else 
    {
      waterLevel=0;
    }
    */
    thedata+=year;
    thedata+="/";
    thedata+=month;
    thedata+="/";
    thedata+=date;
    thedata+="/";
    thedata+=hour;
    thedata+=":";
    thedata+=minute;
    thedata+=":";
    thedata+=second;
    thedata+="|";
    thedata+=temperature;
    //air 
    thedata+="|";
    thedata+=humidity;
    //air 湿度
    thedata+="|";
    thedata+=soilHumidityPinjun;
    thedata+="|";
    thedata+=waterLevel;
    thedata+="|";
    thedata+=photosensor;
    Serial.println("20"+thedata);
  }
}

void SetWateringTime() 
{
  if(rece[0]=='W') 
  {
    //the timeOpenWater ID
    day1[0]=(rece[2]-'0')*10+(rece[3]-'0');
    //hour
    day1[1]=(rece[5]-'0')*10+(rece[6]-'0');
    //second
    day1[2]=(rece[8]-'0')*10+(rece[9]-'0');
    day1[3]=0;
    controltime=1;
  }
}
/*--------------------------------WateringTime---------------------**-------------*/
void WateringTime() 
{
  if(controltime==1) 
  {
    day1[4]=03;
    day1[5]=10;
    day1[6]=00;
    if(day1[0]>0&&day1[1]>=0&&day1[2]>=0) 
    {
      int thehour=0,minute,second;
      second=Clock.getSecond();
      minute=Clock.getMinute();
      thehour=Clock.getHour(h12,PM);
      if(thehour==day1[1]&&minute==day1[2]&&second==day1[3]) 
      {
        numtime++;
        if(numtime==day1[0]) 
        {
          digitalWrite(waterOpenPin,LOW);
          digitalWrite(hydrovalvePin,LOW);
                    delay(100000);
          digitalWrite(waterOpenPin,HIGH);
          digitalWrite(hydrovalvePin,HIGH);
          numtime=0;
        }
      }
    } else 
    {
      int thehour=0,minute,second;
      second=Clock.getSecond();
      minute=Clock.getMinute();
      thehour=Clock.getHour(h12,PM);
      if(thehour==day1[5]&&minute==day1[6]&&second==day1[3]) 
      {
        numtime++;
        if(numtime==day1[4]) 
        {
          digitalWrite(waterOpenPin,LOW);
          digitalWrite(hydrovalvePin,LOW);
                    delay(10000);
          digitalWrite(waterOpenPin,HIGH);
          digitalWrite(hydrovalvePin,HIGH);
          numtime=0;
        }
      }
    }
  }
}

void setup() 
{
  Wire.begin();
  Clock.setSecond(45);
  Clock.setMinute(59);
  Clock.setHour(11);
  Clock.setDoW(2);
  Clock.setDate(28);
  Clock.setMonth(3);
  Clock.setYear(17);
  Serial.begin(9600);
  pinMode(7,OUTPUT);
 pinMode(3,OUTPUT);
  digitalWrite(7,HIGH);
 digitalWrite(3,HIGH);

}
void loop() 
{
 // jiance();
  WateringTime();
  // AutomaticWatering();
  
  while(Serial.available()>0) 
  {
    rece+=char(Serial.read());
    delay(4);
    mark=1;
  }
  if(mark==1) 
  {
    jsjiance();
     SetWateringTime();
  }
  mark=0;
  rece="";
}
