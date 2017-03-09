#include <dht.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <Wire.h>

dht DHT;
int DHT11_PIN=3;
/*number*/
int waterOpenPin=4,nutritionPin=5,hydrovalvePin=6;
int soilHumidityPin=A0,photoSensitivePin=A1,waterLevelPin=A2,nutritionOpenPin=A3;
int soilHumidity,waterLevel,photoSensitive,temperature,humidity,nutrition;
String rece;
int mark=0;//control the message recive
/*clock*/
DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
/*time*/
int year, month, date, DoW, hour, minute, second;
int year1,month1,data1,DoW1,hour1,minute1,second1;
int day1[7],day2[7];
int address=0;
byte value=1;
int theidofthis=1;
/*time Open water*/
int controltime=0,numtime=0;


//int turang,water,guangmin,wendu,shidu;

/*onlineControlOpen&&&&&&&&&&&&&&&& waiting for change*/
void onlineControlOpen(){
  if(mark==1){
  if(rece[0]=='A'){//trust that is the true id
   if(rece[1]=='B'){
     digitalWrite(waterOpenPin,HIGH);
     digitalWrite(hydrovalvePin,HIGH);
     delay(5000);
     digitalWrite(hydrovalvePin,LOW);
     digitalWrite(waterOpenPin,LOW);
     
     
   }
   else
   if(rece[1]=='C'){
     digitalWrite(nutritionPin,HIGH);
     digitalWrite(hydrovalvePin,HIGH);
     delay(5000);
     digitalWrite(hydrovalvePin,LOW);
     digitalWrite(nutritionPin,LOW);
   }
  }

  mark=0;
}

}


/*time Open Water  &&&&&&&&&&&&&&&&&&&&&&&&&&&&& waiting for change*/
void timeOpenWater(){
if(mark==1){
  if(rece[0]=='A')//the ID
if(rece[1]=='A'){
  day1[1]=(rece[3]-'0')*10+(rece[4]-'0');
  day1[2]=(rece[6]-'0')*10+(rece[7]-'0');
  day1[3]=(rece[9]-'0')*10+(rece[10]-'0');
controltime=1;
rece="";
mark=0;
}
}

if(controltime==1)
{
int thehour=0;
thehour=Clock.getHour(h12,PM);

if(hour==day1[1]){
numtime++;
if(numtime==day1[0]){
  digitalWrite(waterOpenPin,HIGH);
  digitalWrite(hydrovalvePin,HIGH);
  for(int i=0;i<10;i++)
  delay(1000);
  digitalWrite(waterOpenPin,LOW);
  digitalWrite(hydrovalvePin,LOW);
  numtime=0;
}
}

//day2[3]=Clock.getHour(h12, PM);

}
}


/*delect the controltime&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&7waiting for change*/
void  cancelTheControlTime(){
  if(mark==1){
    if(rece[1]=='D'){
controltime=0;
    }
}
}

/*zidongjiaohua HanShu*/
void queryHumidity(){
soilHumidity=analogRead(soilHumidityPin);
if(soilHumidity>900)
{
  digitalWrite(waterOpenPin,HIGH);
  digitalWrite(hydrovalvePin,HIGH);
  delay(5000);
  digitalWrite(hydrovalvePin,LOW);
  digitalWrite(waterOpenPin,LOW);
}
}

/*jiancehanshu*/
void jiance(){
String thedata="";
int second,minute,hour,date,month,year; 

int soilHumidity1,soilHumidity2,soilHumidity3,soilHumidity4,soilHumidity5;
int soilHumidityLevel=0;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
day2[4]=Clock.getMinute();
if(day2[4]==0&&day2[4]==30)
{
  soilHumidityLevel=(long)analogRead(A0);
  soilHumidity=map(soilHumidityLevel,0,1023,0,100);

   int chk = DHT.read11(3);  
      switch (chk)  
      {  
        case 0:humidity=DHT.humidity;temperature=DHT.temperature; break;  
        case -1:humidity=2;temperature=2;break;  
        case -2:humidity=0;temperature=0;break;  
        default:humidity=1;temperature=1;break;  
      }  

      temperature=analogRead(A1);
      if(temperature<100)
      photoSensitive=1;
      else
      photoSensitive=0;
      waterLevel=(long)analogRead(2);
      int da=(waterLevel/650)*4;
      if(da>5){waterLevel=1;}
      else
      {waterLevel=0;}
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
thedata+=temperature;
thedata+="|";
thedata+=humidity;
thedata+="|";
thedata+=soilHumidity;
thedata+="|";
thedata+=waterLevel;
thedata+="|";
thedata+=photoSensitive;


  Serial.println(thedata);
 // delay(1000);
}
}
void setup() {
  // put your setup code here, to run once:
Wire.begin();
        Clock.setSecond(45);//Set the second 
        Clock.setMinute(29);//Set the minute 
        Clock.setHour(10);  //Set the hour 
        Clock.setDoW(6);    //Set the day of the week
        Clock.setDate(20);  //Set the date of the month
        Clock.setMonth(11);  //Set the month of the year
        Clock.setYear(16);  //Set the year (Last two digits of the year)
	// Start the serial interface
	Serial.begin(9600);
  pinMode(4,OUTPUT);
  EEPROM.write(address,value);
  pinMode(waterOpenPin,OUTPUT);
  pinMode(nutritionPin,OUTPUT);
  pinMode(hydrovalvePin,OUTPUT);
}

int contime=0;
void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0)
{
  rece+=char(Serial.read());
  delay(4);
  mark=1;
  
}
jiance();
queryHumidity();
onlineControlOpen();
cancelTheControlTime();
timeOpenWater();

}

