#include "PegBoard.h"
 
/* 
   Scans hardware, returns a connection
   object representing the pins (if any)
   that are connected
*/
PegBoard::scanPins() {
      // One by one, pick a peg
      for (byte in=0; in<pegCount; in++) {
        // Set it as output
        pinMode(pegPins[in],OUTPUT);
        // Tie that peg to ground
        digitalWrite(pegPins[in], LOW);
        // One by one, pick every OTHER peg
        for (byte out=0; out<pegCount; out++) {
          // Don't consider the same peg simultaneously as both input and output
          if (in == out) {
            continue;
          }
          if (!digitalRead(pegPins[out])) {
            // We've seen a connected pair of pegs
            return Connection(in, out);
          }
        }
        // Disconnect this input peg from ground
        digitalWrite(pegPins[in],HIGH);
        pinMode(pegPins[in],INPUT_PULLUP);
      }
      // No pins are connected
      return Connection();
    }
    
// Constructor
PegBoard::PegBoard(byte numPegs, byte[] pins) {
  pegCount = pegs;
  pegPins = new byte[pegs];
  for(int i=0; i<pegCount; i++){
    pegPins[i]=pins[i];
  }
}
// Destructor
PegBoard::~PegBoard() {
  delete [] pegPins;
}

// Returns true, if there's a connection
// (different than the last seen)
// Otherwise, returns false.
boolean PegBoard::seesNewConnection() {
  // Scan the hardware for connections
  Connection currentConn = scanPins();

  if (currentConn.isConnected()) {
    // Same as the last one
    if( currentConn == lastConn ) {
      lastConn = Connection(currentConn);
      return false;
    }
    // A new connection has been established
    else {
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

