/*!
 * @file Adafruit_Thermal.cpp
 *
 * @mainpage Adafruit Thermal Printer Library
 *
 * @section intro_sec Introduction
 *
 * An Arduino library for the Adafruit Thermal Printer:
 *
 * https://www.adafruit.com/product/597
 *
 * These printers use TTL serial to communicate.  One pin (5V or 3.3V) is
 * required to issue data to the printer.  A second pin can OPTIONALLY be
 * used to poll the paper status, but not all printers support this, and
 * the output on this pin is 5V which may be damaging to some MCUs.
 *
 * Adafruit invests time and resources providing this open source code.
 * Please support Adafruit and open-source hardware by purchasing products
 * from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries, with
 * contributions from the open source community.  Originally based on
 * Thermal library from bildr.org
 *
 * @section license License
 *
 * MIT license, all text above must be included in any redistribution.
 */

#include "Adafruit_Thermal.h"

// Though most of these printers are factory configured for 19200 baud
// operation, a few rare specimens instead work at 9600.  If so, change
// this constant.  This will NOT make printing slower!  The physical
// print and feed mechanisms are the bottleneck, not the port speed.
#define BAUDRATE  19200  //!< How many bits per second the serial port should transfer

// ASCII codes used by some of the printer config commands:
#define ASCII_TAB '\t' //!< Horizontal tab
#define ASCII_LF '\n'  //!< Line feed
#define ASCII_FF '\f'  //!< Form feed
#define ASCII_CR '\r'  //!< Carriage return
#define ASCII_DC2 18   //!< Device control 2
#define ASCII_ESC 27   //!< Escape
#define ASCII_FS 28    //!< Field separator
#define ASCII_GS 29    //!< Group separator

// Because there's no flow control between the printer and Arduino,
// special care must be taken to avoid overrunning the printer's buffer.
// Serial output is throttled based on serial speed as well as an estimate
// of the device's print and feed rates (relatively slow, being bound to
// moving parts and physical reality).  After an operation is issued to
// the printer (e.g. bitmap print), a timeout is set before which any
// other printer operations will be suspended.  This is generally more
// efficient than using delay() in that it allows the parent code to
// continue with other duties (e.g. receiving or decoding an image)
// while the printer physically completes the task.

/*!
 * Number of microseconds to issue one byte to the printer.  11 bits
 * (not 8) to accommodate idle, start and stop bits.  Idle time might
 * be unnecessary, but erring on side of caution here.
 */
#define BYTE_TIME (((11L * 1000000L) + (BAUDRATE / 2)) / BAUDRATE)

// Constructor
Adafruit_Thermal::Adafruit_Thermal(Stream *s, uint8_t dtr)
    : stream(s), dtrPin(dtr) {
  dtrEnabled = false;
}

// This method sets the estimated completion time for a just-issued task.
void Adafruit_Thermal::timeoutSet(unsigned long x) {
  if(!dtrEnabled) {
    resumeTime = (resumeTime > micros() ?  micros() : lastWritingTime) + x;
  }
}

// This function waits (if necessary) for the prior task to complete.
void Adafruit_Thermal::timeoutWait() {
  if(dtrEnabled) {
    while(digitalRead(dtrPin) == HIGH){yield();};
  } else {
    while((long)(micros() - resumeTime) < 0L){yield();}; // (syntax is rollover-proof)
  }
}

// Printer performance may vary based on the power supply voltage,
// thickness of paper, phase of the moon and other seemingly random
// variables.  This method sets the times (in microseconds) for the
// paper to advance one vertical 'dot' when printing and when feeding.
// For example, in the default initialized state, normal-sized text is
// 24 dots tall and the line spacing is 30 dots, so the time for one
// line to be issued is approximately 24 * print time + 6 * feed time.
// The default print and feed times are based on a random test unit,
// but as stated above your reality may be influenced by many factors.
// This lets you tweak the timing to avoid excessive delays and/or
// overrunning the printer buffer.
void Adafruit_Thermal::setTimes(unsigned long p, unsigned long f) {
  dotPrintTime = p;
  dotFeedTime = f;
}

// The next four helper methods are used when issuing configuration
// commands, printing bitmaps or barcodes, etc.  Not when printing text.

void Adafruit_Thermal::writeBytes(uint8_t a) {
  storeInBuffer(a);
}

void Adafruit_Thermal::writeBytes(uint8_t a, uint8_t b) {
  storeInBuffer(a);
  storeInBuffer(b);
}

void Adafruit_Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c) {
  storeInBuffer(a);
  storeInBuffer(b);
  storeInBuffer(c);
}

void Adafruit_Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  storeInBuffer(a);
  storeInBuffer(b);
  storeInBuffer(c);
  storeInBuffer(d);
}

// Riva Addition _ Updated
void Adafruit_Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h) {
  storeInBuffer(a);
  storeInBuffer(b);
  storeInBuffer(c);
  storeInBuffer(d);
  storeInBuffer(e);
  storeInBuffer(f);
  storeInBuffer(g);
  storeInBuffer(h);
}

// The underlying method for all high-level printing (e.g. println()).
// The inherited Print class handles the rest!
size_t Adafruit_Thermal::write(uint8_t c) {
  storeInBuffer(c);
  return 1;
}


void Adafruit_Thermal::storeInBuffer(uint8_t c)
{

#ifdef DEBUG_PRINTER_
  static int count=0;
  count++;
  if(count%100==0)
  {
    DEBUG_("Stored 100 elements, internalBuffer.size()=%d, internalBuffer.capacity()=%d\n",internalBuffer.size(),internalBuffer.capacity());
  }
#endif
  if (c != 13)
  { // Strip carriage returns

    internalBuffer.push_back(c);

    unsigned long d = BYTE_TIME;
    if ((c == '\n') || (column == maxColumn))
    {                                                                        // If newline or wrap
      d += (prevByte == '\n') ? ((charHeight + lineSpacing) * dotFeedTime) : // Feed line
               ((charHeight * dotPrintTime) + (lineSpacing * dotFeedTime));  // Text line
      column = 0;
      c = '\n'; // Treat wrap as newline on next pass
    }
    else
    {
      column++;
    }
    timeoutSet(d);
    prevByte = c;
  }
}

void Adafruit_Thermal::loop()
{
  if(internalBuffer.size())
  {
    if (micros() - resumeTime > 0)
    {
      stream->write(internalBuffer.at(0));
      SERIAL_WRITE(internalBuffer.at(0));
      internalBuffer.erase(internalBuffer.begin());
      lastWritingTime = micros();
    }
  }
  else if(largeReserve)
  {
      // DEBUG_("Resizing capacity to 100\n");
      internalBuffer.shrink_to_fit();
      internalBuffer.reserve(100);
      largeReserve = false;
  }

  if(askedForPaper)
  {
    static uint32_t prevmillis;
    static uint8_t count;

    if (millis() - prevmillis > 100 && count < 10)
    {
      paperStatus = -1;
      if (stream->available())
      {
        paperStatus = stream->read();
        count = 0;
        askedForPaper = false;
        paperResultReady = true;
        paperStatus = !(paperStatus & 0b00000100);
      }
      if(count == 10 && !paperResultReady)
      {
        count=0;
        paperStatus = -1;
        paperResultReady = true;
        askedForPaper = false;
      }

      count++;
      
    }
  }
}


/*!
  @def printDensity
  Printing density, default: 100% (? can go higher, text is darker but fuzzy)
  @def printBreakTime
  Printing break time. Default: 500 uS
*/
void Adafruit_Thermal::begin(uint8_t heatTime) {

  // The printer can't start receiving data immediately upon power up --
  // it needs a moment to cold boot and initialize.  Allow at least 1/2
  // sec of uptime before printer can receive data.
  internalBuffer.reserve(100);  //Reserve 100 bytes for better code performence (Less memory re-allocations)
  wake();
  reset();

  // ESC 7 n1 n2 n3 Setting Control Parameter Command
  // n1 = "max heating dots" 0-255 -- max number of thermal print head
  //      elements that will fire simultaneously.  Units = 8 dots (minus 1).
  //      Printer default is 7 (64 dots, or 1/6 of 384-dot width), this code
  //      sets it to 11 (96 dots, or 1/4 of width).
  // n2 = "heating time" 3-255 -- duration that heating dots are fired.
  //      Units = 10 us.  Printer default is 80 (800 us), this code sets it
  //      to value passed (default 120, or 1.2 ms -- a little longer than
  //      the default because we've increased the max heating dots).
  // n3 = "heating interval" 0-255 -- recovery time between groups of
  //      heating dots on line; possibly a function of power supply.
  //      Units = 10 us.  Printer default is 2 (20 us), this code sets it
  //      to 40 (throttled back due to 2A supply).
  // More heating dots = more peak current, but faster printing speed.
  // More heating time = darker print, but slower printing speed and
  // possibly paper 'stiction'.  More heating interval = clearer print,
  // but slower printing speed.

  writeBytes(ASCII_ESC, '7');   // Esc 7 (print settings)
  writeBytes(11, heatTime, 40); // Heating dots, heat time, heat interval

  // Print density description from manual:
  // DC2 # n Set printing density
  // D4..D0 of n is used to set the printing density.  Density is
  // 50% + 5% * n(D4-D0) printing density.
  // D7..D5 of n is used to set the printing break time.  Break time
  // is n(D7-D5)*250us.
  // (Unsure of the default value for either -- not documented)

#define printDensity   20 // 100% (? can go higher, text is darker but fuzzy)
#define printBreakTime  1 // 500 uS

  //writeBytes(ASCII_DC2, '#', (printBreakTime << 5) | printDensity);

  // Enable DTR pin if requested
  if (dtrPin < 255) {
    pinMode(dtrPin, INPUT_PULLUP);
    writeBytes(ASCII_GS, 'a', (1 << 5));
    dtrEnabled = true;
  }

  dotPrintTime   =  1; // See comments near top of file for   30000  //Fastes print speed
  dotFeedTime    =   1; // an explanation of these values.  2100     //Fastes print speed
  maxChunkHeight =   255;
  
  setDefault();
}

// Reset printer to default state.
void Adafruit_Thermal::reset() {
  writeBytes(ASCII_ESC, '@'); // Init command
  prevByte = '\n';            // Treat as if prior line is blank
  column = 0;
  maxColumn = 32;
  charHeight = 24;
  lineSpacing = 6;
  barcodeHeight = 50;

#if PRINTER_FIRMWARE >= 264
  // Configure tab stops on recent printers
  writeBytes(ASCII_ESC, 'D'); // Set tab stops...
  writeBytes( 4,  8, 12, 16); // ...every 4 columns,
  //writeBytes(20, 24, 28,  0); // 0 marks end-of-list.  //commented out to remove x( of first line
#endif

}

// Reset text formatting parameters.
void Adafruit_Thermal::setDefault() {
  online();
  justify('L');
  inverseOff();
  doubleHeightOff();
  setLineHeight(30);
  boldOff();
  underlineOff();
  setBarcodeHeight(50);
  setSize('s');
  setCharset();
  setCodePage();
}

void Adafruit_Thermal::test() {
  println(F("Hello World!"));
  feed(2);
}

void Adafruit_Thermal::testPage() {
  writeBytes(ASCII_DC2, 'T');
  timeoutSet(dotPrintTime * 24 * 26 + // 26 lines w/text (ea. 24 dots high)
             dotFeedTime *
                 (6 * 26 + 30)); // 26 text lines (feed 6 dots) + blank line
}

void Adafruit_Thermal::setBarcodeHeight(uint8_t val) { // Default is 50
  if (val < 1)
    val = 1;
  barcodeHeight = val;
  writeBytes(ASCII_GS, 'h', val);
}

void Adafruit_Thermal::printBarcode(char *text, uint8_t type) {
  feed(1); // Recent firmware can't print barcode w/o feed first???
  writeBytes(ASCII_GS, 'H', 2);    // Print label below barcode
  writeBytes(ASCII_GS, 'w', 3);    // Barcode width 3 (0.375/1.0mm thin/thick)
  writeBytes(ASCII_GS, 'k', type); // Barcode type (listed in .h file)
#if PRINTER_FIRMWARE >= 264
  int len = strlen(text);
  if (len > 255)
    len = 255;
  writeBytes(len); // Write length byte
  for (uint8_t i = 0; i < len; i++)
    writeBytes(text[i]); // Write string sans NUL
#else
  uint8_t c, i = 0;
  do { // Copy string + NUL terminator
    writeBytes(c = text[i++]);
  } while (c);
#endif
  timeoutSet((barcodeHeight + 40) * dotPrintTime);
  prevByte = '\n';
}

// === Character commands ===

#define INVERSE_MASK                                                           \
  (1 << 1) //!< Turn on/off white/black reverse printing mode. Not in 2.6.8
           //!< firmware (see inverseOn())
#define UPDOWN_MASK (1 << 2)        //!< Turn on/off upside-down printing mode
#define BOLD_MASK (1 << 3)          //!< Turn on/off bold printing mode
#define DOUBLE_HEIGHT_MASK (1 << 4) //!< Turn on/off double-height printing mode
#define DOUBLE_WIDTH_MASK (1 << 5)  //!< Turn on/off double-width printing mode
#define STRIKE_MASK (1 << 6)        //!< Turn on/off deleteline mode

void Adafruit_Thermal::setPrintMode(uint8_t mask) {
  printMode |= mask;
  writePrintMode();
  charHeight = (printMode & DOUBLE_HEIGHT_MASK) ? 48 : 24;
  maxColumn = (printMode & DOUBLE_WIDTH_MASK) ? 16 : 32;
}

void Adafruit_Thermal::unsetPrintMode(uint8_t mask) {
  printMode &= ~mask;
  writePrintMode();
  charHeight = (printMode & DOUBLE_HEIGHT_MASK) ? 48 : 24;
  maxColumn = (printMode & DOUBLE_WIDTH_MASK) ? 16 : 32;
}

void Adafruit_Thermal::writePrintMode() {
  writeBytes(ASCII_ESC, '!', printMode);
}

void Adafruit_Thermal::normal() {
  printMode = 0;
  writePrintMode();
}

void Adafruit_Thermal::inverseOn() {
#if PRINTER_FIRMWARE >= 268
  writeBytes(ASCII_GS, 'B', 1);
#else
  setPrintMode(INVERSE_MASK);
#endif
}

void Adafruit_Thermal::inverseOff() {
#if PRINTER_FIRMWARE >= 268
  writeBytes(ASCII_GS, 'B', 0);
#else
  unsetPrintMode(INVERSE_MASK);
#endif
}

void Adafruit_Thermal::upsideDownOn() { setPrintMode(UPDOWN_MASK); }

void Adafruit_Thermal::upsideDownOff() { unsetPrintMode(UPDOWN_MASK); }

void Adafruit_Thermal::doubleHeightOn() { setPrintMode(DOUBLE_HEIGHT_MASK); }

void Adafruit_Thermal::doubleHeightOff() { unsetPrintMode(DOUBLE_HEIGHT_MASK); }

void Adafruit_Thermal::doubleWidthOn() { setPrintMode(DOUBLE_WIDTH_MASK); }

void Adafruit_Thermal::doubleWidthOff() { unsetPrintMode(DOUBLE_WIDTH_MASK); }

void Adafruit_Thermal::strikeOn() { setPrintMode(STRIKE_MASK); }

void Adafruit_Thermal::strikeOff() { unsetPrintMode(STRIKE_MASK); }

void Adafruit_Thermal::boldOn() { setPrintMode(BOLD_MASK); }

void Adafruit_Thermal::boldOff() { unsetPrintMode(BOLD_MASK); }

void Adafruit_Thermal::justify(char value) {
  uint8_t pos = 0;

  switch (toupper(value)) {
  case 'L':
    pos = 0;
    break;
  case 'C':
    pos = 1;
    break;
  case 'R':
    pos = 2;
    break;
  }

  writeBytes(ASCII_ESC, 'a', pos);
}

// Feeds by the specified number of lines
void Adafruit_Thermal::feed(uint8_t x) {
#if PRINTER_FIRMWARE >= 264
  writeBytes(ASCII_ESC, 'd', x);
  timeoutSet(dotFeedTime * charHeight);
  prevByte = '\n';
  column = 0;
#else
  while (x--)
    write('\n'); // Feed manually; old firmware feeds excess lines
#endif
}

// Feeds by the specified number of individual pixel rows
void Adafruit_Thermal::feedRows(uint8_t rows) {
  writeBytes(ASCII_ESC, 'J', rows);
  timeoutSet(rows * dotFeedTime);
  prevByte = '\n';
  column = 0;
}

void Adafruit_Thermal::flush() { 
    writeBytes(ASCII_FF); 
}

void Adafruit_Thermal::setSize(char value) {
  uint8_t size;

  switch (toupper(value)) {
  default: // Small: standard width and height
    size = 0x00;
    charHeight = 24;
    maxColumn = 32;
    break;
  case 'M': // Medium: double height
    size = 0x01;
    charHeight = 48;
    maxColumn = 32;
    break;
  case 'L': // Large: double width and height
    size = 0x11;
    charHeight = 48;
    maxColumn = 16;
    break;
  }

  writeBytes(ASCII_GS, '!', size);
  prevByte = '\n'; // Setting the size adds a linefeed
}

// Underlines of different weights can be produced:
// 0 - no underline
// 1 - normal underline
// 2 - thick underline
void Adafruit_Thermal::underlineOn(uint8_t weight) {
  if (weight > 2)
    weight = 2;
  writeBytes(ASCII_ESC, '-', weight);
}

void Adafruit_Thermal::underlineOff() { writeBytes(ASCII_ESC, '-', 0); }

int Adafruit_Thermal::printBitmap( int w, int h, const uint8_t *bitmap, bool fromProgMem) {

  int rowBytes, rowBytesClipped, rowStart, chunkHeight, x, y, i;

  rowBytes = (w + 7) / 8; // Round up to next byte boundary
  DEBUG_("h*rowBytes=%d maxFreeBlockSize()=%d, internalBuffer.size()=%d\n",h * rowBytes,AVAILABLE_MEM,internalBuffer.size());
  uint32_t bytesToReserve = h * rowBytes + 8 + internalBuffer.size() + EXCESS_BYTES;
  if (bytesToReserve > AVAILABLE_MEM)
  {
    return 1;
  }
  else
  {

    DEBUG_("Reserving %d Bytes\n",bytesToReserve);
    internalBuffer.reserve(bytesToReserve);
    largeReserve = true;
  }

  DEBUG_("SendingBytes\n");
  writeBytes(ASCII_GS, 'v', '0', 0, rowBytes % 256, rowBytes / 256, h % 256, h / 256);
  i = 0;
  for(y=0; y < h; y++) {
    for(x=0; x < rowBytes; x++, i++) {
      writeBytes(fromProgMem ? pgm_read_byte(bitmap + i) : *(bitmap+i));
    }
  }
  DEBUG_("setting Timeout\n");
  timeoutSet(h * dotPrintTime);
  prevByte = '\n';

  DEBUG_("Returning 0\n");
  return 0;
}

//this is a ridiculous vertical dot print define, not a horizontal raster.
//to print a c header string, you must transpose (rotate and flip) and image first
//also beware of switched height vs width when the image is transposed
int Adafruit_Thermal::defineBitImage( int w, int h, const uint8_t *bitmap, bool fromProgMem) {

  int colBytes, rowBytes, rowBytesClipped, rowStart, chunkHeight, x, y, i;

  rowBytes = (w + 7) / 8; // Round up to next byte boundary
  colBytes = (h + 7) / 8;
  if (h * rowBytes + internalBuffer.size() + EXCESS_BYTES > AVAILABLE_MEM)
  {
    return 1;
  }
  else
  {
    internalBuffer.reserve(h * rowBytes + 4 + internalBuffer.size() + EXCESS_BYTES);
    largeReserve = true;
  }
  writeBytes(0x1D, 0x2A, rowBytes, colBytes);
  i = 0;
  for(y=0; y < h; y++) {
    for(x=0; x < rowBytes; x++, i++) {
      writeBytes(fromProgMem ? pgm_read_byte(bitmap + (i)) : *(bitmap +i));
    }
  }
  timeoutSet(h * dotPrintTime);
  prevByte = '\n';
  return 0;
}

void Adafruit_Thermal::printDefinedBitImage(int mode){
  writeBytes(0x1D, 0x2F, mode);
}

//this is a ridiculous vertical dot print, not a horizontal raster.
//to print a c header string, you must transpose (rotate and flip) and image first
//also beware of switched height vs width when the image is transposed
//n=1 NV images
int Adafruit_Thermal::defineNVBitmap( int w, int h, const uint8_t *bitmap, bool fromProgMem) {
  int colBytes, rowBytes, rowBytesClipped, rowStart, chunkHeight, x, y, i;

  rowBytes = (w / 8); //Round up to next byte boundary for columns, which are transposed rows
  colBytes = (h + 7) / 8;
  DEBUG_("w*colBytes=%d maxFreeBlockSize()=%d, internalBuffer.size()=%d\n",w*colBytes,AVAILABLE_MEM,internalBuffer.size());

  uint32_t bytesToReserve = w*colBytes + 7 + internalBuffer.size() + EXCESS_BYTES;
  if ( bytesToReserve > AVAILABLE_MEM)
  {
    return 1;
  }
  else
  {
    DEBUG_("Reserving %d bytes\n",bytesToReserve);
    internalBuffer.reserve(bytesToReserve);
    largeReserve = true;
  }
  DEBUG_("SendingBytes\n");

  writeBytes(0x1C, 0x71, 1);
  writeBytes(rowBytes % 256, rowBytes / 256, colBytes % 256, colBytes / 256);
  i = 0;
  for(y=0; y < colBytes; y++) {
    for(x=0; x < rowBytes * 8; x++, i++) {
      writeBytes(fromProgMem ? pgm_read_byte(bitmap + i) : *(bitmap + i));
    }
  }
  DEBUG_("Returning 0\n");
  return 0;
}

//n=2 NV images
int Adafruit_Thermal::defineNVBitmap( int w1, int h1, const uint8_t *bitmap1, int w2, int h2, const uint8_t *bitmap2, bool fromProgMem) {
  int colBytes, rowBytes, rowBytesClipped, rowStart, chunkHeight, x, y, i;

  rowBytes = (w1 / 8); //Round up to next byte boundary for columns, which are transposed rows
  colBytes = (h1 +7)/ 8;
  if ((h1 * w1 / 8) + (h2 * w2 / 8) + 11 + internalBuffer.size() + EXCESS_BYTES > AVAILABLE_MEM)
  {
    return 1;
  }
  else
  {
    internalBuffer.reserve((h1 * w1 / 8) + (h2 * w2 / 8) + 11 + internalBuffer.size() + EXCESS_BYTES);
    largeReserve = true;
  }

  //write n=1 image
  writeBytes(0x1C, 0x71, 2);
  writeBytes(rowBytes % 256, rowBytes / 256, colBytes % 256, colBytes / 256);
  i = 0;
  for(y=0; y < colBytes; y++) {
    for(x=0; x < rowBytes * 8; x++, i++) {
      writeBytes(fromProgMem ? pgm_read_byte(bitmap1 + i): *(bitmap1 + i) );
    }
  }

  //write n=2 image
  rowBytes = (w2 / 8);
  colBytes = (h2 + 7) / 8;
  writeBytes(rowBytes % 256, rowBytes / 256, colBytes % 256, colBytes / 256);
  i = 0;
  for(y=0; y < colBytes; y++) {
    for(x=0; x < rowBytes*8; x++, i++) {
      writeBytes(fromProgMem ? pgm_read_byte(bitmap2 + i): *(bitmap2 + i) );
    }
  }

  return 0;
}

void Adafruit_Thermal::printNVBitmap(int n, int mode){
	writeBytes(0x1C, 0x70, n, mode);
}



void Adafruit_Thermal::printBitmap_ada(
 int w, int h, const uint8_t *bitmap, bool fromProgMem) {
  int rowBytes, rowBytesClipped, rowStart, chunkHeight, chunkHeightLimit,
      x, y, i;

  rowBytes = (w + 7) / 8; // Round up to next byte boundary
  rowBytesClipped = (rowBytes >= 48) ? 48 : rowBytes; // 384 pixels max width

  // Est. max rows to write at once, assuming 256 byte printer buffer.
  if (dtrEnabled) {
    chunkHeightLimit = 255; // Buffer doesn't matter, handshake!
  } else {
    chunkHeightLimit = 256 / rowBytesClipped;
    if (chunkHeightLimit > maxChunkHeight)
      chunkHeightLimit = maxChunkHeight;
    else if (chunkHeightLimit < 1)
      chunkHeightLimit = 1;
  }

  for (i = rowStart = 0; rowStart < h; rowStart += chunkHeightLimit) {
    // Issue up to chunkHeightLimit rows at a time:
    chunkHeight = h - rowStart;
    if (chunkHeight > chunkHeightLimit)
      chunkHeight = chunkHeightLimit;

    writeBytes(ASCII_ESC, '*', chunkHeight, rowBytesClipped); //ASCII_DC2

    for (y = 0; y < chunkHeight; y++) {
      for (x = 0; x < rowBytesClipped; x++, i++) {
        timeoutWait();
        stream->write(fromProgMem ? pgm_read_byte(bitmap + i) : *(bitmap + i));
      }
      i += rowBytes - rowBytesClipped;
    }
    timeoutSet(chunkHeight * dotPrintTime);
  }
  prevByte = '\n';
}

void Adafruit_Thermal::printBitmap_ada(int w, int h, Stream *fromStream) {
  int rowBytes, rowBytesClipped, rowStart, chunkHeight, chunkHeightLimit,
      x, y, i, c;

  rowBytes = (w + 7) / 8; // Round up to next byte boundary
  rowBytesClipped = (rowBytes >= 48) ? 48 : rowBytes; // 384 pixels max width

  // Est. max rows to write at once, assuming 256 byte printer buffer.
  if (dtrEnabled) {
    chunkHeightLimit = 255; // Buffer doesn't matter, handshake!
  } else {
    chunkHeightLimit = 256 / rowBytesClipped;
    if (chunkHeightLimit > maxChunkHeight)
      chunkHeightLimit = maxChunkHeight;
    else if (chunkHeightLimit < 1)
      chunkHeightLimit = 1;
  }

  for (rowStart = 0; rowStart < h; rowStart += chunkHeightLimit) {
    // Issue up to chunkHeightLimit rows at a time:
    chunkHeight = h - rowStart;
    if (chunkHeight > chunkHeightLimit)
      chunkHeight = chunkHeightLimit;

    writeBytes(ASCII_ESC, '*', chunkHeight, rowBytesClipped); //ASCII_DC2

    for (y = 0; y < chunkHeight; y++) {
      for (x = 0; x < rowBytesClipped; x++) {
        while ((c = fromStream->read()) < 0)
          ;
        timeoutWait();
        stream->write((uint8_t)c);
      }
      for (i = rowBytes - rowBytesClipped; i > 0; i--) {
        while ((c = fromStream->read()) < 0)
          ;
      }
    }
    timeoutSet(chunkHeight * dotPrintTime);
  }
  prevByte = '\n';
}

void Adafruit_Thermal::printBitmap_ada(Stream *fromStream) {
  uint8_t tmp;
  uint16_t width, height;

  tmp = fromStream->read();
  width = (fromStream->read() << 8) + tmp;

  tmp = fromStream->read();
  height = (fromStream->read() << 8) + tmp;

  printBitmap_ada(width, height, fromStream);
}

// Take the printer offline. Print commands sent after this will be
// ignored until 'online' is called.
void Adafruit_Thermal::offline() { writeBytes(ASCII_ESC, '=', 0); }

// Take the printer back online. Subsequent print commands will be obeyed.
void Adafruit_Thermal::online() { writeBytes(ASCII_ESC, '=', 1); }

// Put the printer into a low-energy state immediately.
void Adafruit_Thermal::sleep() {
  sleepAfter(1); // Can't be 0, that means 'don't sleep'
}

// Put the printer into a low-energy state after the given number
// of seconds.
void Adafruit_Thermal::sleepAfter(uint16_t seconds) {
#if PRINTER_FIRMWARE >= 264
  writeBytes(ASCII_ESC, '8', seconds, seconds >> 8);
#else
  writeBytes(ASCII_ESC, '8', seconds);
#endif
}

// Wake the printer from a low-energy state.
void Adafruit_Thermal::wake() {
  timeoutSet(0);   // Reset timeout counter
  writeBytes(255); // Wake
#if PRINTER_FIRMWARE >= 264
  timeoutSet(50000L);
  writeBytes(ASCII_ESC, '8', 0, 0); // Sleep off (important!)
#else
  // Datasheet recommends a 50 mS delay before issuing further commands,
  // but in practice this alone isn't sufficient (e.g. text size/style
  // commands may still be misinterpreted on wake).  A slightly longer
  // delay, interspersed with NUL chars (no-ops) seems to help.
  for (uint8_t i = 0; i < 10; i++) {
    writeBytes(0);
    timeoutSet(10000L);
  }
#endif
}

// Check the status of the paper using the printer's self reporting
// ability.  Returns 1 when it has paper ,, 0 when has no paper ,, -1 : timeout or short time / No response.
// Might not work on all printers!
int Adafruit_Thermal::hasPaper()
{
  if (paperResultReady)
  {
    return paperStatus;
  }
  else
    return -1;
}
void Adafruit_Thermal::askForPaperAvailability() {
  writeBytes(0x10, 0x04, 4);
  askedForPaper = true;
}


void Adafruit_Thermal::setLineHeight(int val) {
  if (val < 24)
    val = 24;
  lineSpacing = val - 24;

  // The printer doesn't take into account the current text height
  // when setting line height, making this more akin to inter-line
  // spacing.  Default line spacing is 30 (char height of 24, line
  // spacing of 6).
  writeBytes(ASCII_ESC, '3', val);
}

void Adafruit_Thermal::setMaxChunkHeight(int val) { maxChunkHeight = val; }

// These commands work only on printers w/recent firmware ------------------

// Alters some chars in ASCII 0x23-0x7E range; see datasheet
void Adafruit_Thermal::setCharset(uint8_t val) {
  if (val > 15)
    val = 15;
  writeBytes(ASCII_ESC, 'R', val);
}

// Selects alt symbols for 'upper' ASCII values 0x80-0xFF
void Adafruit_Thermal::setCodePage(uint8_t val) {
  if (val > 47)
    val = 47;
  writeBytes(ASCII_ESC, 't', val);
}

void Adafruit_Thermal::tab() {
  writeBytes(ASCII_TAB);
  column = (column + 4) & 0b11111100;
}

void Adafruit_Thermal::setCharSpacing(int spacing) {
  writeBytes(ASCII_ESC, ' ', spacing);
}

// Cut paper.
void Adafruit_Thermal::cut(){
  writeBytes(ASCII_GS, 'V', 0);
}

// Make printer beep
void Adafruit_Thermal::beep(){
  writeBytes(ASCII_ESC, 'o');
  //writeBytes(30);
}

void Adafruit_Thermal::setBeep(int sec) {
	writeBytes(ASCII_GS, 'o', sec);
}

// -------------------------------------------------------------------------

// Standard ESC/POS commands that work only on printers that support these commands

void Adafruit_Thermal::printQRcode(char *text, uint8_t errCorrect, uint8_t moduleSize, uint8_t model) {	//Store data and print QR Code
	
	//Set QR-Code model
	//Range
	// 49 selects Model 1
	// 50 selects Model 2
	// 51 selects Micro QR-Code
	if (model<49 || model>51) model=50 ; // if out of allowed range set model 2 as fallback. Some printers ignore this setting and use e.g. Model 2 in any case!
	writeBytes(ASCII_GS, '(', 'k', 4); 
	writeBytes(0, 49, 65, model);  
	writeBytes(0);
	
	//Module size in pixels (fn=167)
	//Range: 
	//1 <= n <= 16
	if (moduleSize<1 || moduleSize>16) moduleSize=3; // Default = 3 	
	writeBytes(ASCII_GS, '(', 'k');  
	writeBytes(3, 0, 49, 67);  
	writeBytes(moduleSize); 
	
    //Set error correction level fn=169    
	  // 48 selects error correction level L 7%
    // 49 selects error correction level M 15%
    // 50 selects error correction level Q 25%
    // 51 selects error correction level H 30%
	if (errCorrect<48 || errCorrect>51) errCorrect=48; // if incorrect level is specified take level l
	writeBytes(ASCII_GS, '(', 'k');  
	writeBytes(3, 0, 49, 69);  	
	writeBytes(errCorrect); //Default = 48 
	
	//Store the QR Code data in the symbol storage area.  (fn=180) 
	writeBytes(ASCII_GS, '(', 'k');    
	uint16_t len = strlen(text);
		
	writeBytes((uint8_t)((len+3)%256), (uint8_t)((len+3)/256)); //pL , pH -> pL and pH specify the parameter count (pL + pH x 256) in bytes after cn
	writeBytes(49, 80, 48);   
    for(uint16_t i=0; i<len; i++) writeBytes(text[i]); // Write string
	
	reprintQRcode(); // use "reprint" function to print the QR Code (fn=181)
	
}	
 
void Adafruit_Thermal::reprintQRcode() { //Reprint a previously printed QR Code 
	//Print QR code (fn=181) 
	writeBytes(ASCII_GS, '(', 'k', 3);  
	writeBytes(0, 49, 81, 48); 
    //Time needed to print QR-Code.
	
	prevByte = '\n'; /// Treat as if prior line is blank

}
