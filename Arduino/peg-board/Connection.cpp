#include "Connection.h"
 
// Constructors
Connection::Connection() {
  left = 255;
  right = 255;
}

// Create via provided values
Connection::Connection(byte a, byte b) {
  // Ensure that the smaller of the
  // two values is the left side
  if (a < b) {
    left = a;
    right = b;
  }
  else {
    left = b;
    right = a;
  }
}

// Overload equals comparator
boolean Connection::operator==(const Connection &rhs) {
  return (left==rhs.left && right==rhs.right);
}

// Returns true if connection is connected
boolean Connection::isConnected() {
  return (left != 255 && right != 255);
}

// Prints connection to Serial port
void Connection::print() {
    Serial.print("(");
    Serial.print(left);
    Serial.print(",");
    Serial.print(right);
    Serial.print(")");
}

