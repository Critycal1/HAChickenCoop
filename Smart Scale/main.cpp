#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include "EspMQTTClient.h"
#include <HX711_ADC.h>
#include <EEPROM.h>

// Initialize periphery
#define BUTTON 0 // D3
#define BUZZER 13 // D7

// Initialize Screen:
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize Scale:
#define HX711_dout 12 // D6
#define HX711_sck 14 // D5
HX711_ADC LoadCell(HX711_dout, HX711_sck);

float prevWeight = 0.0;
float currentWeight = 0.0;

// Initialize chicken array:
char *chickens[] = {"Bella", "Blue", "Cleo",
                     "Mocha", "Tara", "Tiffy"
                    };

// Initialize counter:
uint16 cnt = 0;

// Time handling:
const long delta = 600; // Time in ms to recognize "long button press"
unsigned long previousMillis = 0;
unsigned long currentMillis = millis();

// Confirmation bool, to get feedback from HA:
bool connected = false;

// Initialize MQTT Client:
EspMQTTClient client(
  "YOUR_WIFI_NAME",
  "YOUR_WIFI_PASSWORD",
  "192.168.178.95",  // MQTT Broker server ip
  "YOUR_MQTT_USERNAME", //"MQTTUsername",   // Can be omitted if not needed
  "YOUR_MQTT_PASSWORD", //"MQTTPassword",   // Can be omitted if not needed
  "AEW",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

// Splashscreen
void splashscreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("AUTOMATIC"));
  display.println(F("EGG"));
  display.println(F("WEIGHING"));
  display.setTextColor(WHITE);
  display.setTextSize(1);   
  display.println("");
  display.println("by Gerrit");
  display.display();
}

void setup()
{
  //Serial.begin(9600); // Only necessary for debugging.
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  LoadCell.begin();
  
  splashscreen();
  client.enableHTTPWebUpdater();
  client.enableLastWillMessage("AEW/lastwill", "AEW goes offline!");

  LoadCell.start(2000, true);
  LoadCell.setCalFactor(1042.1);
}

void onConnectionEstablished()
{
  client.publish("AEW/connected", "Connected to Automatic Egg Weigher!");

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,26);
  display.println("CONNECTED!");
  display.display();
  connected = true;
  delay(1000);
}


void pickchicken(int chicken){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("Choose a chicken:");
  display.setCursor(35, 25);
  display.setTextSize(2);  
  display.println(chickens[chicken]);
  display.display();
}

void confirmation(){
  display.setTextSize(1); 
  display.setCursor(10, 50);
  display.println("Success!");
  display.display();
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  delay(50);
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  delay(50);
  delay(1000);
}

void weighegg(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);        
  display.setCursor(0,30);
  display.println("Taring Load Cell...");
  display.display();
  LoadCell.tare();
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);

  // While the weight on the scale is < 10g, it won't be recognized
  // as an egg. So it wait's until an egg is present and starts the
  // weighing process.
  do
  {
    display.clearDisplay();
    display.setTextSize(1); 
    display.setCursor(0, 30);
    display.println("Waiting for egg...");
    display.display();
    LoadCell.update();
    LoadCell.getData();
  } while (LoadCell.getData() < 10.0);

  // As long as the difference between the current wait subtracted from
  // the previous weight is (absolute) > 0.006g, the weighing process won't
  // be finished.
  do
  {
    LoadCell.update();
    prevWeight = LoadCell.getData();
    for (size_t i = 0; i < 10; i++){
    LoadCell.update();
    currentWeight = LoadCell.getData();
    delay(20);
  }
    display.clearDisplay();
    display.setCursor(35, 25);
    display.setTextSize(2); 
    display.print(currentWeight);
    display.print("g");
    display.display();
  } while (currentWeight - prevWeight > 0.003 || currentWeight - prevWeight < -0.003);
  delay(100);

  display.clearDisplay();
  display.setTextSize(1); 
  display.setCursor(10,0);
  display.print(chickens[cnt]);
  display.print("'s egg weighs:");
  display.println("");
  display.setCursor(35, 25);
  display.setTextSize(2);
  display.print(currentWeight);
  display.print("g");
  delay(1000);
  // Send the name of the chicken to HA
  client.publish("AEW/chicken", String(chickens[cnt]));
  delay(50);
  // Send the weight of the egg to HA
  client.publish("AEW/weight", String(currentWeight));
  delay(1000);
  confirmation();
  display.display();
  currentWeight = 0;
}

// Time handling for the pushbutton.
// If the button is pressed for a short time (< delta [= 600ms]),
// the counter will be incremented an thus the chicken array will
// go to the next element.
// If it's pressed for longer than 600 ms, the weighing process starts
// for the chosen chicken.
void analyzebutton(){
  if (currentMillis - previousMillis >= delta) {
    weighegg();
  }
  else {
    cnt++;
    if (cnt >= sizeof(chickens) / sizeof(int)){
      cnt = 0;
    } 
  }
}

void loop(){
  // As long as the scale isn't connected to Home Assistant,
  // the splashscreen will be shown. If it stays for an extended
  // period of time, something is wrong with the connection.
if (!connected)
{
  delay(10);
  client.loop();
}
else {
  pickchicken(cnt);
if (!digitalRead(BUTTON))
{
  previousMillis = millis();
  //Serial.println(previousMillis);
  while (!digitalRead(BUTTON))
  {
    delay(5);
  }
  currentMillis = millis();
  analyzebutton();
}
client.loop();
delay(10);
}
} 
