void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(A2,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
double s=analogRead(A2);
double d=(s/650)*4;
Serial.println(d);
 delay(1000);

}
