# Smart Chicken Coop
<a name="ChickenCoop"></a>
[Jump to the smart egg scale instead](#EggScale)

This project is a smart chicken coop that utilizes Home Assistant automations to control the opening and closing of the coop door, temperature and humidity inside the coop, and the water heater. <br>The coop also features manual controls and a mechanical safety mechanism to prevent injury to the chickens.<br> This repository includes code and diagrams for the communication between the coop's hardware and Home Assistant, as well as an egg scale system for weighing eggs and sending the data to a Notion database and update the corresponding data in Home Assistant.

![ChickenCoop](https://user-images.githubusercontent.com/77540922/211643970-cefdf3b4-d908-4894-b6c9-3a609e6877fd.jpg)

### The coop has the following automations implemented:

- Automatic opening/closing for either time or sunstate
- Turning on/off fans at either time, humidity setpoint or temperature setpoint
- Turning on/off the water heater, if the outside temperature is below a certain setpoint

The [code](https://github.com/Critycal1/HAChickenCoop/blob/master/Smart%20Chicken%20Coop/ChickenCoop.cpp) in this repo is only for the communication between the motor controller and Home Assistant, <br>the automations are programmed in Home Assistant itself.

The YAML in the “Smart Chicken Coop”- folder is to get a custom button card to show the controls in Home Assistant in this style:

<img src="https://i.imgur.com/HYaEfDE.jpg" width="200">

The majority of the automations utilize MQTT.

The water heater is connected via a Shelly.

<img src="https://user-images.githubusercontent.com/77540922/211644115-695d3e9c-8bbd-4d21-b2df-8a173cb2ed62.jpg" width="300">

On the right is the mounted linear motor, which is connected via a steel wire to the chicken door.<br>
It is a safety precaution, so that if a chicken would stand under the door while it is closing,<br>
the door won't crush the chicken.

To make it predator proof, the door doesn't sit flush with the opening on the inside, but closes a couple of centimeters more.<br>
This way a predator can't apply leverage to open the door in the night.


## Door mechanism

<img src="https://user-images.githubusercontent.com/77540922/211645664-50ac3cf4-e50d-41b5-a5ad-9d8228419ad1.jpeg" width="300">

## Electronics and manual mode

For the worst case, there is still a manual mode, that is independant of any wireless communication.<br>
<img src="https://user-images.githubusercontent.com/77540922/211648037-3c12192c-41c4-44e7-aefd-0c0b920b8965.jpg" width="300"><br>
Inside this casing is also the microcontroller (NodeMCU), which communicates with Home Assistant and the motor controller,<br>
which operates the linear motor inside of the coop.

<img src="https://user-images.githubusercontent.com/77540922/211648440-8a786d9b-d129-4045-8705-8ad12b14929e.jpg" width="300">

## Circuit diagram and PCB

Click on the desired image to enlarge it.

<img width="300" alt="PCB_3d" src="https://user-images.githubusercontent.com/77540922/211649760-f89fc387-0799-48d0-8d4d-cb612483e8b7.png">

The "Emergency_Switch" could also function as a fuse. <br>In my case I simply bridged it, because my fuse is in the supply line, which originates in another box.

<img width="330" alt="PCB_3d" src="https://user-images.githubusercontent.com/77540922/211649835-32c1a86a-2395-438b-a334-37786bc39b11.png"><img width="300" alt="Layout" src="https://user-images.githubusercontent.com/77540922/211649880-fd3308bb-dd9e-405e-8375-cf2debd1b71c.png">



## Showcase

https://user-images.githubusercontent.com/77540922/211646261-088ce660-c793-48f9-8b6f-52341e0a7fd4.mov

<a name="EggScale"></a>

# Smart Egg Scale

[Jump back to the coop](#ChickenCoop)

<img src="https://i.imgur.com/UQZohLw.jpg" width="400">

The scale updates the rudimentary database in Home Assistant and the more detailed database in Notion.<br>
Its casing is 3d-printed and the technical specs are the following:

- Wemos D1 Mini (ESP8266)
- 18650 li-ion battery
- 0.96 inch I2C OLED (SSD1306)
- HX711 Load Cell 1kg

### Database in Home Assistant:

<img src="https://i.imgur.com/Cqf3e1A.jpg" width="300">

If a hen laid a daily egg, the "Day" counter increments.<br> Also the name of the chicken turns blue, to have an easier overview of who laid this day.<br>
In this case it were "Heihei" and "Bella", who laid an egg that day.

The weekly counter keeps track from Monday - Sunday, to have an overview of the respective weekly number of eggs.

The top right number with the "#" indicates the total number of eggs the hen laid in its life so far.

After sending the data to Home Assistant, the integration "Node-RED" updates the internal total, weekly and daily counter for the respective chicken. Afterwards a [Python script](https://github.com/Critycal1/HAChickenCoop/blob/master/Smart%20Scale/PyScriptHomeAssistant.py) is called, which sends the just updated data and the measured weight of the egg to Notion.

<img width="600" alt="Node-RED" src="https://user-images.githubusercontent.com/77540922/211668978-3c4c1ee7-16ca-40f5-aa10-2d688f447312.png">


### Database in Notion:

Excerpt from Notion database (only from one chicken):

<img src="https://i.imgur.com/TH8GilB.jpg" width="250">

### Showcase
A short video of the scale in action. The left side of the iPad is Notion, the right side is Home Assistant.<br>
I suggest watching the video with sound, the pushbutton has a very satisfying sound!

- A short press (< 500ms) cycles through the chickens
- A long press (> 500ms) confirms the picked chicken and starts the weighing process. <br>It will only start the process, if there is sufficient weight on the scale.

https://user-images.githubusercontent.com/77540922/211641995-b40bc66f-76ae-48fd-a86e-6386c940e03b.mov



