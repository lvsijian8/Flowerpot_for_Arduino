#include <dht.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <Wire.h>

dht DHT;
int DHT11_PIN=3;
/*number*/
int waterOpenPin=4,nutritionPin=7,hydrovalvePin=6;
int soilHumidityPin=A0,photoSensitivePin=A1,waterLevelPin=A2,nutritionOpenPin=A3;
int soilHumidityAll,waterLevel,photoSensitive,temperature,humidity,nutrition;
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
int addressTime[6];
int WaterDosage=0,NutritionDosage=0;

//int turang,water,guangmin,wendu,shidu;

/*onlineControlOpen&&&&&&&&&&&&&&&& waiting for change*/
void onlineControlOpen(){
  
  //trust that is the true id
   if(rece[2]=='A')
   {WaterDosage=20;}
   else
   if(rece[2]=='B')
   {WaterDosage=14;}
   else
   if(rece[2]=='C')
   {WaterDosage=7;
     digitalWrite(waterOpenPin,HIGH);
     digitalWrite(hydrovalvePin,HIGH);
     for(int i=0;i<WaterDosage;i++){
     delay(500);
     }
     digitalWrite(hydrovalvePin,LOW);
     digitalWrite(waterOpenPin,LOW);
   }
   else 
   if(rece[0]=='C'){
     if(rece[2]=='A')
   {NutritionDosage=20;}
   else
   if(rece[2]=='B')
   {NutritionDosage=14;}
   else
   if(rece[2]=='C')
   {NutritionDosage=7;}
     digitalWrite(nutritionPin,HIGH);
     digitalWrite(hydrovalvePin,HIGH);
     for(int i=0;i<NutritionDosage;i++){
     delay(500);
     }
     digitalWrite(hydrovalvePin,LOW);
     digitalWrite(nutritionPin,LOW);
 } 
}


/*time Open Water  &&&&&&&&&&&&&&&&&&&&&&&&&&&&& waiting for change*/
void timeOpenWater(){
if(rece[0]=='A'){//the timeOpenWater ID
  day1[1]=(rece[3]-'0')*10+(rece[4]-'0');//hour
  day1[2]=(rece[6]-'0')*10+(rece[7]-'0');//
  day1[3]=(rece[9]-'0')*10+(rece[10]-'0');
controltime=1;
rece="";
mark=0;
Serial.println(day1[1]);
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
    if(rece[0]=='D'){
controltime=0;
    }
}


/*zidongjiaohua HanShu*/
void queryHumidity(){
  int numOfWater=50;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  String thedata="";
soilHumidityAll=analogRead(soilHumidityPin);
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
//Serial.println(thedata);
  thedata="";
  EEPROM.write(addressTime[0],year);
  EEPROM.write(addressTime[1],month);
  EEPROM.write(addressTime[2],date);
  EEPROM.write(addressTime[3],hour);
  EEPROM.write(addressTime[4],minute);
  EEPROM.write(addressTime[5],second);
}
}

/*----------------------------------------------------------jiancehanshu------------------------------------------------------------*/
void jiance(){
  
String thedata="";
int second,minute,hour,date,month,year; 

int soilHumidity[6];
int soilHumidityLevel=0;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
day2[4]=Clock.getMinute();
if(day2[4]==0||day2[4]==30)
{
  soilHumidity[0]=(long)analogRead(A0);//DeleWrongMessage
  delay(2);
  soilHumidity[1]=(long)analogRead(A0);
  delay(2);
  soilHumidity[2]=(long)analogRead(A0);
  delay(2);
  soilHumidity[3]=(long)analogRead(A0);
  delay(2);
  soilHumidity[4]=(long)analogRead(A0);
  delay(2);
  soilHumidity[5]=(long)analogRead(A0);
  int soilHumidityPinjun,soilcom[6],chazhi;
  soilHumidityPinjun=(soilHumidity[0]+soilHumidity[5]+soilHumidity[4]+soilHumidity[3]+soilHumidity[2]+soilHumidity[1])/6;
  for(int i=0;i<6;i++){
  soilcom[i]=soilHumidityPinjun-soilHumidity[i];
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
thedata+=soilHumidityPinjun;
thedata+="|";
thedata+=waterLevel;
thedata+="|";
thedata+=photoSensitive;


  Serial.println(thedata);
 // delay(1000);
}
}

/*jiancehanshu*/
void jsjiance(){
  //trust that is the true id
   if(rece[0]=='E'){
String thedata="";
int second,minute,hour,date,month,year; 

int soilHumidity[6];
int soilHumidityLevel=0;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
day2[4]=Clock.getMinute();

  soilHumidity[0]=(long)analogRead(A0);//DeleWrongMessage
  delay(2);
  soilHumidity[1]=(long)analogRead(A0);
  delay(2);
  soilHumidity[2]=(long)analogRead(A0);
  delay(2);
  soilHumidity[3]=(long)analogRead(A0);
  delay(2);
  soilHumidity[4]=(long)analogRead(A0);
  delay(2);
  soilHumidity[5]=(long)analogRead(A0);
  int soilHumidityPinjun,soilcom[6],chazhi;
  soilHumidityPinjun=(soilHumidity[0]+soilHumidity[5]+soilHumidity[4]+soilHumidity[3]+soilHumidity[2]+soilHumidity[1])/6;
  for(int i=0;i<6;i++){
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
thedata+=soilHumidityPinjun;
thedata+="|";
thedata+=waterLevel;
thedata+="|";
thedata+=photoSensitive;


  Serial.println(thedata);
 // delay(1000);
   }
}
/*changetime*/
void timeControl(){
  int timeChange[10];
  if(rece[0]=='F'){
second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
timeChange[0]=(rece[2]-'0')*10+(rece[3]-'0');
timeChange[1]=(rece[5]-'0')*10+(rece[6]-'0');
timeChange[2]=(rece[8]-'0')*10+(rece[9]-'0');
timeChange[3]=(rece[11]-'0')*10+(rece[12]-'0');
timeChange[4]=(rece[14]-'0')*10+(rece[15]-'0');
timeChange[5]=(rece[17]-'0')*10+(rece[18]-'0');
if(timeChange[0]==year&&timeChange[1]==month&&timeChange[2]==date){
if(timeChange[3]==hour&&timeChange[4]==minute&&timeChange[5]==second){
}
else{
Wire.begin();
        Clock.setSecond(timeChange[5]);//Set the second 
        Clock.setMinute(timeChange[4]);//Set the minute 
        Clock.setHour(timeChange[3]);  //Set the hour 
            //Set the day of the week
        Clock.setDate(timeChange[2]);  //Set the date of the month
        Clock.setMonth(timeChange[1]);  //Set the month of the year
        Clock.setYear(timeChange[0]);}
}
else{
Wire.begin();
        Clock.setSecond(timeChange[5]);//Set the second 
        Clock.setMinute(timeChange[4]);//Set the minute 
        Clock.setHour(timeChange[3]);  //Set the hour 
            //Set the day of the week
        Clock.setDate(timeChange[2]);  //Set the date of the month
        Clock.setMonth(timeChange[1]);  //Set the month of the year
        Clock.setYear(timeChange[0]);}
}}

void setup() {
  // put your setup code here, to run once:
Wire.begin();
        Clock.setSecond(30);//Set the second 
        Clock.setMinute(20);//Set the minute 
        Clock.setHour(22);  //Set the hour 
        Clock.setDoW(4);    //Set the day of the week
        Clock.setDate(12);  //Set the date of the month
        Clock.setMonth(01);  //Set the month of the year
        Clock.setYear(17);  //Set the year (Last two digits of the year)
	// Start the serial interface
	Serial.begin(9600);
  pinMode(4,OUTPUT);
  EEPROM.write(address,value);
  pinMode(waterOpenPin,OUTPUT);
  pinMode(nutritionPin,OUTPUT);
  pinMode(hydrovalvePin,OUTPUT);
  Serial.println("a1|2017-1-12 21:29:37|33|0|1|0|1");
}


int contime=0;
void loop() {
  // put your main code here, to run repeatedly:
  jiance();
  while(Serial.available()>0)
{
  rece+=char(Serial.read());
  delay(4);
  mark=1;
  
}
if(mark==1){

onlineControlOpen();//id:B&C
cancelTheControlTime();//D
timeOpenWater();//A
jiance();//no id
jsjiance();//E
queryHumidity();
mark=0;
}}

