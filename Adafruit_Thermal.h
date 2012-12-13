/*************************************************************************
  This is an Arduino library for the Adafruit Thermal Printer.
  Pick one up at --> http://www.adafruit.com/products/597
  These printers use TTL serial to communicate, 2 pins are required.

  Adafruit invests time and resources providing this open source code.
  Please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution.
 *************************************************************************/

#ifndef Thermal_h
#define Thermal_h

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "SoftwareSerial.h"
#else
 #include "WProgram.h"
 #include "WConstants.h"
 #include "NewSoftSerial.h"
#endif

// Barcode types
#define UPC_A   0
#define UPC_E   1
#define EAN13   2
#define EAN8    3
#define CODE39  4
#define I25     5
#define CODEBAR 6
#define CODE93  7
#define CODE128 8
#define CODE11  9
#define MSI    10

#if ARDUINO >= 100
  #define SERIAL_IMPL      SoftwareSerial
  #define PRINTER_PRINT(a) _printer->write(a);
#else
  #define SERIAL_IMPL      NewSoftSerial
  #define PRINTER_PRINT(a) _printer->print(a, BYTE);
#endif

class Adafruit_Thermal : public Print {

 public:

  Adafruit_Thermal(int RX_Pin, int TX_Pin);

  void
    begin(int heatTime=200),
    reset(),
    setDefault(),
    test(),
    testPage(),

    normal(),
    inverseOn(),
    inverseOff(),
    upsideDownOn(),
    upsideDownOff(),
    doubleHeightOn(),
    doubleHeightOff(),
    doubleWidthOn(),
    doubleWidthOff(),
    boldOn(),
    boldOff(),
    underlineOn(uint8_t weight=1),
    underlineOff(),
    strikeOn(),
    strikeOff(),

    justify(char value),
    feed(uint8_t x=1),
    feedRows(uint8_t),
    flush(),
    online(),
    offline(),
    sleep(),
    sleepAfter(uint8_t seconds),
    wake(),
    listen(),

    setSize(char value),
    setLineHeight(int val=32),

    printBarcode(char * text, uint8_t type),
    setBarcodeHeight(int val=50),

    printBitmap(int w, int h, const uint8_t *bitmap, bool fromProgMem = true),
    printBitmap(int w, int h, Stream *stream),
    printBitmap(Stream *stream),

    timeoutSet(unsigned long),
    timeoutWait(),
    setTimes(unsigned long, unsigned long),

    setCharSpacing(int spacing), // Not working
    tab();                       // Not working

  bool hasPaper();

#if ARDUINO >= 100
  size_t write(uint8_t c);
#else
  void   write(uint8_t c);
#endif

 protected:

  SERIAL_IMPL
    *_printer;
  uint8_t
    prevByte,      // Last character issued to printer
    column,        // Last horizontal column printed
    maxColumn,     // Page width (output 'wraps' at this point)
    charHeight,    // Height of characters, in 'dots'
    lineSpacing,   // Inter-line spacing (not line height), in dots
    barcodeHeight; // Barcode height in dots, not including text
  unsigned long
    resumeTime,    // Wait until micros() exceeds this before sending byte
    dotPrintTime,  // Time to print a single dot line, in microseconds
    dotFeedTime;   // Time to feed a single dot line, in microseconds
  int
    printMode,
    _RX_Pin,
    _TX_Pin;
  void
    setPrintMode(uint8_t mask),
    unsetPrintMode(uint8_t mask),
    writePrintMode(),
    writeBytes(uint8_t a),
    writeBytes(uint8_t a, uint8_t b),
    writeBytes(uint8_t a, uint8_t b, uint8_t c),
    writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
};

#endif // Thermal_h
