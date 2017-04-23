

int i=0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

String rece="";


while(Serial.available()>0)
{
  rece+=char(Serial.read());
  delay(5);
  }
  //strlen(str) == 0
if(rece!=NULL){
  Serial.println(rece);
}


  
  /*String  a="hello";
  char h[20]={'h','e','l','l','o','w','o','r','l','d'};
     Serial.println(h[i]);
     // Serial.print(a);
       Serial.println(a);
     i++;
     if(i==10)
       i=0;*/
  
       rece=""; 
       delay(2000);
}
