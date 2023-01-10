# Smart Chicken Coop

### Smart Chicken Coop

The coop has the following automations implemented:

- Automatic opening/closing for either time or sunstate
- Turning on/off fans at either time, humidity setpoint or temperature setpoint
- Turning on/off the water heater, if the outside temperature is below a certain setpoint

The [code](https://github.com/Critycal1/HAChickenCoop/blob/master/Smart%20Chicken%20Coop/ChickenCoop.cpp) in this repo is only for the communication between the motor controller and Home Assistant, <br>the automations are programmed in Home Assistant itself.

The YAML in the “Smart Chicken Coop”- folder is to get a custom button card to show the controls in Home Assistant in this style:

<img src="https://i.imgur.com/HYaEfDE.jpg" width="200">

The majority of the automations utilize MQTT.

The water heater is connected via a Shelly. 

## Smart Egg Scale

<img src="https://i.imgur.com/UQZohLw.jpg" width="400">

The scale updates the rudimentary database in Home Assistant and the more detailed database in Notion.<br>
Its casing is 3d-printed and the technical specs are the following:

- Wemos D1 Mini (ESP8266)
- 18650 li-ion battery
- 0.96 inch I2C OLED (SSD1306)
- HX711 Load Cell 1kg

### Database in Home Assistant:

<img src="https://i.imgur.com/Cqf3e1A.jpg" width="300">

If a hen laid a daily egg, the "Daily" counter increments.<br> Also the name of the chicken turns blue, to have an easier overview of who laid this day.<br>
In this case it were "Heihei" and "Bella", who laid an egg that day.

The weekly counter keeps track from Monday - Sunday, to have an overview of the respective weekly number of eggs.

The top right number with the "#" indicates the total number of eggs the hen laid in its life so far.

### Database in Notion:

Excerpt from Notion database (only from one chicken):

<img src="https://i.imgur.com/TH8GilB.jpg" width="250">

After sending the data to Home Assistant, the integration "Node-RED" updates the internal total, weekly and daily counter for the respective chicken. Afterwards a [Python script](https://github.com/Critycal1/HAChickenCoop/blob/master/Smart%20Scale/PyScriptHomeAssistant.py) is called, which sends the just updated data and the measured weight of the egg to Notion.

### Showcase
A short video of the scale in action. The left side of the iPad is Notion, the right side is Home Assistant.<br>
I suggest watching the video with sound, the pushbutton has a very satisfying sound!

https://user-images.githubusercontent.com/77540922/211641995-b40bc66f-76ae-48fd-a86e-6386c940e03b.mov



