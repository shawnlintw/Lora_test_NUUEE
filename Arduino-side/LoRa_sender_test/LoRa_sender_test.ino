#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>
#include <dht.h>


// Lora setting 
// SF : 11
// BW : 125MHz
// CR : 8
/*
Use 3.3V of Arduino to connect it to VCC pin. 
Connect all the GND pins to GND. 
Connect the RST pin to D9 
and DIO0 to D2 of Arduino. 
Connect the SPI Pins NSS, MOSI, MISO, SCK 
to Arduino D10, D11, D12, D13 of Arduino Uno.
*/



#define dhtPin 8    // Connect DHT to arduino pin8   In this case, we use the Arduino uno R3.
dht DHT;

int counter = 0;
unsigned long boot_time=0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSpreadingFactor(11);  
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  boot_time=millis()/1000;
}

void loop() {
  DHT.read11(dhtPin);
  
  Serial.print("Sending packet: ");
  Serial.println(counter);

  StaticJsonBuffer<200> jsonBuffer;
  boot_time=millis()/1000;
  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = "感應器01";
  //root["time"] = 1351824120;
  root["boot_time"]=boot_time;
  root["Sensor Type"]= "DHT11";
  root["Temperature"] = DHT.temperature;
  root["Humidity"] = DHT.humidity;
  
  JsonArray& data = root.createNestedArray("Location");
  data.add(24.5368);
  data.add(120.7874);

  // send packet
  LoRa.beginPacket();
  LoRa.print("01050");		//This is the header of package
  root.printTo(LoRa);
  LoRa.print("43524C46");	//This is the tail of package
  //LoRa.print("hello ");
  //LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(120000);
}
