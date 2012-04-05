/*************************************************** 
  This is a library for the Adafruit Thermal Printer
  
  Pick one up at --> http://www.adafruit.com/products/597
  These printers use TTL serial to communicate, 2 pins are required

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  MIT license, all text above must be included in any redistribution
 ****************************************************/

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

#define UPC_A 0
#define UPC_E 1
#define EAN13 2
#define EAN8  3
#define CODE39 4
#define I25   5
#define CODEBAR 6
#define CODE93 7
#define CODE128 8
#define CODE11  9
#define MSI 10

#if ARDUINO >= 100
  #define SERIAL_IMPL SoftwareSerial
  #define PRINTER_PRINT(a) _printer->write(a);
#else
  #define SERIAL_IMPL NewSoftSerial
  #define PRINTER_PRINT(a) _printer->print(a, BYTE);
#endif


class Adafruit_Thermal : public Print {
  public:

  Adafruit_Thermal(int RX_Pin, int TX_Pin);  // constructor
    void begin();
    void begin(int heatTime);
    void reset();
    void setDefault();
    void test();
    void testPage();

#if ARDUINO >= 100
    size_t write(uint8_t c);
#else
    void write(uint8_t c);
#endif

    void normal();
    void inverseOn();
    void inverseOff();
    void upsideDownOn();
    void upsideDownOff();
    void doubleHeightOn();
    void doubleHeightOff();
    void doubleWidthOn();
    void doubleWidthOff();
    void boldOn();
    void boldOff();
    void underlineOn(uint8_t weight=1);
    void underlineOff();
    void strikeOn();
    void strikeOff();

    void justify(char value);
    void feed(uint8_t x = 1);
    void feedRows(uint8_t);
    void flush();
    void online();
    void offline();
    void sleep();
    void sleepAfter(uint8_t seconds);
    void wake();

    void setCharSpacing(int spacing);
    void setSize(char value);
    void setLineHeight(int val = 32);

    void printBarcode(char * text, uint8_t type);
    void setBarcodeHeight(int val);

    void printBitmap(int w, int h, const uint8_t *bitmap);
    void printBitmap(int w, int h, Stream *stream);
    void printBitmap(Stream *stream);

    // ??
    void tab();

  protected:
    SERIAL_IMPL * _printer;
    boolean linefeedneeded;

    // little helpers to make code easier to read&use
    void writeBytes(uint8_t a);
    void writeBytes(uint8_t a, uint8_t b);
    void writeBytes(uint8_t a, uint8_t b, uint8_t c);
    void writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

    int printMode;
    void writePrintMode();
    void setPrintMode(uint8_t mask);
    void unsetPrintMode(uint8_t mask);

    int _RX_Pin;
    int _TX_Pin;

    int heatTime;
    int heatInterval;
    char printDensity;
    char printBreakTime;
};

#endif
