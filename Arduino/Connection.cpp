#include "Connection.h"
 
// Constructors
Connection::Connection() {
  left = -1;
  right = -1;
}

// Create via value-copy from another
Connection::Connection(Connection conn) {
  left = conn.left;
  right = conn.right;
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
Connection::operator==(const Connection& rhs) {
  return (left==rhs.left && right=rhs.right);
}

// Returns true if connection is connected
boolean Connection::isConnected() {
  return (left != -1 && right != -1);
}

// Prints connection to Serial port
void Connection::print() {
    Serial.print("(");
    Serial.print(left);
    Serial.print(",");
    Serial.print(right);
    Serial.print(")");
}

