// Include the required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>


// Define the pins connected to the L298 motor driver
const int ENA = D1;   // PWM signal to control motor speed
const int IN1 = D2;   // Direction control for motor
const int IN2 = D3;   // Direction control for motor

// Define the pin for the built-in LED
const int LED_PIN = LED_BUILTIN;

const char* ssid = "Home";
const char* password = "galosa3323";


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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);

  Serial.begin(115200);
  delay(10);
  
  blinkLED(3,100);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    blinkLED(1,200);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Stop the motor for 1 second
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  blinkLED(2,100);
  delay(300);

  // Turn the motor forward for 2 seconds
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  blinkLED(2);
  delay(2000);

  // Stop the motor for 1 second
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  blinkLED(1,500);

  // Increase the motor speed for 2 seconds
  // analogWrite(ENA, 255);

  // Reverse the motor direction for 2 seconds
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  blinkLED(3,100);
  delay(2000);
}

