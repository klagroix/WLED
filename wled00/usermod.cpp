#include "wled.h"
/*
 * This v1 usermod file allows you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * EEPROM bytes 2750+ are reserved for your custom use case. (if you extend #define EEPSIZE in const.h)
 * If you just need 8 bytes, use 2551-2559 (you do not need to increase EEPSIZE)
 * 
 * Consider the v2 usermod API if you need a more advanced feature set!
 */

//Use userVar0 and userVar1 (API calls &U0=,&U1=, uint16_t)

// PIR sensor pin
const int MOTION_PIN_LEFT = 16; // Left side of bed
const int MOTION_PIN_RIGHT = 21; // Right side of bed
 // MQTT topic for sensor values
const char MQTT_TOPIC_LEFT[] = "/motion-left";
const char MQTT_TOPIC_RIGHT[] = "/motion-right";

int prevStateLeft = LOW;
int prevStateRight = LOW;

//gets called once at boot. Do all initialization that doesn't depend on network here
void userSetup()
{
  pinMode(MOTION_PIN_LEFT, INPUT);
  pinMode(MOTION_PIN_RIGHT, INPUT);
}

//gets called every time WiFi is (re-)connected. Initialize own network interfaces here
void userConnected()
{

}

void publishMqttLeft(String state)
{
  //Check if MQTT Connected, otherwise it will crash the 8266
  if (mqtt != nullptr){
    char subuf[38];
    strcpy(subuf, mqttDeviceTopic);
    strcat(subuf, MQTT_TOPIC_LEFT);
    mqtt->publish(subuf, 0, true, state.c_str());
  }
}

void publishMqttRight(String state)
{
  //Check if MQTT Connected, otherwise it will crash the 8266
  if (mqtt != nullptr){
    char subuf[38];
    strcpy(subuf, mqttDeviceTopic);
    strcat(subuf, MQTT_TOPIC_RIGHT);
    mqtt->publish(subuf, 0, true, state.c_str());
  }
}

//loop. You can use "if (WLED_CONNECTED)" to check for successful connection
void userLoop()
{
  if (digitalRead(MOTION_PIN_LEFT) == HIGH && prevStateLeft == LOW) { // Motion detected
    publishMqttLeft("ON");
    prevStateLeft = HIGH;
  } 
  if (digitalRead(MOTION_PIN_LEFT) == LOW && prevStateLeft == HIGH) {  // Motion stopped
    publishMqttLeft("OFF");
    prevStateLeft = LOW;
  }


  if (digitalRead(MOTION_PIN_RIGHT) == HIGH && prevStateRight == LOW) { // Motion detected
    publishMqttRight("ON");
    prevStateRight = HIGH;
  } 
  if (digitalRead(MOTION_PIN_RIGHT) == LOW && prevStateRight == HIGH) {  // Motion stopped
    publishMqttRight("OFF");
    prevStateRight = LOW;
  }
}

