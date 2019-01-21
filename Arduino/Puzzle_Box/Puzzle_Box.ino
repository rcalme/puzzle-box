#include <Servo.h>
#include "PegBoard.h"
#include "Connection.h"

#define INPUT_PULLUP 0x2
#define OPEN LOW
#define CLOSED HIGH

// Initialize the PegBoard
//   Specify the pins for the peg board connectors
//   NOTE: If using pins 0 and 1, we can't do serial communication at the same time.
const byte pegCount = 10;
const byte pegPins[pegCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
PegBoard pegBoard = PegBoard(pegCount, pegPins);

// Initialize the Servos
//   Specify the pins for the two servo control lines
// Right drawer servo
const byte rightDrawerServoPin = 10;
const int rightDrawerRange[2] = {1000, 2200}; // will likely have to fiddle with these values per servo
// Key drawer servo
const byte keyDrawerServoPin = 11;
const int keyDrawerRange[2] = {800, 2125};    // will likely have to fiddle with these values per servo
// Only use one Servo object, and connect/disconnect as needed.
// We're never controlling both simultaneously.
Servo servo;

// Initialize the LEDs
//   Specify the pins for the pegboard LEDs
const byte ledCount = 6;
const byte ledPins[ledCount] = { A0, A1, A2, A4, A3, A5 };

// Define our peg board combinations
const Connection pegCombos[ledCount] = {
    // Here are the expected combinations, and their order
    // NOTE: The nature of this pegboard makes it impossible
    //       to require a connection between the same digits
    //       (e.g. You can never wire up 2--2)
    // Change these (obviously) to the combinations you would like to use.
    Connection(0,1),
    Connection(0,2),
    Connection(0,3),
    Connection(0,4),
    Connection(0,5),
    Connection(0,6)
    // That's the kind of thing an idiot would have on their luggage!
    // https://www.youtube.com/watch?v=_JNGI1dI-e8
};

// How many combinations have we solved thusfar
byte progress = 0;

void setup() {
  // Enable serial connection - Note: when using pins 0 or 1 for anything else, this won't work.
  //Serial.begin(9600);

  // Enable peg board
  pegBoard.begin();

  // Set all LED pins as OUTPUT
  for (byte j=0; j<ledCount; j++) {
    pinMode(ledPins[j],OUTPUT);
  }
  
  // On power up, we want this drawer to open
  openRightDrawer();
}

void loop() {
  // Check if there are connections
  if (pegBoard.hasNewConnection()) {
    Connection conn = pegBoard.getConnection();
    conn.print();
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

        // Flash all LEDs a few times
        for (int j=0; j<=5; j++) {
          for (int i=0; i<ledCount; i++) {
            digitalWrite(ledPins[i],LOW);
          }
          delay(250);
          for (int i=0; i<ledCount; i++) {
            digitalWrite(ledPins[i],HIGH);
          }
          delay(250);
        }

        // Open the key drawer
        openKeyDrawer();
      }
    }
    // Bad connection - reset progress
    else {
      Serial.println("  Bad connection. All locks resetting.");

      // Turn off all LEDs in reverse order
      for (int i=progress-1; i>=0; i--) {
        // Turn an LED off
        Serial.print("LED ");
        Serial.print(i+1);
        Serial.println(" off.");
        digitalWrite(ledPins[i],LOW);
        delay(500);
      }
        
      // If the box had been solved, and an incorrect connection is made,
      // move the servos back to their starting positions to reset the box.
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

// Causes the servo to move such that the right drawer opens
void openRightDrawer(){
  Serial.println("Opening right drawer");
  moveServo(rightDrawerServoPin, 180, rightDrawerRange);
}
// Causes the servo to move such that the right drawer closes
void closeRightDrawer(){
  Serial.println("Closing right drawer");
  moveServo(rightDrawerServoPin, 130, rightDrawerRange);
}

// Causes the servo to move such that the key drawer opens
void openKeyDrawer(){
  Serial.println("Opening key drawer");
  moveServo(keyDrawerServoPin, 180, keyDrawerRange);
}
// Causes the servo to move such that the key drawer opens
void closeKeyDrawer(){
  Serial.println("Closing key drawer");
  moveServo(keyDrawerServoPin, 0, keyDrawerRange);
}

// Causes the servo to move such that the right drawer closes
void resetServos(){
  Serial.println("Closing both drawers");
  closeRightDrawer();
  closeKeyDrawer();
}

/*
 * Moves the specified servo to the requested degree position.
 * Requires the specification of min/max values.
 * Attaches to the servo, moves it, and detaches. This means that
 * the servo won't hold its position, but also won't jitter
 * or continue to draw battery. This was sufficient for this use.
 */
void moveServo(byte servoPin, int position, const int range[]) {
  // Write first, to avoid jumping
  servo.write(position);
  // Connect, causing the servo to move to the position written earlier
  servo.attach(servoPin, range[0], range[1]);
  // Give the servo some time to get where it's going
  delay(500);
  // Disconnect from the servo, to save battery and avoid jitter
  servo.detach();
}
