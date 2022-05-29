# Smart Chicken Coop

### Smart Chicken Coop

The coop has the following automations implemented:

- Automatic opening/closing for either time or sunstate
- Turning on/off fans at either time, humidity setpoint, temperature setpoint
- Turning on/off the water heater, if the outside temperature is below a certain setpoint

The code in this repo is only for the communication between the motor controller and Home Assistant, the automations are programmed in Home Assistant itself.

The YAML in the “Smart Chicken Coop”- folder is to get a custom button card to show the controls in Home Assistant in this style:

![https://i.imgur.com/HYaEfDE.jpg](https://i.imgur.com/HYaEfDE.jpg)

The majority of the automations utilize MQTT.

The water heater is connected via a Shelly. 

### Smart Egg Scale

The scale updates the rudimentary database in Home Assistant and the more detailed database in Notion.

Database in Home Assistant:

![https://i.imgur.com/Cqf3e1A.jpg](https://i.imgur.com/Cqf3e1A.jpg)

Excerpt from Notion database (only from one chicken):

![https://i.imgur.com/TH8GilB.jpg](https://i.imgur.com/TH8GilB.jpg)