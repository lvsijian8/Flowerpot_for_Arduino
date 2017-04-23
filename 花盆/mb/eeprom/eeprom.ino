#include <EEPROM.h>


int address=0;
byte value;
void setup()
{
Serial.begin(9600);
value=EEPROM.read(address);
Serial.println(value,DEC);
value=value+1;
EEPROM.write(address,value);
}
void loop()
{
}
