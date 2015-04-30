/*------------------------------------------------------------------------
  Example sketch for Adafruit Thermal Printer library for Arduino.
  Demonstrates some of the available alternate characters.
  See 'A_printertest' sketch for a more generalized printing example.

  THESE FEATURES ARE NOT AVAILABLE ON ALL PRINTERS.  Older firmware
  does not support the alternate characters used here.
  ------------------------------------------------------------------------*/

#include "Adafruit_Thermal.h"

// Here's the syntax when using SoftwareSerial (e.g. Arduino Uno) --------
// If using hardware serial instead, comment out or remove these lines:

#include "SoftwareSerial.h"
#define TX_PIN 6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor
// Then see setup() function regarding serial & printer begin() calls.

// Here's the syntax for hardware serial (e.g. Arduino Due) --------------
// Un-comment the following line if using hardware serial:

//Adafruit_Thermal printer(&Serial1);      // Or Serial2, Serial3, etc.

// -----------------------------------------------------------------------

void setup() {

  // This line is for compatibility with the Adafruit IotP project pack,
  // which uses pin 7 as a spare grounding point.  You only need this if
  // wired up the same way (w/3-pin header into pins 5/6/7):
  pinMode(7, OUTPUT); digitalWrite(7, LOW);

  mySerial.begin(19200);  // Initialize SoftwareSerial
  //Serial1.begin(19200); // Use this instead if using hardware serial
  printer.begin();        // Init printer (same regardless of serial type)

  printer.underlineOn();
  printer.println(F("CHARACTER SET EXAMPLE\n"));
  printer.underlineOff();

  printer.println(F("DEFAULT CHARSET & CODE PAGE:"));
  dump();

  printer.println(F("\nNORWAY CHARSET w/"));
  printer.println(F("KATAKANA CODEPAGE:"));
  // Charset selection alters a few chars in ASCII 0x23-0x7E range.
  printer.setCharset(CHARSET_NORWAY);
  // Code page selects alt symbols for 'upper' ASCII 0x80-0xFF.
  // There's a TON of these, best to check datasheet!
  printer.setCodePage(CODEPAGE_KATAKANA);
  dump();

  printer.feed(2);
  printer.setDefault(); // Restore printer to defaults
}

// Print charset map to printer, similar to test page but
// without the baud rate, etc.
void dump() {
  uint8_t major, minor, c;

  printer.println(F("        01234567  89ABCDEF"));
  for(major=0; major<16; major++) {
    printer.print(F("     "));
    printer.print(major, HEX);
    printer.print(F("- "));
    for(minor=0; minor<16; minor++) {
      c = (major << 4) | minor;
      if(c < 32) c = ' '; // Skip control codes!
      printer.write(c);
      if(minor == 7) printer.print(F("  "));
    }
    printer.println();
  }
}

void loop() {
}
