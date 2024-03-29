const boolean CHATTY_CATHY  = false;
const char*   MQTT_SERVER   = "192.168.1.156";
const char*   MQTT_USERNAME = "blinky-lite-box-01";
const char*   MQTT_PASSWORD = "areallybadpassword";
const char*   BOX           = "blinky-lite-box-01";
const char*   TRAY_TYPE     = "blinky-mqtt";
const char*   TRAY_NAME     = "maxi-01";
const char*   HUB           = "cube";

#include <Controllino.h>  
union CubeData
{
  struct
  {
    int16_t state;
    int16_t watchdog;
    int16_t linoD0;
    int16_t linoD1;
    int16_t linoR0;
    int16_t linoR1;
  };
  byte buffer[12];
};
CubeData cubeData;
byte mac[] = { 0x42, 0x4C, 0x30, 0x30, 0x30, 0x31 };

#include "BlinkyEtherCube.h"

unsigned long lastPublishTime;
unsigned long publishInterval = 2000;

void setup()
{
  if (CHATTY_CATHY)
  {
    Serial.begin(9600);
    delay(10000);
  }

  // Optional setup to overide defaults
  BlinkyEtherCube.setChattyCathy(CHATTY_CATHY);
  BlinkyEtherCube.setMqttRetryMs(3000);
  BlinkyEtherCube.setBlMqttKeepAlive(8);
  BlinkyEtherCube.setBlMqttSocketTimeout(4);
  
  // Must be included
  BlinkyEtherCube.setMqttServer(mac, MQTT_SERVER, MQTT_USERNAME, MQTT_PASSWORD);
  BlinkyEtherCube.setMqttTray(BOX,TRAY_TYPE,TRAY_NAME, HUB);
  BlinkyEtherCube.init(&cubeData);

  lastPublishTime = millis();
  cubeData.state = 1;
  cubeData.watchdog = 0;
  cubeData.linoD0 = 0;
  cubeData.linoD1 = 0;
  cubeData.linoR0 = 0;
  cubeData.linoR1 = 0;

  pinMode(CONTROLLINO_D0, OUTPUT);
  pinMode(CONTROLLINO_D1, OUTPUT); 
  pinMode(CONTROLLINO_R0, OUTPUT); 
  pinMode(CONTROLLINO_R1, OUTPUT); 
   
  digitalWrite(CONTROLLINO_D0, cubeData.linoD0);   
  digitalWrite(CONTROLLINO_D1, cubeData.linoD1);   
  digitalWrite(CONTROLLINO_R0, cubeData.linoR0);   
  digitalWrite(CONTROLLINO_R1, cubeData.linoR1);   

}

void loop()
{
  unsigned long nowTime = millis();
  
  if ((nowTime - lastPublishTime) > publishInterval)
  {
    lastPublishTime = nowTime;
    cubeData.watchdog = cubeData.watchdog + 1;
    if (cubeData.watchdog > 32760) cubeData.watchdog= 0 ;
    BlinkyEtherCube.publishToServer();
  }  
  BlinkyEtherCube.loop();
}
void handleNewSettingFromServer(uint8_t address)
{
  switch(address)
  {
    case 0:
      break;
    case 1:
      break;
    case 2:
      digitalWrite(CONTROLLINO_D0, cubeData.linoD0);  
      break;
    case 3:
      digitalWrite(CONTROLLINO_D1, cubeData.linoD1);    
      break;
    case 4:
      digitalWrite(CONTROLLINO_R0, cubeData.linoR0);  
      break;
    case 5:
      digitalWrite(CONTROLLINO_R1, cubeData.linoR1);    
      break;
    default:
      break;
  }
}
