V2:
/*
  SimpleMQTTClient.ino
  The purpose of this exemple is to illustrate a simple handling of MQTT and Wifi connection.
  Once it connects successfully to a Wifi network and a MQTT broker, it subscribe to a topic and send a message to it.
  It will also send a message delayed 5 seconds later.
*/

#include "EspMQTTClient.h"
//const int buttonPin = 5;

//int buttonState = 0;
//int lastButtonState = 0;

int coopState = 0; // 0 = Closed, 1 = Opening, 2 = Opened, 3 = Closing, 4 = Manual Override (Physical buttons)

const int limitSwitchOpen = 5;
const int limitSwitchClosed = 4;

const int motorOpen = 13;
const int motorClose = 12;

int lastCoopState = 0;
int currentCoopState = 0;

//const int emergencyStop = 14;

EspMQTTClient client(
  "Wifi_name", //WIFI Name
  "Wifi_password", //Wifi Password
  "192.168.XXX.XX",  // MQTT Broker server ip
  "mqtt_user", //"MQTTUsername",   // Can be omitted if not needed
  "mqtt_password", //"MQTTPassword",   // Can be omitted if not needed
  "Motor_Controller",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

void setup()
{

  //Serial.begin(115200);
  //pinMode(buttonPin, INPUT);
  pinMode(limitSwitchOpen, INPUT);
  pinMode(limitSwitchClosed, INPUT);

  pinMode(motorOpen, OUTPUT);
  pinMode(motorClose, OUTPUT);

  digitalWrite(motorOpen, HIGH);
  digitalWrite(motorClose, HIGH);

  if (digitalRead(limitSwitchOpen) == HIGH) {
    coopState = 2;

  }
  else if (digitalRead(limitSwitchClosed) == HIGH) {
    coopState = 0;
  }
  else {
    coopState = 4;
  }
  int lastCoopState = coopState;
  int currentCoopState = coopState;

  // Optional functionalities of EspMQTTClient :
  //client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  client.publish("motor/connected", "Connected to Motor_Controller!"); // You can activate the retain flag by setting the third parameter to true
  client.publish("motor/coopState", "Coopstate: " + String(coopState));
  client.subscribe("motor/open_coop", openCoop);
  client.subscribe("motor/close_coop", closeCoop);
}

void loop()

  client.loop();
  digitalWrite(motorClose, HIGH);
  digitalWrite(motorOpen, HIGH);
  if (digitalRead(limitSwitchOpen) == HIGH) {
    coopState = 2;
    currentCoopState = coopState;
    if (currentCoopState != lastCoopState) {
      client.publish("motor/opened", "Chickens released!");
      lastCoopState = currentCoopState;
    }
  }
  else if (digitalRead(limitSwitchClosed) == HIGH) {
    coopState = 0;
    currentCoopState = coopState;
    if (currentCoopState != lastCoopState) {
      client.publish("motor/closed", "Chickens captured!");
      lastCoopState = currentCoopState;
    }
  }
  else {
    coopState = 4;
    currentCoopState = coopState;
    if (currentCoopState != lastCoopState) {
      client.publish("motor/manual", "Manual override!");
      lastCoopState = currentCoopState;
    }
  }
  delay(1);
}

void openCoop(const String &msg)
{

  if (digitalRead(limitSwitchOpen) == HIGH) {
    return;
  }
  coopState = 1;
  client.publish("motor/opening", "Chickens will be released!");
  while (digitalRead(limitSwitchOpen) != HIGH) {

    digitalWrite(motorClose, HIGH);
    digitalWrite(motorOpen, LOW);
    //Serial.println(digitalRead(limitSwitchOpen));
    delay(1);
  }
  coopState = 2;
  //client.publish("motor/opened", "Chickens released!");
}

void closeCoop(const String &msg)
{

  if (digitalRead(limitSwitchClosed) == HIGH) {
    return;
  }
  coopState = 3;
  client.publish("motor/closing", "Chickens will be captured!");
  while (digitalRead(limitSwitchClosed) != HIGH) {
    digitalWrite(motorClose, LOW);
    digitalWrite(motorOpen, HIGH);
    //Serial.println(digitalRead(limitSwitchClosed));
    delay(1);
  }
  coopState = 0;
  //client.publish("motor/closed", "Chickens captured!");
}