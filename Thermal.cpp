#include <WProgram.h>
#include <WConstants.h>
#include <Thermal.h>

Thermal::Thermal(int RX_Pin, int TX_Pin) {
  _RX_Pin = RX_Pin;
  _TX_Pin = TX_Pin;

}

void Thermal::begin() {
  _printer = new NewSoftSerial (_RX_Pin, _TX_Pin);
  _printer->begin(19200);
  
  heatTime = 120; //80 is default from page 23 of datasheet. Controls speed of printing and darkness
  heatInterval = 50; //2 is default from page 23 of datasheet. Controls speed of printing and darkness
  printDensity = 15; //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
  printBreakTime = 15; //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
  
  
  _printer->print(27, BYTE);
  _printer->print(55, BYTE);
  _printer->print(7, BYTE); //Default 64 dots = 8*('7'+1)
  _printer->print(heatTime, BYTE); //Default 80 or 800us
  _printer->print(heatInterval, BYTE); //Default 2 or 20us
  
  
  //Modify the print density and timeout
  _printer->print(18, BYTE);
  _printer->print(35, BYTE);
  
  int printSetting = (printDensity<<4) | printBreakTime;
  _printer->print(printSetting, BYTE); //Combination of printDensity and printBreakTime
  
  setDefault();
}

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
void Thermal::write(uint8_t c) {
  if (c == 0x13) return;
  if (c != 0xA)  
    linefeedneeded = true;
  else
    linefeedneeded = false;

  Serial.print(" 0x");
  Serial.print(c, HEX);
  Serial.print(" ("); Serial.print(c, BYTE); Serial.println(")");

  _printer->print(c);
  delay(1);
}

void Thermal::setBarcodeHeight(int val){
	//default is 50
	_printer->print(29, BYTE);
	_printer->print(104, BYTE);
	_printer->print(val, BYTE); 
}

void Thermal::printBarcode(char * text, uint8_t type) {
  writeBytes(29, 107, type); // set the type first
  for(int i = 0; i < strlen(text); i ++){
    write(text[i]); //Data
  }
  write(0); //Terminator
	
  delay(3000); //For some reason we can't immediately have line feeds here
  feed(2);
}


void Thermal::writeBytes(uint8_t a, uint8_t b) {
  _printer->print(a, BYTE);
  _printer->print(b, BYTE);
}

void Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c) {
  _printer->print(a, BYTE);
  _printer->print(b, BYTE);
  _printer->print(c, BYTE);
}

void Thermal::writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  _printer->print(a, BYTE);
  _printer->print(b, BYTE);
  _printer->print(c, BYTE);
  _printer->print(d, BYTE);
}

void Thermal::inverseOn(){
  writeBytes(29, 'B', 1);
}

void Thermal::inverseOff(){
  writeBytes(29, 'B', 0, 10);
}

void Thermal::doubleHeightOn(){
  writeBytes(27, 14);
}

void Thermal::doubleHeightOff(){
  writeBytes(27, 20);
}


void Thermal::boldOn(){
  writeBytes(27, 69, 1);
}

void Thermal::boldOff(){
  writeBytes(27, 69, 0);
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
void Thermal::underlineThin() {
  writeBytes(27, 45, 1);
}
void Thermal::underlineThick() {
  writeBytes(27, 45, 2);
}

void Thermal::wake(){
  writeBytes(27, 61, 1);
}

void Thermal::sleep(){
  writeBytes(27, 61, 0);
}

////////////////////// not working?
void Thermal::tab(){
  _printer->print(9, BYTE);
}

void Thermal::setCharSpacing(int spacing) {
  writeBytes(27, 32, 0, 10);
}


void Thermal::setLineHeight(int val){
  writeBytes(27, 51, val); // default is 32
}

void *operator new(size_t size_) { return malloc(size_); }
void* operator new(size_t size_,void *ptr_) { return ptr_; }
