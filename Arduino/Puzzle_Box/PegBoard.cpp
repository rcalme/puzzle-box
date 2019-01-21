#include "PegBoard.h"
 
// Constructor
PegBoard::PegBoard(const byte numPegs, const byte pins[]) {
  pegCount = numPegs;
  pegPins = new byte[pegCount];
  for(int i=0; i<pegCount; i++){
    pegPins[i]=pins[i];
  }
  lastConn = Connection();
}

// Destructor
PegBoard::~PegBoard() {
  delete [] pegPins;
}

// Sets up pin modes for peg board
void PegBoard::begin() {
  for(int i=0; i<pegCount; i++){
    pinMode(pegPins[i],INPUT_PULLUP);
  }
}

/* 
 * Scans hardware, returns a connection
 * object representing the pins (if any)
 * that are connected
 */
Connection PegBoard::scanPins() {
  // One by one, pick a starting peg
  for (byte in=0; in<pegCount; in++) {
    startProbe(in);
    // One by one, pick every OTHER peg
    for (byte out=0; out<pegCount; out++) {
      // Don't consider the same peg simultaneously as both input and output
      if (in == out) {
        continue;
      }
      if (!digitalRead(pegPins[out])) {
        // We've seen a connected pair of pegs
        stopProbe(in);
        return Connection(in, out);
      }
    }
    // No connections seen using this peg
    stopProbe(in);
  }
  // No pins are connected
  return Connection();
}

void PegBoard::startProbe(byte in) {
  // Set it as output
  pinMode(pegPins[in],OUTPUT);
  // Tie that peg to ground
  digitalWrite(pegPins[in], LOW);
}

void PegBoard::stopProbe(byte in) {
  // Send a 5v signal on this pin
  digitalWrite(pegPins[in],HIGH);
  // Set it as an input, with pullup resistor
  pinMode(pegPins[in],INPUT_PULLUP);
}

// Returns true, if there's a connection (different than the last seen)
// Otherwise, returns false.
boolean PegBoard::hasNewConnection() {
  // Scan the hardware for connections
  Connection currentConn = scanPins();

  if (currentConn.isConnected()) {
      // Same as the last one
      if( currentConn == lastConn ) {
          return false;
      }
      // A new connection has been established
      else {
          lastConn = currentConn;
          return true;
      }
  }
  // No connection present
  else {
    return false;
  }
}

Connection PegBoard::getConnection() {
  return lastConn;
}
