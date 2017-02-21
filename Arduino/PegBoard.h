#ifndef PEGBOARD
#define PEGBOARD
 
#include <Arduino.h>
#include "Connection.h"
 
/*
  Defines a Peg Board hardware configuration.

  Knows how to interface with the hardware to determine
  current connections, and detect change.
*/

class PegBoard {
  private:
    byte pegCount;
    byte* pegPins;
    Connection lastConn();

    // Scans hardware, returns a connection
    // object representing the pins (if any)
    // that are connected
    Connection scanPins();
  
  public:
    // Constructor
    PegBoard(byte numPegs, byte[] pins);
    // Destructor
    ~PegBoard();

    // Returns true, if there's a connection
    // (different than the last seen)
    // Otherwise, returns false.
    boolean seesNewConnection();

    // Returns the most recent connection
    Connection getConnection();
};
 
#endif
