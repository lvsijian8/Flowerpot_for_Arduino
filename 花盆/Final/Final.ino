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
static int counter;
String rece;
byte value;
double waterLevel,bottleLevel;
static int controltime=0;
int day1[7]={0,0,0},day2[7],addressTime[6];
int address[6]={0,1,2,3,4,5};
int mark=0,numtime=0,WaterDosage=0,bolleDosage,address1=0;
int year, month, date, DoW, hour, minute, second;
int waterOpenPin=3,nutritionPin=7,hydrovalvePin=6;
int photosensor,humidity,temperature,soilHumidityAll;
int bottleday,bottlehour,waterday,waterhour;
/*----------------------------------jiance------------------------------------*/
void jiance() 
{
  int second,minute,hour,date,month,year;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
if(minute==0||minute==30) 
  {
    String thedata="";
    int soilHumidity[6];
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
    if(da>0) 
    {
      waterLevel=1;
    } else 
    {
      waterLevel=0;
    }
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
/*---------------------------------sjiance-----------------------------------*/
void sjiance(){
  int second,minute;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  if(minute==59 && second==57){
      int soilHumidity[6];
      String thedata="";
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
    waterLevel=(long)analogRead(A2);
    double da=(waterLevel/650)*4;
    if(da>0) 
    {
      waterLevel=1;
    } else 
    {
      waterLevel=0;
    }
      bottleLevel=(long)analogRead(A3);
    double dat=(bottleLevel/650)*4;
    if(dat>0) 
    {
      waterLevel=1;
    } else 
    {
      waterLevel=0;
    }
 
    thedata+=(String)id;
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
    thedata+="|";
    thedata+=bottleLevel;
    Serial.println("updata|"+thedata);
    delay(1000);
  }
}
/*---------------------------------jsjiance-----------------------------------*/
void jsjiance() 
{
  if(rece[0]=='L') 
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
    photosensor=100-map(analogRead(A1),0,1024,0,100);
    waterLevel=(long)analogRead(2);
    double da=(waterLevel/650)*4;
     if(da>0) 
    {
      waterLevel=1;
    } else 
    {
      waterLevel=0;
    }

   thedata+=(String)id;
    thedata+="|";
    thedata+=temperature;
    //air 
    thedata+="|";
    thedata+=humidity;
    //air 湿度
    thedata+="|";
    thedata+="90%";
    thedata+="|";
    thedata+=photosensor;
    Serial.println(thedata);
  }
}
/*------------------------------SetWateringTime--------------------------------*/
/*void SetWateringTime() 
{
  if(rece[0]=='T') 
  {
    //the timeOpenWater ID
    day1[0]=(rece[2]-'0')*10+(rece[3]-'0');
    //hour
    day1[1]=(rece[5]-'0')*10+(rece[6]-'0');
    //second
    day1[2]=(rece[8]-'0')*10+(rece[9]-'0');
    day1[3]=0;
    controltime=1;
    Serial.println(day1[0]);
    Serial.println(day1[1]);
    Serial.println(day1[2]);
  }
}*/
/*--------------------------------WateringTime---------------------**-------------*/
/*void WateringTime() 
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
}*/
void WateringTime(){
     day1[0]=EEPROM.read(address[2]);
     day1[1]=EEPROM.read(address[3]);
       int thehour=0,minute,second;
      second=Clock.getSecond();
      minute=Clock.getMinute();
      thehour=Clock.getHour(h12,PM);
      if(thehour==day1[1]&&minute==0&&second==0) 
      {
        numtime++;
        if(numtime==day1[0]) 
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
void BottleTime(){
    day1[2]=EEPROM.read(address[0]);
  day1[3]=EEPROM.read(address[1]);
 
       int thehour=0,minute,second;
      second=Clock.getSecond();
      minute=Clock.getMinute();
      thehour=Clock.getHour(h12,PM);
      if(thehour==day1[3]&&minute==0&&second==0) 
      {
        numtime++;
        if(numtime==day1[2]) 
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
/*-----------------------------CloseWateringTime-------------------------------*/
void  CloseTheControlTime() 
{
  if(rece[0]=='C') 
  {
    controltime=0;
    //return  controltime;
  }
}
/*----------------------------------settime------------------------------------*/
void setTime() 
{
  String year,month,date,dow,hour,minute,second;
  if(rece[0]=='S') 
  {
    year=(rece[4]-'0')*10+(rece[5]-'0');
    month=(rece[7]-'0')*10+(rece[8]-'0');
    date=(rece[10]-'0')*10+(rece[11]-'0');
    hour=(rece[13]-'0')*10+(rece[14]-'0');
    minute=(rece[16]-'0')*10+(rece[17]-'0');
    second=(rece[19]-'0')*10+(rece[20]-'0');
    Wire.begin();
    Clock.setSecond(atoi(second.c_str()));
    //Set the second 
    Clock.setMinute(atoi(minute.c_str()));
    //Set the minute 
    Clock.setHour(atoi(hour.c_str()));
    //Set the hour 
    // Clock.setDoW(dow);    //Set the day of the week
    Clock.setDate(atoi(date.c_str()));
    //Set the date of the month
    Clock.setMonth(atoi(month.c_str()));
    //Set the month of the year
    Clock.setYear(atoi(year.c_str()));
    bottleday=(rece[22]-'0')*10+(rece[23]-'0');
    bottlehour=(rece[25]-'0')*10+(rece[26]-'0');
    waterday=(rece[32]-'0')*10+(rece[33]-'0');
    waterhour=(rece[35]-'0')*10+(rece[36]-'0');
     EEPROM.write(address[0],bottleday);
     EEPROM.write(address[1],bottlehour);
     EEPROM.write(address[2],waterday);
     EEPROM.write(address[3],waterhour);
  }
}
/*-----------------------------AutomaticWatering--------------------**-----------*/
void AutomaticWatering() 
{
  int numOfWater=50;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  String thedata="";
  soilHumidityAll=analogRead(A0);
  if(soilHumidityAll>900) 
  {
    digitalWrite(waterOpenPin,HIGH);
    digitalWrite(hydrovalvePin,HIGH);
    delay(5000);
    digitalWrite(hydrovalvePin,LOW);
    digitalWrite(waterOpenPin,LOW);
    thedata="a1|20";
    thedata+=year;
    thedata+="-";
    thedata+=month;
    thedata+="-";
    thedata+=date;
    thedata+=" ";
    thedata+=hour;
    thedata+=":";
    thedata+=minute;
    thedata+=":";
    thedata+=second;
    thedata+="|";
    thedata+=numOfWater;
    // Serial.println(thedata);
    thedata="";
    EEPROM.write(addressTime[0],year);
    EEPROM.write(addressTime[1],month);
    EEPROM.write(addressTime[2],date);
    EEPROM.write(addressTime[3],hour);
    EEPROM.write(addressTime[4],minute);
    EEPROM.write(addressTime[5],second);
  }
}

/*-----------------------------OnlineWatering----------------------**---------*/
void OnlineWatering() 
{
  String water;
  if(rece[8]=='W') 
  {
   // water=(rece[4]-'0')*10+(rece[5]-'0');
   // WaterDosage=atoi(water.c_str());
    //gai
    digitalWrite(waterOpenPin,LOW);
    digitalWrite(hydrovalvePin,LOW);
  /*  for (int i=0;i<WaterDosage;i++) 
    {
      delay(1000);
    }*/
     delay(1000);
    digitalWrite(waterOpenPin,HIGH);
    digitalWrite(nutritionPin,HIGH);
  }
}
void OnlineBottling() 
{
  if(rece[18]=='B') 
  {
    String bolle;
   // bolle=(rece[4]-'0')*10+(rece[5]-'0');
   // bolleDosage=atoi(water.c_str());
    //gai
    digitalWrite(nutritionPin,LOW);
    digitalWrite(hydrovalvePin,LOW);
    /*for (int i=0;i<WaterDosage;i++) 
    {
      delay(1000);
    }*/
     delay(1000);
    digitalWrite(hydrovalvePin,HIGH);
    digitalWrite(nutritionPin,HIGH);
  }
}
/*---------------------------------HeadBeat--------------------------------*/
void HeadBeat(){

  Serial.println("headBeat|"+(String)id+"|");
}
/*---------------------------------settime--------------------------------*/
void settime(){
  int second,minute,hour,date,month,year;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  if(hour==0&&minute==0&&second==0){
    Serial.println("setTime");
    delay(1000);
  }
  
}
void setup() 
{
  Wire.begin();
  Clock.setSecond(52);
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
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  MsTimer2::set(3000,HeadBeat);
  MsTimer2::start();
}
void loop() 
{
  sjiance();
  settime();
  
  WateringTime();
  BottleTime();
  // AutomaticWatering();
  
  while(Serial.available()>0) 
  {
    rece+=char(Serial.read());
    delay(4);
    mark=1;
  }
  if(mark==1) 
  {
    setTime();
  //  SetWateringTime();
    jsjiance();
    CloseTheControlTime();
    OnlineWatering();
    delay(2000);
    OnlineBottling();
  }
  mark=0;
  rece="";
}
