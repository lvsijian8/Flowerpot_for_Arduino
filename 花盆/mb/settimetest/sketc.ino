void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  String rece;
  while(Serial.available()>0)
{
  rece+=char(Serial.read());
  delay(4);
}
if(rece[0]>0){
   if(rece[0]=='S'){
      Serial.println(rece);
  }else{
    Serial.println(12);
    Serial.println(rece);
    }
}
}
