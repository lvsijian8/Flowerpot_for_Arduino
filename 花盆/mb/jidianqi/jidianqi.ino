void setup() {
  // put your setup code here, to run once:
  pinMode(7,OUTPUT);
 pinMode(3,OUTPUT);
  digitalWrite(7,HIGH);
 digitalWrite(3,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

digitalWrite(7,LOW);
  digitalWrite(3,LOW);
  delay(3000);
  digitalWrite(7,HIGH);
 digitalWrite(3,HIGH);
delay(3000);
  
 /* 
  delay(3000);

  //
  digitalWrite(3,HIGH);
  delay(10000);
 //digitalWrite(7,LOW);
  digitalWrite(3,LOW);
  delay(10000);


 digitalWrite(7,LOW);

  digitalWrite(3,LOW);
  delay(10000);
 //digitalWrite(7,LOW);
  digitalWrite(3,HIGH);
  delay(10000);
  */

}
