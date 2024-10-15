# WiFi Robot Remote Control (L298N 2A)

This project allows you to remotely control a robot car using a web-based interface via WiFi. The robot is controlled by an ESP8266 module and uses an L298N motor driver to drive the motors. The robot can move forward, backward, turn left, right, and stop. It also has a recording and playback mode for predefined movements.

## Features

- Remote control via WiFi (access through a web server)
- Forward, backward, left, right, and stop commands
- Adjustable speed settings
- Record and playback movement instructions
- Over-the-Air (OTA) firmware updates

## Components

- ESP8266 (NodeMCU) module
- L298N Motor Driver
- DC Motors (2 pairs for left and right sides)
- Power supply for motors and ESP8266
- WiFi-enabled device (phone, tablet, or computer) for control

## Pin Configuration

- Motor 1 (Right) connections:
  - `enA` = D0
  - `in1` = D2
  - `in2` = D1
- Motor 2 (Left) connections:
  - `in3` = D4
  - `in4` = D3
  - `enB` = D5

## Installation

1. Install the necessary libraries:
   - [ESP8266WiFi](https://github.com/esp8266/Arduino)
   - [ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
   - [ArduinoOTA](https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA)

2. Upload the provided code to the ESP8266 using the Arduino IDE.

3. Connect your ESP8266 to your preferred WiFi network or set it in AP mode.

4. Access the robot control interface via the ESP8266’s IP address using a web browser.

## Usage

Once connected, the robot can be controlled via the web interface:

- **F**: Move forward
- **B**: Move backward
- **L**: Turn left
- **R**: Turn right
- **S**: Stop
- **V**: Play the recorded sequence in reverse
- **Speed**: Choose a speed level from 0 (330) to 9 (960), or `q` for full speed (1023)

### Recording Mode

- The robot records all movements after issuing a command.
- When you issue the "V" command, it will play the recorded movements in reverse.

### Over-the-Air (OTA) Updates

OTA allows you to update the firmware without physical access to the ESP8266:
- Ensure the ESP8266 is connected to the same WiFi network.
- Use the Arduino IDE’s OTA feature to upload the new code.

## License

This project is open-source and can be modified or redistributed as per the terms of the MIT License.
