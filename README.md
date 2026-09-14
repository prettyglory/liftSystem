\# LiftSystem



\## Accessible Elevator Control System Using Arduino



LiftSystem is an Arduino-based elevator control system designed as a simulation in Tinkercad. The project demonstrates how an Arduino Uno can control an elevator prototype using floor-selection buttons, an LCD display, a servo motor, and a buzzer.



The system is designed with accessibility in mind, providing simple floor selection and visual and audio feedback.



\## Project Features



\* Four floor-selection buttons:



&#x20; \* Basement

&#x20; \* Ground Floor

&#x20; \* Floor 1

&#x20; \* Floor 2

\* Arduino Uno as the main controller

\* 16×2 LCD for displaying elevator information

\* Servo motor for representing elevator movement

\* Buzzer for arrival notification

\* Resistors for stable button inputs and LCD backlight protection

\* Tinkercad simulation for testing the complete circuit



\## How the System Works



When a user presses one of the floor-selection buttons, the Arduino detects the selected floor.



The Arduino then:



1\. Identifies the selected floor.

2\. Displays the floor information on the LCD.

3\. Displays a floor-changing message.

4\. Moves the servo motor to the position representing the selected floor.

5\. Activates the buzzer when the elevator reaches the selected floor.



\## Components



\* Arduino Uno

\* 16×2 LCD

\* Servo motor

\* 4 push buttons

\* Buzzer

\* 10 kΩ resistors

\* 220 Ω resistor

\* Connecting wires

\* Power supply



\## Project Structure



```text

liftSystem/

├── liftSystem.ino

├── README.md

├── report/

│   └── LiftSystem\_Report.pdf

├── images/

│   └── circuit.png

└── tinkercad/

&#x20;   └── tinkercad\_link.txt

```



\## Tinkercad Simulation



The complete circuit and simulation were developed using Tinkercad.



The Tinkercad project link is available in:



`tinkercad/tinkercad\_link.txt`



\## Main Arduino File



The main Arduino source code is:



`liftSystem.ino`



\## Purpose



The purpose of LiftSystem is to demonstrate the application of Arduino programming, digital inputs, LCD output, servo motor control, and audio feedback in an accessible elevator-control prototype.



\## Author



Gloria Beneth Mbilinyi



Computer Engineering



Dar es Salaam Institute of Technology (DIT)



