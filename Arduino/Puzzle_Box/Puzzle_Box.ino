#include <Servo.h>
#include "PegBoard.h"
#include "Connection.h"

#define INPUT_PULLUP 0x2
#define OPEN LOW
#define CLOSED HIGH

// Initialize the PegBoard
const byte pegCount = 10;
const byte pegPins[pegCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
PegBoard pegBoard = PegBoard(pegCount, pegPins);

// Initialize the LEDs
const byte ledCount = 6;
const byte ledPins[ledCount] = { A0, A1, A2, A3, A4, A5 };

// Initialize the success and failure sounds
const byte successSoundPin = 13;
const byte failureSoundPin = 12;

// Set up our combinations
const Connection pegCombos[ledCount] = {
  Connection(0,2),
  Connection(2,3),
  Connection(1,5),
  Connection(0,4),
  Connection(1,6),
  Connection(1,7)
};

Servo rightDrawerServo;
Servo keyDrawerServo;
const byte rightDrawerServoPin = 10;
const byte keyDrawerServoPin = 11;

// How many combinations have we solved thusfar
byte progress = 0;


void setup() {
  // Enable serial connection
  Serial.begin(9600);

  // Set all peg pins with pull-up resistance (high, unless pulled to ground)
  for (byte i=0; i<pegCount; i++) {
    pinMode(pegPins[i],INPUT_PULLUP);
  }

  // Set all LED pins as OUTPUT
  for (byte j=0; j<ledCount; j++) {
    pinMode(ledPins[j],OUTPUT);
  }

  // Sound system
  pinMode(successSoundPin, OUTPUT);
  pinMode(failureSoundPin, OUTPUT);

  // Servos
  rightDrawerServo.attach(rightDrawerServoPin);
  keyDrawerServo.attach(keyDrawerServoPin);

  // Play a success sound
  digitalWrite(successSoundPin,HIGH);
  delay(100);
  // On power up, we want this drawer to open
  openRightDrawer();
}

void loop() {
  // Check if there are connections
  if (pegBoard.hasNewConnection()) {
    Connection conn = pegBoard.getConnection();
    conn.print();
    if (conn.isConnected()) {
      // New connection matches expected combination
      if (conn == pegCombos[progress]) {
        Serial.print("  Good connection! Lock ");
        Serial.print(progress+1);
        Serial.println(" disabled.");
  
        // Light up an LED
        Serial.print("LED ");
        Serial.print(progress+1);
        Serial.println(" on.");
        digitalWrite(ledPins[progress],HIGH);
  
        // Move forward in the combination
        progress++;
  
        // We've connected all the peg pairs
        if (progress >= ledCount) {
          Serial.println("Access granted.");
          
          // Play a success sound
          digitalWrite(successSoundPin,HIGH);
          delay(100);
          digitalWrite(successSoundPin,LOW);

          // Open the key drawer
          openKeyDrawer();
        }
      }
      // Bad connection - reset progress
      else {
        Serial.println("  Bad connection. All locks resetting.");
            
        // Turn off all LEDs.
        for (int i=progress-1; i>=0; i--) {
          // Play failure sound
          digitalWrite(failureSoundPin,HIGH);
            
          // Turn an LED off
          Serial.print("LED ");
          Serial.print(i+1);
          Serial.println(" off.");
          digitalWrite(ledPins[i],LOW);
          delay(500);

          // Stop failure sound
          digitalWrite(failureSoundPin,LOW);
        }

        // Reset progress to 0
        progress = 0;
      }
    }
  }
  // There is no longer a connection
  else {
    // We had previously completed the combination, and a wire has been disconnected
    if (progress >= ledCount) {
      Serial.print("Peg board resetting...");
      for (int i=0; i<ledCount; i++) {
        // Turn an LED off
        digitalWrite(ledPins[i],LOW);
      }
      // Reset progress
      progress = 0;
      // Close the drawers
      resetServos();
    }
  }
}

// Causes the servo to move such that the right drawer opens
void openRightDrawer(){
  Serial.print("Opening right drawer");
  rightDrawerServo.write(90);  // 90 degrees
  delay(500);
}

// Causes the servo to move such that the key drawer opens
void openKeyDrawer(){
  Serial.print("Opening key drawer");
  keyDrawerServo.write(180);  // 180 degrees
  delay(500);
}

// Causes the servo to move such that the right drawer closes
void resetServos(){
  Serial.print("Closing both drawers");
  rightDrawerServo.write(0);  // 0 degrees
  delay(250);
  keyDrawerServo.write(0);    // 0 degrees
  delay(250);
}
