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


#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
 #include "WConstants.h"
#endif
#include "Adafruit_Thermal.h"
#include <avr/pgmspace.h>

Adafruit_Thermal::Adafruit_Thermal(int RX_Pin, int TX_Pin) {
  _RX_Pin = RX_Pin;
  _TX_Pin = TX_Pin;
}

void Adafruit_Thermal::begin() {
  begin(150);
}

// heatTime - 80 is default from page 23 of datasheet. Controls speed of printing and darkness
void Adafruit_Thermal::begin(int heatTime) {
  _printer = new SERIAL_IMPL(_RX_Pin, _TX_Pin);
  _printer->begin(19200);

  // The printer can't start receiving data immediately
  // upon power up -- needs a moment to initialize.  If
  // Arduino & printer are powered from the same supply,
  // they're starting simultaneously.  Need to pause for
  // a moment so the printer is ready for commands.
  // (A more robust approach might be to wait in a loop
  // issuing status commands until valid response.)
  delay(500);

  reset();

  heatInterval = 50; //2 is default from page 23 of datasheet. Controls speed of printing and darkness
  printDensity = 15; //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
  printBreakTime = 15; //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.

  writeBytes(27, 55);
  writeBytes(7); //Default 64 dots = 8*('7'+1)
  writeBytes(heatTime); //Default 80 or 800us
  writeBytes(heatInterval); //Default 2 or 20us

  //Modify the print density and timeout
  writeBytes(18, 35);
  int printSetting = (printDensity<<4) | printBreakTime;
  writeBytes(printSetting); //Combination of printDensity and printBreakTime
}

// reset printer
void Adafruit_Thermal::reset() {
  writeBytes(27, 64);
}

// reset formatting
void Adafruit_Thermal::setDefault(){
  online();
  justify('L');
  inverseOff();
  doubleHeightOff();
  setLineHeight(32);
  boldOff();
  underlineOff();
  setBarcodeHeight(50);
  setSize('s');
}

void Adafruit_Thermal::test(){
  println("Hello World!");
  feed(2);
}

void Adafruit_Thermal::testPage() {
  writeBytes(18, 84);
}

// this is the basic function for all printing, the rest is taken care of by the
// inherited Print class!
#if ARDUINO >= 100
size_t Adafruit_Thermal::write(uint8_t c) {
  if (c == 0x13) return 0;
#else
void Adafruit_Thermal::write(uint8_t c) {
  if (c == 0x13) return;
#endif
  if (c != 0xA)
    linefeedneeded = true;
  else
    linefeedneeded = false;

  Serial.print(" 0x");
  Serial.print(c, HEX);
  Serial.print(" ("); 
#if ARDUINO >= 100
  Serial.write(c);
#else
  Serial.print(" (");
#endif
  Serial.println(")");
  PRINTER_PRINT(c);
  delay(1);

#if ARDUINO >= 100
  return 1;
#endif
}

void Adafruit_Thermal::setBarcodeHeight(int val){
  //default is 50
  writeBytes(29, 104, val);
}

void Adafruit_Thermal::printBarcode(char * text, uint8_t type) {
  int i;
  byte c;
  writeBytes(29, 72,2); //Set that numbers are printed below the Barcode. (0=not printed,  1= above, 2=below, 3=both)
  //delay(1000); //You don't need these delays...
  writeBytes(29, 119, 3); //Set the hoizontal size of the barcode (2 = small, 3 = big), 3 is default
  writeBytes(29, 107, type); // set the type first
  
  //delay(500); //NO! ! ! The Barcode won't print with this delay!
  // Copy string, not including NUL terminator
  for(i=0; (c = text[i]); i++) writeBytes(c);
  //delay(500); //Isn't needed !
  writeBytes(0); //Just send 0 to tell the printer the data is over

  delay(3000); //For some reason we can't immediately have line feeds here
  feed(2);
}

void Adafruit_Thermal::writeBytes(uint8_t a) {
  PRINTER_PRINT(a);
}

void Adafruit_Thermal::writeBytes(uint8_t a, uint8_t b) {
  PRINTER_PRINT(a);
  PRINTER_PRINT(b);
}

void Adafruit_Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c) {
  PRINTER_PRINT(a);
  PRINTER_PRINT(b);
  PRINTER_PRINT(c);
}

void Adafruit_Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  PRINTER_PRINT(a);
  PRINTER_PRINT(b);
  PRINTER_PRINT(c);
  PRINTER_PRINT(d);
}

// === Character commands ===

#define INVERSE_MASK (1 << 1)
#define UPDOWN_MASK (1 << 2)
#define BOLD_MASK (1 << 3)
#define DOUBLE_HEIGHT_MASK (1 << 4)
#define DOUBLE_WIDTH_MASK (1 << 5)
#define STRIKE_MASK (1 << 6)

void Adafruit_Thermal::setPrintMode(uint8_t mask) {
  printMode |= mask;
  writePrintMode();
}
void Adafruit_Thermal::unsetPrintMode(uint8_t mask) {
  printMode &= ~mask;
  writePrintMode();
}

void Adafruit_Thermal::writePrintMode() {
  writeBytes(27, 33, printMode);
}

void Adafruit_Thermal::normal() {
  printMode = 0;
  writePrintMode();
}

void Adafruit_Thermal::inverseOn(){
  setPrintMode(INVERSE_MASK);
}

void Adafruit_Thermal::inverseOff(){
  unsetPrintMode(INVERSE_MASK);
}

void Adafruit_Thermal::upsideDownOn(){
  setPrintMode(UPDOWN_MASK);
}

void Adafruit_Thermal::upsideDownOff(){
  unsetPrintMode(UPDOWN_MASK);
}

void Adafruit_Thermal::doubleHeightOn(){
  setPrintMode(DOUBLE_HEIGHT_MASK);
}

void Adafruit_Thermal::doubleHeightOff(){
  unsetPrintMode(DOUBLE_HEIGHT_MASK);
}

void Adafruit_Thermal::doubleWidthOn(){
  setPrintMode(DOUBLE_WIDTH_MASK);
}

void Adafruit_Thermal::doubleWidthOff(){
  unsetPrintMode(DOUBLE_WIDTH_MASK);
}

void Adafruit_Thermal::strikeOn(){
  setPrintMode(STRIKE_MASK);
}

void Adafruit_Thermal::strikeOff(){
  unsetPrintMode(STRIKE_MASK);
}

void Adafruit_Thermal::boldOn(){
  setPrintMode(BOLD_MASK);
}

void Adafruit_Thermal::boldOff(){
  unsetPrintMode(BOLD_MASK);
}

void Adafruit_Thermal::justify(char value){
  uint8_t pos = 0;

  if(value == 'l' || value == 'L') pos = 0;
  if(value == 'c' || value == 'C') pos = 1;
  if(value == 'r' || value == 'R') pos = 2;

  writeBytes(0x1B, 0x61, pos);
}

// Feeds by the specified number of lines
void Adafruit_Thermal::feed(uint8_t x){
  // The datasheet claims sending bytes 27, 100, <x> will work
  // but it feeds much much more.
  while (x--)
    write('\n');
}

// Feeds by the specified number of rows of pixels
void Adafruit_Thermal::feedRows(uint8_t rows) {
  writeBytes(27, 74, rows);
}

void Adafruit_Thermal::flush() {
  writeBytes(12);
}

void Adafruit_Thermal::setSize(char value){
  int size = 0;

  if(value == 's' || value == 'S') size = 0;
  if(value == 'm' || value == 'M') size = 10;
  if(value == 'l' || value == 'L') size = 25;

  writeBytes(29, 33, size, 10);
  // if (linefeedneeded)
  //  println("lfn"); //feed();
  //linefeedneeded = false;
}

// Underlines of different weights can be produced:
// 0 - no underline
// 1 - normal underline
// 2 - thick underline
void Adafruit_Thermal::underlineOn(uint8_t weight) {
  writeBytes(27, 45, weight);
}

void Adafruit_Thermal::underlineOff() {
  underlineOn(0);
}

void Adafruit_Thermal::printBitmap(int w, int h, const uint8_t *bitmap) {
  if (w > 384) return; // maximum width of the printer
  for (int rowStart=0; rowStart < h; rowStart += 256) {
    int chunkHeight = ((h - rowStart) > 255) ? 255 : (h - rowStart);
    delay(500); // Need these delays else bitmap doesn't always print. ???
    writeBytes(18, 42);
    writeBytes(chunkHeight, w/8);
    delay(500);
    for (int i=0; i<((w/8)*chunkHeight); i++) {
      PRINTER_PRINT(pgm_read_byte(bitmap + (rowStart*(w/8)) + i));
    }
    delay(500);
  }
}

void Adafruit_Thermal::printBitmap(int w, int h, Stream *stream) {
  if (w > 384) return; // maximum width of the printer
  for (int rowStart=0; rowStart < h; rowStart += 256) {
    int chunkHeight = ((h - rowStart) > 255) ? 255 : (h - rowStart);
    delay(500); // Need these delays else bitmap doesn't always print. ???
    writeBytes(18, 42);
    writeBytes(chunkHeight, w/8);
    delay(500);
    for (int i=0; i<((w/8)*chunkHeight); i++) {
      PRINTER_PRINT((uint8_t)stream->read());
    }
    delay(500);
  }
};

void Adafruit_Thermal::printBitmap(Stream *stream) {
  uint8_t tmp;
  uint16_t width, height;

  tmp = stream->read();
  width = (stream->read() << 8) + tmp;

  tmp = stream->read();
  height = (stream->read() << 8) + tmp;

  printBitmap(width, height, stream);
};

// Take the printer offline. Print commands sent after this will be
// ignored until `online` is called
void Adafruit_Thermal::offline(){
  writeBytes(27, 61, 0);
}

// Take the printer back online. Subsequent print commands will be
// obeyed.
void Adafruit_Thermal::online(){
  writeBytes(27, 61, 1);
}

// Put the printer into a low-energy state immediately
void Adafruit_Thermal::sleep() {
  sleepAfter(0);
}

// Put the printer into a low-energy state after the given number
// of seconds
void Adafruit_Thermal::sleepAfter(uint8_t seconds) {
  writeBytes(27, 56, seconds);
}

// Wake the printer from a low-energy state. This command will wait
// for 50ms (as directed by the datasheet) before allowing further
// commands to be send.
void Adafruit_Thermal::wake() {
  writeBytes(255);
  delay(50);
}

////////////////////// not working?
void Adafruit_Thermal::tab(){
  PRINTER_PRINT(9);
}
void Adafruit_Thermal::setCharSpacing(int spacing) {
  writeBytes(27, 32, 0, 10);
}
void Adafruit_Thermal::setLineHeight(int val){
  writeBytes(27, 51, val); // default is 32
}
/////////////////////////

#if ARDUINO < 100
void *operator new(size_t size_) { return malloc(size_); }
void* operator new(size_t size_,void *ptr_) { return ptr_; }
#endif
