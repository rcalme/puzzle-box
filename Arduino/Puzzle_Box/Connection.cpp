#include "Connection.h"
 
// Empty constructor
Connection::Connection() {
  left = 0;
  right = 0;
  connected = false;
}

// Constructor with provided pegs
Connection::Connection(byte a, byte b) {
  // Ensure that the smaller of the
  // two values is the left side
  if (a < b) {
    left = a;
    right = b;
    connected = true;
  } else if (b > a) {
    left = b;
    right = a;
    connected = true;
  } else {
    left = a;
    right = a;
    connected = false;
  }
}

// Overload equals comparator
boolean Connection::operator==(const Connection &rhs) {
  return (left==rhs.left && right==rhs.right);
}

// Returns true if connection is connected
boolean Connection::isConnected() {
  return connected;
}

// Prints connection to Serial port
void Connection::print() {
  if (connected) {
    Serial.print("(");
    Serial.print(left);
    Serial.print(",");
    Serial.print(right);
    Serial.print(")");
  } else {
    Serial.print("Not connected");
  }
}
