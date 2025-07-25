# Iot-based-smart-door
An IOT based smart door project that has remote and manual access.
Go to the master brach for the code.
Read the requirement and connections too.
In connections, it is shown for a solenoid lock, but here if you are using Servo motor like me you have to make some changes in connection like:

Servo to Arduino:
IN to D13 on Arduino
VCC to 5V
GND to GND

MC38 door sensor:
Signal pin to D11 on Arduino
GND to GND
