/* WiFi Robot Remote Control Mode - L298N 2A **/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

// connections for drive Motor FR & BR
int enA = D0;
int in1 = D2;
int in2 = D1;
// connections for drive Motor FL & BL
int in3 = D4;
int in4 = D3;
int enB = D5;


String command;   // String to store app command state.
int SPEED = 800;  // 330 - 1023.
int speed_Coeff = 3;

ESP8266WebServer server(80);  // Create a webserver object that listens for HTTP request on port 80

unsigned long previousMillis = 0;

String sta_ssid = "Group_39";  // set Wifi networks you want to connect to



bool isrecording = true;
int x = 0;
int instsize = 0;
String instructions = "";
String instructions1 = "";
void setup() {
  Serial.begin(115200);  // set up Serial library at 115200 bps
  Serial.println();
  Serial.println("WiFi Robot Remote Control Mode - L298N 2A");
  Serial.println("------------------------------------------------");


  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  // Set the speed to start, from 0 (off) to 1023 (max speed)
  analogWrite(enA, SPEED);
  analogWrite(enB, SPEED);

  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);



  Serial.println();
  Serial.println("Hostname: " + sta_ssid);

  // first, set NodeMCU as STA mode to connect with a Wifi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(sta_ssid.c_str());
  Serial.println("");
  Serial.print("Connecting to: ");
  Serial.println(sta_ssid);


  // try to connect with Wifi network about 10 seconds
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  while (WiFi.status() != WL_CONNECTED && currentMillis - previousMillis <= 10000) {
    delay(500);
    Serial.print(".");
    currentMillis = millis();
  }

  // if failed to connect with Wifi network set NodeMCU as AP mode
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi-STA-Mode");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    // Wifi LED on when connected to Wifi as STA mode
    delay(3000);
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(sta_ssid);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("");
    Serial.println("WiFi failed connected to " + sta_ssid);
    Serial.println("");
    Serial.println("WiFi-AP-Mode");
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    delay(3000);
  }


  server.on("/", HTTP_handleRoot);     // call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(HTTP_handleRoot);  // when a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();                      // actually start the server

  ArduinoOTA.begin();  // enable to receive update/uploade firmware via Wifi OTA
}

void loop() {

  delay(125);
  if (isrecording == true) {
    ArduinoOTA.handle();    // listen for update OTA request from clients
    server.handleClient();  // listen for HTTP requests from clients

    command = server.arg("State");

    // check HTPP request, if has arguments "State" then saved the value
    if (command == "L") TurnLeft();  // check string then call a function or set a value
    else if (command == "R") TurnRight();
    else if (command == "B") Backward();
    else if (command == "F") Forward();
    else if (command == "V") {
      isrecording = false;
      Serial.println("Received V successfully");
      Serial.println(instructions);
      instsize = instructions.length();
      for (int j = instructions.length() - 2; j >= 0; j = j - 1) {
        if (instructions[j] == 'F')
          instructions1 += "B";
        else if (instructions[j] == 'B')
          instructions1 += "F";
        else if (instructions[j] == 'R')
          instructions1 += "L";
        else if (instructions[j] == 'L')
          instructions1 += "R";
      }
      instructions1 += 'S';
      Serial.println(instructions1);
      instructions = "";
    } else if (command == "S") Stop();
    else if (command == "0") SPEED = 330;
    else if (command == "1") SPEED = 400;
    else if (command == "2") SPEED = 470;
    else if (command == "3") SPEED = 540;
    else if (command == "4") SPEED = 610;
    else if (command == "5") SPEED = 680;
    else if (command == "6") SPEED = 750;
    else if (command == "7") SPEED = 820;
    else if (command == "8") SPEED = 890;
    else if (command == "9") SPEED = 960;
    else if (command == "q") SPEED = 1023;
  } else {
    Serial.println("Executing V loop");
    if (x < instsize) {
      command = instructions1[x];
      Serial.println("Command is ");
      Serial.println(command);
      if (command == "L") TurnLeft();  // check string then call a function or set a value
      else if (command == "R") TurnRight();
      else if (command == "B") Backward();
      else if (command == "F") Forward();
      else if (command == "S") Stop();
      x++;
    } else {
      x = 0;

      instructions += "S";
      instructions1 = instructions;
      instructions = "";
      instructions1 = "";
      isrecording = true;
    }
  }
}

// function prototypes for HTTP handlers
void HTTP_handleRoot(void) {
  server.send(200, "text/html", "");  // Send HTTP status 200 (Ok) and send some text to the browser/client

  if (server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");  // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

// function to move forward
void Forward() {
  analogWrite(enA, SPEED / 3);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, SPEED / 3);
  instructions += command;
}

// function to move backward
void Backward() {
  analogWrite(enA, SPEED / 3);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED / 3);
  instructions += command;
}

// function to turn right
void TurnRight() {
  analogWrite(enA, SPEED);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, SPEED);
  instructions += command;
}

// function to turn left
void

TurnLeft() {
  analogWrite(enA, SPEED);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED);
  instructions += command;
}

// function to move forward left

void Stop() {
  analogWrite(enA, 0);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in1, LOW);
  analogWrite(enB, 0);
  instructions += command;
}
