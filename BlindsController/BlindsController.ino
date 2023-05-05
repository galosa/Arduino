// Include the required libraries
#include <Arduino.h> 
#include <ESP8266WiFi.h> // For Wemos d1 mini - wifi
// for encoder AS5600
#include <Wire.h>
#include <AS5600.h>

AS5600 as5600;


// Define the pins connected to the L298 motor driver
const int ENA = D1;   // PWM signal to control motor speed
const int IN1 = D2;   // Direction control for motor
const int IN2 = D3;   // Direction control for motor

const int SECOND = 1000;
// const int BLIND_UP_TIME = 22;
// const int BLIND_DOWN_TIME = 18;
const int BLIND_UP_TIME = 3;
const int BLIND_DOWN_TIME = 3;

// Define the pin for the built-in LED
const int LED_PIN = LED_BUILTIN;

const char* ssid = "Home";
const char* password = "";


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

  //AS5600
  Wire.begin(D7, D8);
  as5600.begin();


}

void loop() {
  int angle = 0;
  // Stop the motor for 1 second
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  blinkLED(2,100);
  delay(2);
  angle = as5600.readAngle();
  Serial.println("a:" + angle);

  // Turn the motor forward for 2 seconds
  // Blind going up
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  blinkLED(2);
  delay(BLIND_UP_TIME*SECOND);

  angle = as5600.readAngle();
  Serial.println("b:" + angle);

  // Serial.print("\tω = ");
  // Serial.println(as5600.getAngularSpeed(AS5600_MODE_RPM));

  // Stop the motor for 1 second
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  blinkLED(1,500);
  delay(2*SECOND);

  angle = as5600.readAngle();
  Serial.println("c:" + angle);
  // Increase the motor speed for 2 seconds
  // analogWrite(ENA, 255);

  // Reverse the motor direction for 2 seconds
  // Blind going up
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  blinkLED(3,100);
  delay(BLIND_DOWN_TIME*SECOND);

    angle = as5600.readAngle();
  Serial.println("d:" + angle);
}

