## Elevator with 3 levels controlled by Arduino (Mega)
by M.Kairgaliyev and D.Kim

### Project Demonstration Video:
<div align="center">
  <a href="https://youtu.be/I0xOMNZUyGs"><img src="https://img.youtube.com/vi/I0xOMNZUyGs/0.jpg" alt="Project Demo Video"></a>
</div>

The aim of the project was to design and implement a simulation of real fully functional 3-floor elevator with all its features such as ability to transport objects within the floors, smart doors, overweight system and fire alarm system. The simulation consists of the functional breadboard with implemented logic using Arduino integrated into the prototype elevator built using carton. This project is a good practice of solving real life problems using the knowledge and experience obtained during this course.

# Materials and Methods 
-	Arduino Mega 
-	Breadboard
-	2-wire motor as a main motor which move the elevator cage within the floors
-	Servo motor as a motor responsible for opening and closing the door
-	Proximity sensor to define is there any object (human) going through entry while door is open
-	Force sensor to prevent the elevator from the overweight 
-	Thermistor to detect a fire and turn on fire alarm 
-	4 push-buttons (small) – 3 buttons for calling elevator on each floor and 1 button for emergency situation alarm 
-	LED (Red) as one of the indicators of alarm
-	Wires
-	Resistors for push-buttons, sensors and LED
-	Carton box, metal beams, string and scotch tape to build the elevator
 
<p align="center">
  <img width="460" height="300" src="/images/schematic.png">
</p>

**Table 1:** Sensors and actuators from the Figure 1.

| **#** | **Sensor or actuator** |
| --- | --- |
| 1 | 2-wire motor |
| 2 | Servo motor |
| 3 | Force sensor |
| 4 | Proximity sensor |
| 5 | Breadboard with push-buttons |
| 6 | Thermistor |

<p align="center">
  <img width="350" height="300" src="/images/image1.jpg"> <img width="460" height="300" src="/images/close-up.jpg">
</p>

<p align="center">
  <img width="250" height="450" src="/images/breadboard.jpg">
</p>

# Description of the mechanism logic
- The main mechanism of the elevator, which raises and lowers the elevator cage within the floors, works by the 2-wire motor with implemented logic using Arduino.
- The elevator is called on required floor by pressing the corresponding push-button. After the button was pressed, if the elevator is already on desirable floor it just opens the door, else it goes to the selected floor (if two buttons had been pressed simultaneously, both of them will be in queue). 
- The door is opening up via servo motor and waits some time in order to people come in, also during that time proximity sensor checks if there an object going through entry, if so, door remains open; if no object is entering, then door closes. 
- As in real life, also the overweight might occur. Therefore, force sensor inside the elevator cage makes sure that the weight does not exceeds maximum, if that happens LED inside the elevator turns on and door does not close till the weight normalizes. After weight becomes normal the door closes and elevator continue to work. 
- Furthermore, the emergency system is implemented so that it turns on if either thermistor detects temperature more than maximum temperature or alarm (emergency) button is pressed. If either of these conditions holds, LED inside the elevator turns on and elevator automatically goes down to the first floor.

