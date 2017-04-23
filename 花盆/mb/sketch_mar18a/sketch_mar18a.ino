void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   pinMode(3,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(analogRead(3));
Serial.println();
delay(1000);
}
