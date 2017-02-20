#include <Keypad.h>

#define INPUT_PULLUP 0x2
#define OPEN LOW
#define CLOSED HIGH

const int pegCount = 10;
const int ledCount = 5;
int pegPins[pegCount] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
int ledPins[ledCount] = { A0, A1, A2, A3, A4 };
int pegCombo[ledCount][2] = {
  {1,7},
  {3,5},
  {2,8},
  {4,9},
  {0,6}
};
int progress = 0;
int pegLast[2] = {0, 0};
int successSoundPin = 13;
int failureSoundPin = 12;

void setup() {
  // Enable serial connection
  Serial.begin(9600);
  // Set all peg pins with pull-up resistance (high, unless pulled to ground)
  for (int i=0; i<pegCount; i++) {
    pinMode(pegPins[i],INPUT_PULLUP);
  }
  // Set all LED pins as OUTPUT
  for (int j=0; j<ledCount; j++) {
    pinMode(ledPins[j],OUTPUT);
  }
  // Sound system
  pinMode(successSoundPin, OUTPUT);
  pinMode(failureSoundPin, OUTPUT);
}

void loop() {
  scanPegs();
  delay(100);
}

// Hardware scan
void scanPegs() {
  // One by one, pick a peg
  for (int in=0; in<pegCount; in++) {
    // Set it as output
    pinMode(pegPins[in],OUTPUT);
    // Tie that peg to ground
    digitalWrite(pegPins[in], LOW);
    // One by one, pick every OTHER peg
    for (int out=0; out<pegCount; out++) {
      // Don't consider the same peg simultaneously as both input and output
      if (in == out) {
        continue;
      }
      if (!digitalRead(pegPins[out])) {
        // We've seen a connected pair of pegs
        int pegPair[2];
        if (in < out) {
          pegPair[0] = in;
          pegPair[1] = out;
        }
        else {
          pegPair[0] = out;
          pegPair[1] = in;
        }
        if( pegPair[0] != pegLast[0] || pegPair[1] != pegLast[1]) {
          if (pegPair[0] == pegCombo[progress][0] && pegPair[1] == pegCombo[progress][1]){            
            // Print connection pair
            Serial.print("(");
            Serial.print(pegPair[0]);
            Serial.print(",");
            Serial.print(pegPair[1]);
            Serial.print(")");

            Serial.print("  Good connection! Lock ");
            Serial.print(progress+1);
            Serial.println(" disabled.");

            // Light up another LED
            Serial.print("LED ");
            Serial.print(progress+1);
            Serial.println(" on.");
            digitalWrite(ledPins[progress],HIGH);

            // Move forward in the combination
            progress++;

            // We've connected all the peg pairs
            if (progress >= 5) {
              Serial.println("Access granted.");
              
              // Play a success sound
              digitalWrite(successSoundPin,HIGH);
              delay(1000);
              digitalWrite(successSoundPin,LOW);
            }
          }
          else {
            Serial.println("  Bad connection. All locks resetting.");
            
            // Play failure sound
            digitalWrite(failureSoundPin,HIGH);
            
            // Turn off all LEDs.
            for (int i=progress; i>=0; i--) {
              Serial.print("LED ");
              Serial.print(i+1);
              Serial.println(" off.");
              digitalWrite(ledPins[progress],LOW);
              delay(500);
            }

            // Stop failure sound
            digitalWrite(failureSoundPin,LOW);

            // Reset progress to 0
            progress = 0;
          }
          
          // Remember last connection pair
          pegLast[0] = pegPair[0];
          pegLast[1] = pegPair[1];
        }
      }
    }
    // Disconnect this input peg from ground
    digitalWrite(pegPins[in],HIGH);
    pinMode(pegPins[in],INPUT_PULLUP);
  }
}

