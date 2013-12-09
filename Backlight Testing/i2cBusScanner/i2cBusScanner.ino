/**
 * I2CScanner.pde -- I2C bus scanner for Arduino
 *
 * 2009, Tod E. Kurt, http://todbot.com/blog/
 *
 */

#include "Wire.h"
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

const int redLedPin = 10;
const int greenLedPin = 11;
const int blueLedPin = 12;

// Scan the I2C bus between addresses from_addr and to_addr.
// On each address, call the callback function with the address and result.
// If result==0, address was found, otherwise, address wasn't found
// (can use result to potentially get other status on the I2C bus, see twi.c)
// Assumes Wire.begin() has already been called
void scanI2CBus(byte from_addr, byte to_addr, 
                void(*callback)(byte address, byte result) ) 
{
  byte rc;
  byte data = 0; // not used, just an address to feed to twi_writeTo()
  for( byte addr = from_addr; addr <= to_addr; addr++ ) {
    rc = twi_writeTo(addr, &data, 0, 1, 0);
    callback( addr, rc );
  }
}

// Called when address is found in scanI2CBus()
// Feel free to change this as needed
// (like adding I2C comm code to figure out what kind of I2C device is there)
void scanFunc( byte addr, byte result ) {
  Serial.print("addr: ");
  Serial.print(addr,DEC);
  Serial.print( (result==0) ? " found!":"       ");
  Serial.print( (addr%4) ? "\t":"\n");
}


byte start_address = 1;
byte end_address = 100;

// standard Arduino setup()
void setup()
{
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(blueLedPin, OUTPUT);
    digitalWrite(redLedPin, LOW);    // set the LED off
    digitalWrite(greenLedPin, LOW);  // set the LED off
    digitalWrite(blueLedPin, LOW);   // set the LED off
    Wire.begin();

    Serial.begin(9600);
    Serial.println("\nI2CScanner ready!");

    Serial.print("starting scanning of I2C bus from ");
    Serial.print(start_address,DEC);
    Serial.print(" to ");
    Serial.print(end_address,DEC);
    Serial.println("...");

    // start the scan, will call "scanFunc()" on result from each address
    scanI2CBus( start_address, end_address, scanFunc );

    Serial.println("\ndone");
}

// standard Arduino loop()
void loop() 
{
  Serial.println("Setting color enable pins low");
  digitalWrite(redLedPin, LOW);    // set the LED off
  digitalWrite(greenLedPin, LOW);  // set the LED off
  digitalWrite(blueLedPin, LOW);   // set the LED off
  delay(300);
}