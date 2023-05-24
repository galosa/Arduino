// Include the required libraries
#include <Arduino.h> 
#include <ESP8266WiFi.h> // For Wemos d1 mini - wifi
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


// Define the pins connected to the L298 motor driver
const int ENA = D1;   // PWM signal to control motor speed
const int IN1 = D2;   // Direction control for motor
const int IN2 = D3;   // Direction control for motor

// Define the pin for the built-in LED
const int LED_PIN = LED_BUILTIN;

const char* ssid = "Home";
const char* password = "galosa3323";

ESP8266WebServer server(80);


// Function to blink the LED a specified number of times
void blinkLED(int numBlinks, int blink_delay=200) {
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(LED_PIN, LOW);          // turn on LED
    delay(blink_delay);                  // wait for XXXms
    digitalWrite(LED_PIN, HIGH);         // turn off LED
    delay(blink_delay);                  // wait for XXXms
  }
}

void setup() {
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Turn LED off initially

  // Initialize the pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Set the initial motor direction and speed
  // Start OFF
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  // Max speed 255, range 0-255
  analogWrite(ENA, 255);

  Serial.begin(115200);
  while(!Serial) {
    blinkLED(2,100);
  }
    
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    blinkLED(1,200);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start HTTP server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Get the direction, speed, and time from the HTTP request parameters
  String direction = server.arg("direction");
  int speed = server.arg("speed").toInt();
  int time = server.arg("time").toInt();

  // Set the motor direction
  if (direction == "up") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (direction == "down") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    // Invalid direction, stop the motor
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // Set the motor speed
  analogWrite(ENA, speed);

  // Wait for the specified time
  delay(time);

  // Stop the motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Send a response back to the client
  server.send(200, "text/plain", "Motor command executed");
}