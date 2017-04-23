


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int  a= 100-map(analogRead(A1),0,1024,0,100);
Serial.println(a);
}
