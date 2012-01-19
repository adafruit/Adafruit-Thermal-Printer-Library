#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
 #include "WConstants.h"
#endif
#include "Thermal.h"
#include <avr/pgmspace.h>

Thermal::Thermal(int RX_Pin, int TX_Pin) {
  _RX_Pin = RX_Pin;
  _TX_Pin = TX_Pin;
}

void Thermal::begin() {
  _printer = new SERIAL_IMPL(_RX_Pin, _TX_Pin);
  _printer->begin(19200);

  reset();

  heatTime = 120; //80 is default from page 23 of datasheet. Controls speed of printing and darkness
  heatInterval = 50; //2 is default from page 23 of datasheet. Controls speed of printing and darkness
  printDensity = 15; //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
  printBreakTime = 15; //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.

  PRINTER_PRINT(27);
  PRINTER_PRINT(55);
  PRINTER_PRINT(7); //Default 64 dots = 8*('7'+1)
  PRINTER_PRINT(heatTime); //Default 80 or 800us
  PRINTER_PRINT(heatInterval); //Default 2 or 20us

  //Modify the print density and timeout
  PRINTER_PRINT(18);
  PRINTER_PRINT(35);

  int printSetting = (printDensity<<4) | printBreakTime;
  PRINTER_PRINT(printSetting); //Combination of printDensity and printBreakTime
}

// reset printer
void Thermal::reset() {
  writeBytes(27, 64);
}

// reset formatting
void Thermal::setDefault(){
  wake();
  justify('L');
  inverseOff();
  doubleHeightOff();
  setLineHeight(32);
  boldOff();
  underlineOff();
  setBarcodeHeight(50);
  setSize('s');
}

void Thermal::test(){
  println("Hello World!");
  feed(2);
}

// this is the basic function for all printing, the rest is taken care of by the
// inherited Print class!
#if ARDUINO >= 100
size_t Thermal::write(uint8_t c) {
  if (c == 0x13) return 0;
#else
void Thermal::write(uint8_t c) {
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

void Thermal::setBarcodeHeight(int val){
  //default is 50
  writeBytes(29, 104, val);
}

void Thermal::printBarcode(char * text, uint8_t type) {
  writeBytes(29, 107, type); // set the type first
  for(uint16_t i = 0; i < strlen(text); i ++){
    write(text[i]); //Data
  }
  write(0); //Terminator

  delay(3000); //For some reason we can't immediately have line feeds here
  feed(2);
}

void Thermal::writeBytes(uint8_t a) {
  PRINTER_PRINT(a);
}

void Thermal::writeBytes(uint8_t a, uint8_t b) {
  PRINTER_PRINT(a);
  PRINTER_PRINT(b);
}

void Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c) {
  PRINTER_PRINT(a);
  PRINTER_PRINT(b);
  PRINTER_PRINT(c);
}

void Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
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

void Thermal::setPrintMode(uint8_t mask) {
  printMode |= mask;
  writePrintMode();
}
void Thermal::unsetPrintMode(uint8_t mask) {
  printMode &= ~mask;
  writePrintMode();
}

void Thermal::writePrintMode() {
  writeBytes(27, 33, printMode);
}

void Thermal::normal() {
  printMode = 0;
  writePrintMode();
}

void Thermal::inverseOn(){
  setPrintMode(INVERSE_MASK);
}

void Thermal::inverseOff(){
  unsetPrintMode(INVERSE_MASK);
}

void Thermal::upsideDownOn(){
  setPrintMode(UPDOWN_MASK);
}

void Thermal::upsideDownOff(){
  unsetPrintMode(UPDOWN_MASK);
}

void Thermal::doubleHeightOn(){
  setPrintMode(DOUBLE_HEIGHT_MASK);
}

void Thermal::doubleHeightOff(){
  unsetPrintMode(DOUBLE_HEIGHT_MASK);
}

void Thermal::doubleWidthOn(){
  setPrintMode(DOUBLE_WIDTH_MASK);
}

void Thermal::doubleWidthOff(){
  unsetPrintMode(DOUBLE_WIDTH_MASK);
}

void Thermal::strikeOn(){
  setPrintMode(STRIKE_MASK);
}

void Thermal::strikeOff(){
  unsetPrintMode(STRIKE_MASK);
}

void Thermal::boldOn(){
  setPrintMode(BOLD_MASK);
}

void Thermal::boldOff(){
  unsetPrintMode(BOLD_MASK);
  if (linefeedneeded)
    feed();
  linefeedneeded = false;
}

void Thermal::justify(char value){
  uint8_t pos = 0;

  if(value == 'l' || value == 'L') pos = 0;
  if(value == 'c' || value == 'C') pos = 1;
  if(value == 'r' || value == 'R') pos = 2;

  writeBytes(0x1B, 0x61, pos);
}

void Thermal::feed(uint8_t x){
  while (x--)
    write('\n');
}

void Thermal::flush() {
  writeBytes(12);
}

void Thermal::setSize(char value){
  int size = 0;

  if(value == 's' || value == 'S') size = 0;
  if(value == 'm' || value == 'M') size = 10;
  if(value == 'l' || value == 'L') size = 25;

  writeBytes(29, 33, size, 10);
  // if (linefeedneeded)
  //  println("lfn"); //feed();
  //linefeedneeded = false;
}

void Thermal::underlineOff() {
  writeBytes(27, 45, 0, 10);
}
void Thermal::underlineOn() {
  writeBytes(27, 45, 1);
}

void Thermal::printBitmap(int w, int h, const uint8_t *bitmap) {
  if (w > 384) return; // maximum width of the printer
  for (int rowStart=0; rowStart < h; rowStart += 256) {
    int chunkHeight = ((h - rowStart) > 255) ? 255 : (h - rowStart);
    printBitmapChunk(w, chunkHeight, rowStart*w, bitmap);
  }
}

void Thermal::printBitmapChunk(int w, uint8_t h, int offset, const uint8_t *bitmap) {
  writeBytes(18, 42);
  writeBytes(h, w/8);
  for (int i=0; i<((w/8)*h); i++) {
    PRINTER_PRINT(pgm_read_byte(bitmap + offset + i));
  }
}

void Thermal::wake(){
  writeBytes(27, 61, 1);
}

void Thermal::sleep(){
  writeBytes(27, 61, 0);
}

////////////////////// not working?
void Thermal::tab(){
  PRINTER_PRINT(9);
}
void Thermal::setCharSpacing(int spacing) {
  writeBytes(27, 32, 0, 10);
}
void Thermal::setLineHeight(int val){
  writeBytes(27, 51, val); // default is 32
}
/////////////////////////

#if ARDUINO < 100
void *operator new(size_t size_) { return malloc(size_); }
void* operator new(size_t size_,void *ptr_) { return ptr_; }
#endif
