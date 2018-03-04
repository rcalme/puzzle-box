#include <Servo.h>
#include "PegBoard.h"
#include "Connection.h"

#define INPUT_PULLUP 0x2
#define OPEN LOW
#define CLOSED HIGH

// Initialize the PegBoard
const byte pegCount = 10;
// NOTE: If using pins 0 and 1, we can't do serial communication at the same time.
const byte pegPins[pegCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
PegBoard pegBoard = PegBoard(pegCount, pegPins);

// Initialize the LEDs
const byte ledCount = 6;
const byte ledPins[ledCount] = { A0, A1, A2, A4, A3, A5 };

// Initialize the success and failure sounds
//const byte successSoundPin = 13;
//const byte failureSoundPin = 12;

// Set up our combinations
const Connection pegCombos[ledCount] = {
    // Here are the expected combinations, and their order
    // NOTE: The nature of this pegboard makes it impossible
    //       to require a connection between the same digits
    //       (e.g. You can never wire up 2--2)
    Connection(0,1),
    Connection(0,2),
    Connection(0,3),
    Connection(0,4),
    Connection(0,5),
    Connection(0,6)
    // That's the kind of thing an idiot would have on their luggage!
    // https://www.youtube.com/watch?v=_JNGI1dI-e8
};

Servo rightDrawerServo;
Servo keyDrawerServo;
const byte rightDrawerServoPin = 10;
const byte keyDrawerServoPin = 11;
const int rightMin = 1000;
const int rightMax = 2200;
const int keyMin = 800;
const int keyMax = 2125;

// How many combinations have we solved thusfar
byte progress = 0;


void setup() {
  // Enable serial connection
  //Serial.begin(9600);

  // Set up the peg board pin modes
  for (byte i=0; i<pegCount; i++) {
    pinMode(pegPins[i],INPUT_PULLUP);
  }

  // Set all LED pins as OUTPUT
  for (byte j=0; j<ledCount; j++) {
    pinMode(ledPins[j],OUTPUT);
  }

  // Sound system
  //pinMode(successSoundPin, OUTPUT);
  //pinMode(failureSoundPin, OUTPUT);

  // Play a success sound
  //digitalWrite(successSoundPin,HIGH);
  //delay(100);
  
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
          //digitalWrite(successSoundPin,HIGH);
          //delay(100);
          //digitalWrite(successSoundPin,LOW);

          // Flash all LEDs a few times
          for (int j=0; j<=5; j++) {
            for (int i=0; i<=ledCount; i++) {
              digitalWrite(ledPins[i],LOW);
            }
            delay(250);
            for (int i=0; i<=ledCount; i++) {
              digitalWrite(ledPins[i],HIGH);
            }
            delay(250);
          }
          
          // Open the key drawer
          openKeyDrawer();

          // Stop failure sound
          //digitalWrite(failureSoundPin,LOW);
        }
      }
      // Bad connection - reset progress
      else {
        Serial.println("  Bad connection. All locks resetting.");
            
        // Turn off all LEDs in reverse order
        for (int i=progress-1; i>=0; i--) {
          // Play failure sound
          //digitalWrite(failureSoundPin,HIGH);
            
          // Turn an LED off
          Serial.print("LED ");
          Serial.print(i+1);
          Serial.println(" off.");
          digitalWrite(ledPins[i],LOW);
          delay(500);

          // Stop failure sound
          //digitalWrite(failureSoundPin,LOW);
        }
        
        // If the box had been solved, and an incorrect connection is made,
        // move the servos back to their starting positions.
        if (progress >= ledCount) {
          Serial.println("Resetting servos...");
          // Close the drawers
          resetServos();
        }
        
        // Reset progress to 0
        progress = 0;
      }
    }
  }  
}

void moveServo(servo, position, minVal, maxVal) {
  servo.write(position);
  servo.attach(servoPin, minVal, maxVal);
  delay(500);
  servo.detach();
}

// Causes the servo to move such that the right drawer opens
void openRightDrawer(){
  Serial.println("Opening right drawer");
  moveServo(rightDrawerServo, 180, rightMin, rightMax);
}
// Causes the servo to move such that the right drawer closes
void closeRightDrawer(){
  Serial.println("Closing right drawer");
  moveServo(rightDrawerServo, 130, rightMin, rightMax);
}

// Causes the servo to move such that the key drawer opens
void openKeyDrawer(){
  Serial.println("Opening key drawer");
  moveServo(keyDrawerServo, 180, keyMin, keyMax);
}
// Causes the servo to move such that the key drawer opens
void closeKeyDrawer(){
  Serial.println("Closing key drawer");
  moveServo(keyDrawerServo, 0, keyMin, keyMax);
}

// Causes the servo to move such that the right drawer closes
void resetServos(){
  Serial.println("Closing both drawers");
  closeRightDrawer();
  closeKeyDrawer();
}
