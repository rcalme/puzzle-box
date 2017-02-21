#include <Keypad.h>
#include "PegBoard.h"
#include "Connection.h"

#define INPUT_PULLUP 0x2
#define OPEN LOW
#define CLOSED HIGH

// Initialize the PegBoard
const byte pegCount = 10;
const byte pegPins[pegCount] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
pegBoard = PegBoard(pegCount, pegPins);

// Initialize the LEDs
const byte ledCount = 5;
const byte ledPins[ledCount] = { A0, A1, A2, A3, A4 };

// Initialize the success and failure sounds
byte successSoundPin = 13;
byte failureSoundPin = 12;

// Set up our combinations
const byte pegCombo[ledCount][2] = {
  Connection(1,7),
  Connection(1,7),
  Connection(3,5),
  Connection(2,8),
  Connection(4,9),
  Connection(0,6)
};

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
}

void loop() {
  // Check if there are connections
  if (pegBoard.hasNewConnection()) {
    conn = pegBoard.getConnection();
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
        }
      }
      // Bad connection - reset progress
      else {
        Serial.println("  Bad connection. All locks resetting.");
            
        // Turn off all LEDs.
        for (int i=progress; i>=0; i--) {
          // Play failure sound
          digitalWrite(failureSoundPin,HIGH);
            
          // Turn an LED off
          Serial.print("LED ");
          Serial.print(i+1);
          Serial.println(" off.");
          digitalWrite(ledPins[progress],LOW);
          delay(500);

          // Stop failure sound
          digitalWrite(failureSoundPin,LOW);
        }

        // Reset progress to 0
        progress = 0;
      }
    }
  }
}

