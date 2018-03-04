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
    Connection lastConn;

    // Scans hardware, returns a connection
    // object representing the pins (if any)
    // that are connected
    Connection scanPins();

    // Sets pin for scan mode
    void startProbe(const byte pin);

    // Sets pin back to normal mode
    void stopProbe(const byte pin);
  
  public:
    // Constructor
    PegBoard(const byte numPegs, const byte pins[]);
    // Destructor
    ~PegBoard();

    // Returns true, if there's a connection
    // (different than the last seen)
    // Otherwise, returns false.
    boolean hasNewConnection();

    // Returns the most recent connection
    Connection getConnection();
};
 
#endif
