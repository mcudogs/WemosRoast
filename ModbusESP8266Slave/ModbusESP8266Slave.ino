#include <ModbusTCPSlave.h>
#include <max6675.h>

//#define MBDebug = true;

////https://forums.adafruit.com/viewtopic.php?f=31&t=61774&sid=06d9885dcf852259d1f568fa96e40265&start=15

////WIFI Settings
///*
byte ip[]      = { 192, 168, 1, 210};
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[]  = { 255, 255, 255, 0 };
//*/
ModbusTCPSlave Mb;

unsigned long timer;
unsigned long checkRSSIMillis;
int thermoDO = D1;
int thermoCS = D2;
int thermoCS1 = D5;
int thermoCLK = D3;
int led = D6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
MAX6675 thermocouple1(thermoCLK, thermoCS1, thermoDO);
void setup()
{
  Serial.begin(9600);
  while (!Serial) {
 //; // wait for serial port to connect.
 }
  Mb.begin("AP0079CF", "1462Dogs04");
  //Mb.begin("AP0079CF", "1462Dogs04", ip, gateway, subnet);
  delay(1000);
  Mb.MBInputRegister[0] = 0;
  Mb.MBInputRegister[1] = 0;
  Mb.MBInputRegister[2] = 0;
  Mb.MBInputRegister[3] = 0;
  Mb.MBInputRegister[4] = 0;

  Mb.MBHoldingRegister[0] = 100;
  Mb.MBHoldingRegister[1] = 200;
  Mb.MBHoldingRegister[2] = 300;
  Mb.MBHoldingRegister[3] = 400;
  Mb.MBHoldingRegister[4] = 0;
  pinMode(led, OUTPUT);
  pinMode(thermoCLK, OUTPUT);
  pinMode(thermoCS1, OUTPUT);
  pinMode(thermoCS, OUTPUT);
  pinMode(thermoDO, INPUT);
  delay(500);
}

void loop()
{
  Mb.Run();
  delay(10);

  if (millis() - timer >= 1000) {
    timer = millis();
    Mb.MBHoldingRegister[1] = ((uint16_t) thermocouple1.readCelsius()*100);
    Mb.MBHoldingRegister[0] = ((uint16_t) thermocouple.readCelsius()*100);
    //Mb.MBHoldingRegister[0] = ((uint16_t) 20000); 
    //Mb.MBHoldingRegister[1] = ((uint16_t) 21100);
    //Serial.print("RSSI = ");
    Serial.println(WiFi.localIP());
    //Serial.println("Regs = ");
    //Serial.println(Mb.MBHoldingRegister[0]);
    //Serial.println(Mb.MBHoldingRegister[1]);
    //Serial.println(Mb.MBHoldingRegister[2]);
    //Serial.println(Mb.MBHoldingRegister[3]);
  }

    if(millis() - timer <= (Mb.MBHoldingRegister[2]*10))
      digitalWrite(led, HIGH);

    else
      digitalWrite(led, LOW);
      
    //delay(10);
   //}
   
}
byte checkRSSI() {
  byte quality;
  long rssi = WiFi.RSSI();
  if (rssi <= -100)
    quality = 0;
  else if (rssi >= -50)
    quality = 100;
  else
    rssi = rssi + 100;
  quality = byte(rssi * 2);

  return quality;
}


