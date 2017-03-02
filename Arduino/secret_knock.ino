/*
 * Secret Knock Arduino
*/

#include <Servo.h>

/*Pin definitions*/
const int knockSensor = A0;    // (Analog 0) for using the piezo as an input device. (aka knock sensor)
const int ledPin = 13;         // The built in LED
const int servoPin = 9;        // The pin that activates the servo lock.
const int audioOut = 7;        // For chirping
 
/*Tuning constants. Changing the values below changes the behavior of the device.*/
int threshold = 5;                 // Minimum signal from the piezo to register as a knock. Higher = less sensitive. Typical values 1 - 10
const int rejectValue = 25;        // If an individual knock is off by this percentage of a knock we don't unlock. Typical values 10-30
const int averageRejectValue = 15; // If the average timing of all the knocks is off by this percent we don't unlock. Typical values 5-20
const int knockFadeTime = 120;     // Milliseconds we allow a knock to fade before we listen for another one. (Debounce timer.)
const int lockOperateTime = 2500;  // Milliseconds that we operate the lock solenoid latch before releasing it.
const int maximumKnocks = 6;      // Maximum number of knocks to listen for.
const int knockComplete = 1200;    // Longest time to wait for a knock before we assume that it's finished. (milliseconds)
 
byte secretCode[maximumKnocks] = {50, 35, 25, 50, 100, 50};  // Initial setup: "Shave and a Hair Cut, two bits."
int knockReadings[maximumKnocks];    // When someone knocks this array fills with the delays between knocks.
int knockSensorValue = 0;            // Last reading of the knock sensor.
boolean isLocked = true;             // Whether we're locked or not

Servo myservo; // Create servo object
 
void setup() {
  myservo.attach(servoPin);
  pinMode(ledPin, OUTPUT); 
  doorUnlock();   // Unlock the door for a bit when we power up. For system check and to allow a way in if the key is forgotten.
  delay(500);     // This delay is here because the solenoid lock returning to place can otherwise trigger and inadvertent knock.
  doorLock();     // Unlock the door for a bit when we power up. For system check and to allow a way in if the key is forgotten.
}
 
void loop() {
  // Listen for any knock at all.
  knockSensorValue = analogRead(knockSensor);
  
  if (knockSensorValue >= threshold){
     digitalWrite(ledPin, HIGH);
     knockDelay();
     digitalWrite(ledPin, LOW);
     listenToSecretKnock();           // We have our first knock. Go and see what other knocks are in store...
  } 
} 
 
// Records the timing of knocks.
void listenToSecretKnock(){
  int i = 0;
  // First reset the listening array.
  for (i=0; i < maximumKnocks; i++){
    knockReadings[i] = 0;
  }
  
  int currentKnockNumber = 0;               // Position counter for the array.
  int startTime = millis();                 // Reference for when this knock started.
  int now = millis();   
 
  do {                                      // Listen for the next knock or wait for it to timeout. 
    knockSensorValue = analogRead(knockSensor);
    if (knockSensorValue >= threshold){                   // Here's another knock. Save the time between knocks.
      now=millis();
      knockReadings[currentKnockNumber] = now - startTime;
      currentKnockNumber ++;                             
      startTime = now;          
 
      digitalWrite(ledPin, HIGH);
      knockDelay();
      digitalWrite(ledPin, LOW);
    }
 
    now = millis();
  
    // Stop listening if there are too many knocks or there is too much time between knocks.
  } while ((now-startTime < knockComplete) && (currentKnockNumber < maximumKnocks));
  
  //we've got our knock recorded, lets see if it's valid
  if (validateKnock() == true){
      if(isLocked == true){
          delay(200);
          doorUnlock();
          isLocked = false;
      } else {
          doorLock();
          isLocked = true;
      }
  } else {
      // knock is invalid. Blink the LED as a warning to others.
      for (i=0; i < 4; i++){          
        digitalWrite(ledPin, HIGH);
        delay(50);
        digitalWrite(ledPin, LOW);
        delay(50);
      }
  }
}
 
 
// Unlocks the door.
void doorUnlock(){
  myservo.write(180);              // tell servo to go to position in variable 'pos'
}

// Locks the door.
void doorLock(){
  myservo.write(0);              // tell servo to go to position in variable 'pos'
}
 
// Checks to see if our knock matches the secret.
// Returns true if it's a good knock, false if it's not.
boolean validateKnock(){
  int i = 0;
 
  int currentKnockCount = 0;
  int secretKnockCount = 0;
  int maxKnockInterval = 0;               // We use this later to normalize the times.
  
  for (i=0;i<maximumKnocks;i++){
    if (knockReadings[i] > 0){
      currentKnockCount++;
    }
    if (secretCode[i] > 0){         
      secretKnockCount++;
    }
    
    if (knockReadings[i] > maxKnockInterval){   // Collect normalization data while we're looping.
      maxKnockInterval = knockReadings[i];
    }
  }
  
  if (currentKnockCount != secretKnockCount){  // Easiest check first. If the number of knocks is wrong, don't unlock.
    return false;
  }
  
  /*  Now we compare the relative intervals of our knocks, not the absolute time between them.
      (ie: if you do the same pattern slow or fast it should still open the door.)
      This makes it less picky, which while making it less secure can also make it
      less of a pain to use if you're tempo is a little slow or fast. 
  */
  int totaltimeDifferences = 0;
  int timeDiff = 0;
  for (i=0; i < maximumKnocks; i++){    // Normalize the times
    knockReadings[i]= map(knockReadings[i], 0, maxKnockInterval, 0, 100);      
    timeDiff = abs(knockReadings[i] - secretCode[i]);
    if (timeDiff > rejectValue){        // Individual value too far out of whack. No access for this knock!
      return false;
    }
    totaltimeDifferences += timeDiff;
  }
  // It can also fail if the whole thing is too inaccurate.
  if (totaltimeDifferences / secretKnockCount > averageRejectValue){
    return false; 
  }
  
  return true;
}
  
// Deals with the knock delay thingy.
void knockDelay(){
  int itterations = (knockFadeTime / 20);      // Wait for the peak to dissipate before listening to next one.
  for (int i=0; i < itterations; i++){
    delay(10);
    analogRead(knockSensor);                  // This is done in an attempt to defuse the analog sensor's capacitor that will give false readings on high impedance sensors.
    delay(10);
  } 
}
 
// Plays a non-musical tone on the piezo.
// playTime = milliseconds to play the tone
// delayTime = time in microseconds between ticks. (smaller=higher pitch tone.)
void chirp(int playTime, int delayTime){
  long loopTime = (playTime * 1000L) / delayTime;
  pinMode(audioOut, OUTPUT);
  for(int i=0; i < loopTime; i++){
    digitalWrite(audioOut, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(audioOut, LOW);
  }
  pinMode(audioOut, INPUT);
}
