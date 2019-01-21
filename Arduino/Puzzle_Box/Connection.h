#ifndef CONNECTION
#define CONNECTION

#include <Arduino.h>
/*
  Defines a connection between a pair of pegs.

  "left", and "right", are the two pegs participating
  in the connection.
*/
 
class Connection {
  private:
    byte left;
    byte right;
    boolean connected;

  public:
    // Constructors
    Connection();
    // Create via provided values
    Connection(byte a, byte b);

    // Overload equals comparator
    boolean operator==(const Connection &rhs);

    // Returns whether or not a connection is established
    boolean isConnected();

    // Prints the connection to Serial
    void print();

};
    
#endif
