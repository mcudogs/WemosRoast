//#include <ESP8266WiFi.h>
//#include <ModbusTCPSlave.h>
#include <ModbusIP_ESP8266.h>
#include <max6675.h>
#include <Adafruit_MAX31865.h>
//#define MBDebug = true;

////https://forums.adafruit.com/viewtopic.php?f=31&t=61774&sid=06d9885dcf852259d1f568fa96e40265&start=15

////WIFI Settings
///*
// Modbus Registers Offsets
const int BT_HREG = 0;
const int ET_HREG = 1;
const int HT_HREG = 2;

//ModbusTCPSlave Mb;
//ModbusIP object
ModbusIP mb;
  
unsigned long timer, heater = 0; 
unsigned long turnon = 0;
unsigned long turnoff = 0;
unsigned long checkRSSIMillis;
int thermoDO = D1;
int thermoCS = D8;
int thermoCS1 = D5;
int thermoCLK = D3;
//int fancurrent = D7;
int pt100DO = D2;
int led = D6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
//MAX6675 thermocouple1(thermoCLK, thermoCS1, thermoDO);


// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 maxrtd = Adafruit_MAX31865(thermoCS1, pt100DO, thermoDO, thermoCLK);
// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

int readthermoAveraged(int pin) {
  const int samples = 20;
  long total = 0;

  for (int i = 0; i < samples; i++) {
    thermocouple.readCelsius()
    total += thermocouple.readCelsius();
    delay(2);
  }

  return total / samples;
}

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
  //; // wait for serial port to connect.
  }
  Serial.println("Starting Up");
  maxrtd.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
  WiFi.begin("AP0079CF", "1462Dogs04");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mb.server();
  mb.addHreg(BT_HREG, 0xABCD);
  mb.Hreg(BT_HREG, 2030);
  mb.addHreg(ET_HREG, 0xABCD);
  mb.Hreg(ET_HREG, 3040);
  mb.addHreg(HT_HREG, 0xABCD);
  mb.Hreg(HT_HREG, 0);
  pinMode(led, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(thermoCLK, OUTPUT);
  pinMode(thermoCS1, OUTPUT);
  pinMode(thermoCS, OUTPUT);
  pinMode(thermoDO, INPUT);
  pinMode(pt100DO, OUTPUT);
  //pinMode(fancurrent, INPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(led, LOW);
  
  delay(500);
}

void loop()
{
    mb.task();

    if (millis() - timer >= 1000) 
    {
     //mb.Hreg(BT_HREG, 3600);
     mb.Hreg(BT_HREG, thermocouple.readCelsius()*100);
     heater = mb.Hreg(HT_HREG);
     mb.Hreg(ET_HREG, maxrtd.temperature(RNOMINAL, RREF)*100 ); 
     //mb.Hreg(ET_HREG, 4600); 
     Serial.print("Heater = ");
     Serial.println(mb.Hreg(HT_HREG));
     Serial.print("BT = ");
     Serial.println(mb.Hreg(BT_HREG));
     timer = millis();
     Serial.print("ET = ");
     Serial.println(mb.Hreg(ET_HREG));
     delay(5);
    }
    if((millis() - timer)/10 <= heater) 
    {
     if(digitalRead(led) == LOW)
     { 
       turnon = millis();
       digitalWrite(led, HIGH);
       digitalWrite(LED_BUILTIN, LOW);
     }
    }
    else
    {
      if(digitalRead(led) == HIGH)
      {
        turnoff = millis();
        digitalWrite(led, LOW);
        digitalWrite(LED_BUILTIN, HIGH);
      }
     }
}
