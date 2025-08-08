#include <SoftwareSerial.h> //to allow serial communication on digital pins 

int sensorPin = 2;  // PIR sensor OUT pin
int relayPin = 3;   // Relay IN pin
int bluetoothTx = 10; // HC-05 TX (Connect to Arduino RX)
int bluetoothRx = 11; // HC-05 RX (Connect to Arduino TX)

bool isLightOn = false;   // Light state (false = OFF, true = ON)
bool motionDetected = false; // Tracks if motion is currently detected

SoftwareSerial BT(bluetoothTx, bluetoothRx); // initializing bluetooth communication

void setup() {  
  pinMode(sensorPin, INPUT);   // Set PIR sensor as input
  pinMode(relayPin, OUTPUT);   // Set relay as output
  digitalWrite(relayPin, LOW); // Ensure relay is OFF initially
  
  Serial.begin(9600);  // Initialize serial communication
  BT.begin(9600);      // Start Bluetooth communication
}

void loop() {
  int motionState = digitalRead(sensorPin); // Read PIR sensor state

  if (motionState == HIGH && !motionDetected) {
    motionDetected = true; // Mark motion as detected

    if (isLightOn) {
      // If light is ON, turn it OFF
      Serial.println("Motion detected! Bulb OFF");
      BT.println("Bulb OFF (Motion)");
      digitalWrite(relayPin, LOW);
      isLightOn = false;
    } else {
      // If light is OFF, turn it ON
      Serial.println("Motion detected! Bulb ON");
      BT.println("Bulb ON (Motion)");
      digitalWrite(relayPin, HIGH);
      isLightOn = true;
    }
  }

  if (motionState == LOW) {
    motionDetected = false; // Reset for next detection
  }

  // *Bluetooth Control from Mobile App*
  if (BT.available()) { 
    char command = BT.read(); // Read incoming Bluetooth command

    if (command == '1') {  // If '1' is received, turn ON light
      digitalWrite(relayPin, HIGH);
      Serial.println("Bulb ON via Bluetooth");
      BT.println("Bulb ON (Bluetooth)");
      isLightOn = true;
    } 
    else if (command == '0') {  // If '0' is received, turn OFF light
      digitalWrite(relayPin, LOW);
      Serial.println("Bulb OFF via Bluetooth");
      BT.println("Bulb OFF (Bluetooth)");
      isLightOn = false;
    }
  }
}