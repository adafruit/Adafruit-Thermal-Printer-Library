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


class Thermal : public Print {
  public:

    Thermal(int RX_Pin, int TX_Pin);  // constructor
    void begin();
    void reset();
    void setDefault();
    void test();
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
    void flush();
    void sleep();
    void wake();

    void setCharSpacing(int spacing);
    void setSize(char value);
    void setLineHeight(int val = 32);

    void printBarcode(char * text, uint8_t type);
    void setBarcodeHeight(int val);

    void printBitmap(int w, int h, const uint8_t *bitmap);

    // ??
    void tab();

  private:
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

    void printBitmapChunk(int w, uint8_t h, int offset, const uint8_t *bitmap);

    int _RX_Pin;
    int _TX_Pin;

    int heatTime;
    int heatInterval;
    char printDensity;
    char printBreakTime;
};

#endif
