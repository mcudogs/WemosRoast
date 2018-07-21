//#include <ESP8266WiFi.h>
#include <ModbusTCPSlave.h>
#include <max6675.h>



////https://forums.adafruit.com/viewtopic.php?f=31&t=61774&sid=06d9885dcf852259d1f568fa96e40265&start=15

////WIFI Settings
///*
//byte ip[]      = { 192, 168, 1, 126};
//byte gateway[] = { 192, 168, 1, 1 };
//byte subnet[]  = { 255, 255, 255, 0 };
//*/
ModbusTCPSlave Mb;

unsigned long timer;
unsigned long checkRSSIMillis;
int thermoDO = 1;
int thermoCS = 2;
int thermoCS1 = 3;
int thermoCLK = 0;
int led = 4;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
MAX6675 thermocouple1(thermoCLK, thermoCS1, thermoDO);
//MAX6675 thermocouple(D0, D2, D1);
//MAX6675 thermocouple1(D0, D3, D1);
void setup()
{

  //Mb.begin("Telecom-28778737", "passwordwificasa47893000", ip, gateway, subnet);
  Mb.begin("AP0079CF", "1462Dogs04");

  delay(1000);
  Mb.MBInputRegister[0] = 100;
  Mb.MBInputRegister[1] = 65500;
  Mb.MBInputRegister[2] = 300;
  Mb.MBInputRegister[3] = 400;
  Mb.MBInputRegister[4] = 500;

  Mb.MBHoldingRegister[0] = 1;
  Mb.MBHoldingRegister[1] = 2;
  Mb.MBHoldingRegister[2] = 3;
  Mb.MBHoldingRegister[3] = 4;
  Mb.MBHoldingRegister[4] = 5;
  pinMode(led, OUTPUT);
  delay(500);
}

void loop()
{
  Mb.Run();
  delay(10);

  if (millis() - timer >= 1000) {
    timer = millis();
    Mb.MBInputRegister[2] = ((uint16_t) thermocouple1.readCelsius()*100);
    Mb.MBInputRegister[1] = ((uint16_t) thermocouple.readCelsius()*100);
  }
  for(int i=1; i<=99; i++) {
    if(i<=Mb.MBHoldingRegister[4])
      digitalWrite(led, HIGH);
      //digitalWrite(D4, HIGH);
    else
      digitalWrite(led, LOW);
      //digitalWrite(D4, LOW);
    delay(5);
   }
}



