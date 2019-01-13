#include <Servo.h>

// On start-up, moves the right drawer servo (pin 10)
//
// Also opens a serial connection to the terminal, and allows you to type
// degree values, which will be reflected on the key drawer servo (pin 11)

Servo rightDrawerServo;
Servo keyDrawerServo;
const byte rightDrawerServoPin = 10;
const byte keyDrawerServoPin = 11;
int incomingByte = 0;

void setup() {
  // Enable serial connection
  Serial.begin(9600);
  
  // Servos
  rightDrawerServo.attach(rightDrawerServoPin);
  keyDrawerServo.attach(keyDrawerServoPin);
  
  // On power up, we want this drawer to open
  openRightDrawer();
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.parseInt();
    Serial.print("Moving to ");
    Serial.print(incomingByte, DEC);
    Serial.println(" degrees...");
    keyDrawerServo.write(incomingByte);
  }
}

// Causes the servo to move such that the right drawer opens
void openRightDrawer(){
  Serial.println("Opening right drawer");
  rightDrawerServo.write(90);  // 90 degrees
  delay(500);
}

// Causes the servo to move such that the key drawer opens
void openKeyDrawer(){
  Serial.println("Opening key drawer");
  keyDrawerServo.write(180);  // 180 degrees
  delay(500);
}
