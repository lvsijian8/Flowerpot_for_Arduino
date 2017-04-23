

//#include <dht.h>





void setup() {
 Serial.begin(9600);
 pinMode(A0,INPUT);
 pinMode(8,OUTPUT);
}

void loop() {
  
int soilHumidity[6];
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
  Serial.println(soilHumidityPinjun);
      Serial.println();
delay(1000);
}
