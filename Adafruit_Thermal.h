/*!
 * @file Adafruit_Thermal.h
 */

#ifndef ADAFRUIT_THERMAL_H
#define ADAFRUIT_THERMAL_H

// *** EDIT THIS NUMBER ***  Printer firmware version is shown on test
// page (hold feed button when connecting power).  Number used here is
// integerized, e.g. 268 = 2.68 firmware.
#define PRINTER_FIRMWARE 629    //268

#include "Arduino.h"

// Barcode types and charsets
#if PRINTER_FIRMWARE >= 264
#define UPC_A 65  //!< UPC-A barcode system. 11-12 char
#define UPC_E 66  //!< UPC-E barcode system. 11-12 char
#define EAN13 67  //!< EAN13 (JAN13) barcode system. 12-13 char
#define EAN8 68   //!< EAN8 (JAN8) barcode system. 7-8 char
#define CODE39 69 //!< CODE39 barcode system. 1<=num of chars
#define ITF 70 //!< ITF barcode system. 1<=num of chars, must be an even number
#define CODABAR 71 //!< CODABAR barcode system. 1<=num<=255
#define CODE93 72  //!< CODE93 barcode system. 1<=num<=255
#define CODE128 73 //!< CODE128 barcode system. 2<=num<=255

#define CHARSET_USA 0           //!< American character set
#define CHARSET_FRANCE 1        //!< French character set
#define CHARSET_GERMANY 2       //!< German character set
#define CHARSET_UK 3            //!< UK character set
#define CHARSET_DENMARK1 4      //!< Danish character set 1
#define CHARSET_SWEDEN 5        //!< Swedish character set
#define CHARSET_ITALY 6         //!< Italian character set
#define CHARSET_SPAIN1 7        //!< Spanish character set 1
#define CHARSET_JAPAN 8         //!< Japanese character set
#define CHARSET_NORWAY 9        //!< Norwegian character set
#define CHARSET_DENMARK2 10     //!< Danish character set 2
#define CHARSET_SPAIN2 11       //!< Spanish character set 2
#define CHARSET_LATINAMERICA 12 //!< Latin American character set
#define CHARSET_KOREA 13        //!< Korean character set
#define CHARSET_SLOVENIA 14     //!< Slovenian character set
#define CHARSET_CROATIA 14      //!< Croatian character set
#define CHARSET_CHINA 15        //!< Chinese character set

#define CODEPAGE_CP437 0    //!< USA, Standard Europe character code table
#define CODEPAGE_KATAKANA 1 //!< Katakana (Japanese) character code table
#define CODEPAGE_CP850 2    //!< Multilingual character code table
#define CODEPAGE_CP860 3    //!< Portuguese character code table
#define CODEPAGE_CP863 4    //!< Canadian-French character code table
#define CODEPAGE_CP865 5    //!< Nordic character code table
#define CODEPAGE_WCP1251 6  //!< Cyrillic character code table
#define CODEPAGE_CP866 7    //!< Cyrillic #2 character code table
#define CODEPAGE_MIK 8      //!< Cyrillic/Bulgarian character code table
#define CODEPAGE_CP755 9    //!< East Europe, Latvian 2 character code table
#define CODEPAGE_IRAN 10    //!< Iran 1 character code table
#define CODEPAGE_CP862 15   //!< Hebrew character code table
#define CODEPAGE_WCP1252 16 //!< Latin 1 character code table
#define CODEPAGE_WCP1253 17 //!< Greek character code table
#define CODEPAGE_CP852 18   //!< Latin 2 character code table
#define CODEPAGE_CP858 19 //!< Multilingual Latin 1 + Euro character code table
#define CODEPAGE_IRAN2 20 //!< Iran 2 character code table
#define CODEPAGE_LATVIAN 21     //!< Latvian character code table
#define CODEPAGE_CP864 22       //!< Arabic character code table
#define CODEPAGE_ISO_8859_1 23  //!< West Europe character code table
#define CODEPAGE_CP737 24       //!< Greek character code table
#define CODEPAGE_WCP1257 25     //!< Baltic character code table
#define CODEPAGE_THAI 26        //!< Thai character code table
#define CODEPAGE_CP720 27       //!< Arabic character code table
#define CODEPAGE_CP855 28       //!< Cyrillic character code table
#define CODEPAGE_CP857 29       //!< Turkish character code table
#define CODEPAGE_WCP1250 30     //!< Central Europe character code table
#define CODEPAGE_CP775 31       //!< Baltic character code table
#define CODEPAGE_WCP1254 32     //!< Turkish character code table
#define CODEPAGE_WCP1255 33     //!< Hebrew character code table
#define CODEPAGE_WCP1256 34     //!< Arabic character code table
#define CODEPAGE_WCP1258 35     //!< Vietnam character code table
#define CODEPAGE_ISO_8859_2 36  //!< Latin 2 character code table
#define CODEPAGE_ISO_8859_3 37  //!< Latin 3 character code table
#define CODEPAGE_ISO_8859_4 38  //!< Baltic character code table
#define CODEPAGE_ISO_8859_5 39  //!< Cyrillic character code table
#define CODEPAGE_ISO_8859_6 40  //!< Arabic character code table
#define CODEPAGE_ISO_8859_7 41  //!< Greek character code table
#define CODEPAGE_ISO_8859_8 42  //!< Hebrew character code table
#define CODEPAGE_ISO_8859_9 43  //!< Turkish character code table
#define CODEPAGE_ISO_8859_15 44 //!< Latin 3 character code table
#define CODEPAGE_THAI2 45       //!< Thai 2 character code page
#define CODEPAGE_CP856 46       //!< Hebrew character code page
#define CODEPAGE_CP874 47       //!< Thai character code page

//QR barcode models
#define MODEL_1 49
#define MODEL_2 50
#define MICRO_QR 51

//QR Code error correction levels
#define LEVEL_L 48
#define LEVEL_M 49
#define LEVEL_Q 50
#define LEVEL_H 51

#else
#define UPC_A 0
#define UPC_E 1
#define EAN13 2
#define EAN8 3
#define CODE39 4
#define I25 5
#define CODEBAR 6
#define CODE93 7
#define CODE128 8
#define CODE11 9
#define MSI 10
#endif

/*!
 * Driver for the thermal printer
 */
class Adafruit_Thermal : public Print {

public:
  // IMPORTANT: constructor syntax has changed from prior versions
  // of this library.  Please see notes in the example code!
  /*!
   * @brief Thermal constructor
   * @param s Serial stream
   * @param dtr Data Terminal Ready control
   */
  Adafruit_Thermal(Stream *s = &Serial, uint8_t dtr = 255);

  size_t
    /*!
     * @brief Writes a character to the thermal printer
     * @param c Character to write
     * @return Returns true if successful
     */
    write(uint8_t c);
  void
    /*!
     * @param heatTime how much time to spend heating up the printer
     */
    begin(uint8_t heatTime=120),
    /*!
     * @brief Disables bold text
     */
    boldOff(),
    /*!
     * @brief Enables bold text
     */
    boldOn(),
    /*!
     * @brief Disables double-height text
     */
    doubleHeightOff(),
    /*!
     * @brief Enables double-height text
     */
    doubleHeightOn(),
    /*!
     * @brief Disables double-width text
     */
    doubleWidthOff(),
    /*!
     * @brief Enables double-width text
     */
    doubleWidthOn(),
    /*!
     * @brief Feeds by the specified number of lines 
     * @param x How many lines to feed 
     */
    feed(uint8_t x=1),
    /*!
     * @brief Feeds by the specified number of individual pixel rows 
     * @param rows How many rows to feed
     */
    feedRows(uint8_t),
    /*!
     * @brief Flush data pending in the printer 
     */
    flush(),
    /*!
     * @brief Disables white/black reverse printing mode
     */
    inverseOff(),
    /*!
     * @brief Enables white/black reverse printing mode
     */
    inverseOn(),
    /*!
     * @brief Set the justification of text
     * @param value justification, must be JUSTIFY_LEFT, JUSTIFY_CENTER, JUSTIFY_RIGHT
     */
    justify(char value),
    /*!
     * @brief Put the printer into an offline state. No other commands can be sent until an online call is made
     */
    offline(),
    /*!
     * @brief Put the printer into an online state after previously put offline
     */
    online(),
    /*!
     * @brief Print a barcode
     * @param text The specified text/number (the meaning varies based on the type of barcode) and type to write to the barcode
     * @param type Value from the datasheet or class-level variables like UPC-A. Note the type value changes depending on the firmware version so use class-level values where possible
     */
    printBarcode(const char *text, uint8_t type),
    /*!
     * @brief Prints a bitmap
     * @param w Width of the image in pixels
     * @param h Height of the image in pixels
     * @param bitmap Bitmap data, from a file.
     * @param fromProgMem
     */
    printBitmap(int w, int h, const uint8_t *bitmap, bool fromProgMem = true),
    /*!
     * @brief Prints a bitmap : Adafruit's code
     * @param w Width of the image in pixels
     * @param h Height of the image in pixels
     * @param bitmap Bitmap data, from a file.
     * @param fromProgMem
     */
    printBitmap_ada(int w, int h, const uint8_t *bitmap, bool fromProgMem=true),
    /*!
     * @brief Prints a bitmap : Adafruit's code
     * @param w Width of the image in pixels
     * @param h Height of the image in pixels
     * @param fromStream Stream to get bitmap data from
     */
    printBitmap_ada(int w, int h, Stream *fromStream),
    /*!
     * @brief Prints a bitmap : Adafruit's code
     * @param fromStream Stream to get bitmap data from
     */
    printBitmap_ada(Stream *fromStream),
    /*!
     * @brief Prints a QR Code, Only works on printers with support for this feature
     * @param text 
     * @param errCorrect 
     * @param moduleSize 
     * @param model 
     * @param timeoutQR 
     */
    printQRcode(char *text, uint8_t errCorrect=48, uint8_t moduleSize=3, uint8_t model=50, uint16_t timeoutQR=4000000L),
    /*!
     * @brief Re-Prints a QR Code, Only works on printers with support for this feature
     * @param timeoutQR 
     */
    reprintQRcode(uint16_t timeoutQR=4000000L),
    /*!
     * @brief Sets text to normal mode
     */ 
	  

    normal(),
    /*!
     * @brief Reset the printer
     */
    reset(),
    /*!
     * @brief Sets the barcode height
     * @param val Desired height of the barcode
     */
    setBarcodeHeight(uint8_t val=50),
    /*!
     * @brief Sets the character spacing
     * @param spacing Desired character spacing
     */
    setCharSpacing(int spacing=0), // Only works w/recent firmware
    /*!
     * @brief Sets the character set
     * @param val Value of the desired character set
     */
    setCharset(uint8_t val=0),
    /*!
     * @brief Sets character code page
     * @param val Value of the desired character code page
     */
    setCodePage(uint8_t val=0),
    /*!
     * @brief Sets the default settings
     */
    setDefault(),
    /*!
     * @brief Sets the line height
     * @param val Desired line height
     */
    setLineHeight(int val=30),
    /*!
     * @brief Set max rows to write
     * @param val Max rows to write
     */
    setMaxChunkHeight(int val=256),
    /*!
     * @brief Sets text size
     * @param value Text size
     */
    setSize(char value),
    /*!
     * @brief Sets print and feed speed
     * @param p print speed
     * @param f feed speed
     */
    setTimes(unsigned long, unsigned long),
    /*!
     * @brief Puts the printer into a low-energy state immediately
     */
    sleep(),
    /*!
     * @brief Puts the printer into a low-energe state after the given number of seconds
     * @param seconds How many seconds to wait until sleeping
     */
    sleepAfter(uint16_t seconds),
    /*!
     * @brief Disables delete line mode
     */ 
    strikeOff(),
    /*!
     * @brief Enables delete line mode
     */
    strikeOn(),
    /*!
     * @brief Sends tab to device
     */
    tab(),                         // Only works w/recent firmware
    /*!
     * @brief Prints test text
     */
    test(),
    /*!
     * @brief Prints test page
     */
    testPage(),
    /*!
     * @brief Sets the estimated completion time for a just-issued task
     * @param x Estimated completion time
     */
    timeoutSet(unsigned long),
    /*!
     * @brief Waits for the prior task to complete 
     */
    timeoutWait(),
    /*!
     * @brief Disables underline
     */
    underlineOff(),
    /*!
     * @brief Enables underline
     * @param weight Weight of the line
     */
    underlineOn(uint8_t weight=1),
    /*!
     * @brief Disables upside-down text mode
     */
    upsideDownOff(),
    /*!
     * @brief Enables upside-down text mode
     */
    upsideDownOn(),
    /*!
     * @brief Wakes device that was in sleep mode
     */
    wake(),

    /*!
     * @brief Cuts the paper
     */
  	cut(),
    /*!
     * @brief Run beep
     */
  	beep(),
    /*!
     * @brief Define Bit Image
     * @param w Width
     * @param h Height
     * @param bitmap Bitmap pointer
     */
    defineBitImage( int w, int h, const uint8_t *bitmap),
    /*!
     * @brief Print the defined Bit Image
     * @param mode
     */
    printDefinedBitImage(int mode=0),
    /*!
     * @brief Print NV Bit Image
     * @param n
     * @param mode
     */
    printNVBitmap(int n, int mode=0),
    /*!
     * @brief Define NV Bitmap
     * @param w Width
     * @param h Height
     * @param bitmap Bitmap pointer
     */
    defineNVBitmap(int w, int h, const uint8_t *bitmap),
    /*!
     * @brief Define NV Bitmap
     * @param w1 First Width
     * @param h1 First Height
     * @param bitmap1 First Bitmap pointer
     * @param w2 Second Width
     * @param h2 Second Height
     * @param bitmap2 Second Bitmap pointer
     */
    defineNVBitmap(int w1, int h1, const uint8_t *bitmap1,int w2, int h2, const uint8_t *bitmap2),
    /*!
     * @brief Sets beep duration
     * @param sec seconds
     */
    setBeep(int sec);

  bool
    /*!
     * @brief Whether or not the printer has paper
     * @return Returns true if there is still paper
     */
    hasPaper();

private:
  Stream *stream;
  uint8_t printMode,
      prevByte,      // Last character issued to printer
      column,        // Last horizontal column printed
      maxColumn,     // Page width (output 'wraps' at this point)
      charHeight,    // Height of characters, in 'dots'
      lineSpacing,   // Inter-line spacing (not line height), in dots
      barcodeHeight, // Barcode height in dots, not including text
      maxChunkHeight,
      dtrPin;         // DTR handshaking pin (experimental)
  boolean dtrEnabled; // True if DTR pin set & printer initialized
  unsigned long
    resumeTime,    // Wait until micros() exceeds this before sending byte
    dotPrintTime,  // Time to print a single dot line, in microseconds
    dotFeedTime;   // Time to feed a single dot line, in microseconds
  void
    writeBytes(uint8_t a),
    writeBytes(uint8_t a, uint8_t b),
    writeBytes(uint8_t a, uint8_t b, uint8_t c),
    writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d),
	// Riva addition _ Updated
    writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h),
	
    setPrintMode(uint8_t mask),
    unsetPrintMode(uint8_t mask),
    writePrintMode();

};

#endif // ADAFRUIT_THERMAL_H
