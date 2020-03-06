#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

const char *ssid = "geomeo/webtoall";
const char *password = "ASDFGHJKL";
#define DoorPin 0

WebThingAdapter *adapter;

const char *DoorTypes[] = {"DoorSensor", "Door" , nullptr};
ThingDevice Door("IR", "Open-Close Monitering", DoorTypes);
ThingProperty DoorOpen("High", "", BOOLEAN, "OpenProperty");

bool lastOpen = false; 

 
void setup() {
  // put your setup code here, to run once:
  pinMode(DoorPin, INPUT);
//  digitalRead(DoorPin, HIGH); 
  Serial.begin(115200);
  
 Serial.println("Connecting to");
 Serial.println(ssid);
 WiFi.mode(WIFI_STA); 
 WiFi.begin(ssid,password);
 while ( WiFi.status() !=WL_CONNECTED )  {
  delay(500);
  Serial.println(".");
 }
 Serial.println();

 //Show connection details
  Serial.println("Connected!");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
 
  adapter = new WebThingAdapter("doorsen", WiFi.localIP());
  
  Door.addProperty(&DoorOpen);
  adapter->addDevice(&Door);
  adapter->begin();
  
  Serial.println("HTTP Server Started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(Door.id);  
  
     

}

void loop() {
  // put your main code here, to run repeatedly:
  ThingPropertyValue tpVal;
  bool pin;
  pin = digitalRead(DoorPin);
  tpVal.boolean = pin;
  DoorOpen.setValue(tpVal);

  
  bool on = DoorOpen.getValue().boolean;
if(on == HIGH)
{  
	Serial.println("open");
  delay(1000);
}
else
{
	Serial.println("close");
  delay(1000);
}

  adapter->update();
}
