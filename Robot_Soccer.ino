#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

const char* ssid = "Robot Wifi";
const char* password = "87654321";

#define ENA 16    // Enable/speed motors Right    GPIO4(D2)
#define IN_1 5   // L298N in1  motors Right       GPIO0(D3)
#define IN_2 4   // L298N in2 motors Right       GPIO2(D4)
#define IN_3 0  // L298N in3 motors Left        GPIO12(D6)
#define IN_4 2  // L298N in4 motors Left        GPIO13(D7)
#define ENB 14   // Enable/speed motors Left     GPIO15(D8)

String command;
int speedCar = 150;  // 0 to 255
int speed_low = 60;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);  // Gunakan 9600 baud rate
  pinMode(ENA, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(ENB, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}

void HTTP_handleRoot() {
  if (server.hasArg("State")) {
    command = server.arg("State");
    command.trim();
    Serial.print("Command received: ");
    Serial.println(command);

    if (command == "F") goForward();
    else if (command == "B") goBackward();
    else if (command == "L") goLeft();
    else if (command == "R") goRight();
    else if (command == "I") goForwardRight();
    else if (command == "G") goForwardLeft();
    else if (command == "J") goBackRight();
    else if (command == "H") goBackLeft();
    else if (command == "0") speedCar = 100;
    else if (command == "1") speedCar = 120;
    else if (command == "2") speedCar = 140;
    else if (command == "3") speedCar = 160;
    else if (command == "4") speedCar = 180;
    else if (command == "5") speedCar = 200;
    else if (command == "6") speedCar = 215;
    else if (command == "7") speedCar = 230;
    else if (command == "8") speedCar = 240;
    else if (command == "9") speedCar = 255;
    else if (command == "S") stopRobot();
    else Serial.println("Invalid command");
  } else {
    Serial.println("No command received");
  }

  server.send(200, "text/html", "<h1>ESP8266 Web Server</h1><p>Command received!</p>");
}

void goForward() {
  Serial.println("Moving forward");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goBackward() {
  Serial.println("Moving backward");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goRight() {
  Serial.println("Turning right");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goLeft() {
  Serial.println("Turning left");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goForwardRight() {
  Serial.println("Turning Forward Right");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar - speed_low);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goForwardLeft() {
  Serial.println("Turning Forward Left");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar - speed_low);
}

void goBackRight() {
  Serial.println("Turning Back Right");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar - speed_low);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goBackLeft() {
  Serial.println("Turning Back Left");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar - speed_low);
}

void stopRobot() {
  Serial.println("Stopping robot");
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
}