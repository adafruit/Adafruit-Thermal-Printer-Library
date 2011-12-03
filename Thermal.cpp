#include <WProgram.h>
#include <WConstants.h>
#include <Thermal.h>

Thermal::Thermal(int RX_Pin, int TX_Pin){

	_RX_Pin = RX_Pin;
	_TX_Pin = TX_Pin;
	
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
	
	//set to normal font size
	_printer->print(29, BYTE);
	_printer->print(33, BYTE);
	_printer->print(0, BYTE);
	_printer->print(10, BYTE);
	
}

void Thermal::test(){
	_printer->println("Hello World!");
	_printer->print(10, BYTE); //Sends the LF to the printer, advances the paper
	_printer->print(10, BYTE);
}


void Thermal::print(char * text){
	_printer->print(text);
	delay(10);
}


void Thermal::print(float text, int digits){
	_printer->print(text, digits);
	delay(10);
}


void Thermal::println(char * text){
	_printer->println(text);
	delay(10);
}

void Thermal::println(float text, int digits){
	_printer->println(text, digits);
	delay(10);
}


void Thermal::setBarcodeHeight(int val){
	//default is 50
	_printer->print(29, BYTE);
	_printer->print(104, BYTE);
	_printer->print(val, BYTE); 
}

void Thermal::printBarCode(char * text){
	_printer->print(29, BYTE); //GS
	_printer->print(107, BYTE); //k
	_printer->print(0, BYTE); //m = 0
	
	for(int i = 0; i < strlen(text); i ++){
		_printer->print(text[i], BYTE); //Data
	}
	_printer->print(0, BYTE); //Terminator
	
	delay(3000); //For some reason we can't immediately have line feeds here
	_printer->print(10, BYTE); //Paper feed
	_printer->print(10, BYTE); //Paper feed

}

void Thermal::printFancyBarCode(char * text){
	_printer->print(29, BYTE); //GS
	_printer->print(107, BYTE); //k
	_printer->print(4, BYTE); //Fancy!
	
	for(int i = 0; i < strlen(text); i ++){
		_printer->print(text[i], BYTE); //Data
	}
 
	_printer->print(0, BYTE); //Terminator
	
	delay(3000); //For some reason we can't immediately have line feeds here
	_printer->print(10, BYTE); //Paper feed
	_printer->print(10, BYTE); //Paper feed

}

void Thermal::boldOn(){
	_printer->print(27, BYTE);
	_printer->print(32, BYTE);
	_printer->print(1, BYTE);
	_printer->print(10, BYTE);
}

void Thermal::boldOff(){
	_printer->print(27, BYTE);
	_printer->print(32, BYTE);
	_printer->print(0, BYTE);
	_printer->print(10, BYTE);
}


void Thermal::setSize(char value){
	int size = 0;
	
	if(value == 's' || value == 'S') size = 0;
	if(value == 'm' || value == 'M') size = 10;
	if(value == 'l' || value == 'L') size = 25;
	_printer->print(29, BYTE);
	_printer->print(33, BYTE);
	_printer->print(size, BYTE);
	_printer->print(10, BYTE);
}


void Thermal::setLineHeight(int val){
	_printer->print(27, BYTE);
	_printer->print(55, BYTE);
	_printer->print(val, BYTE); //Default 64 dots = 8*('7'+1)
}





void Thermal::wake(){
	_printer->print(27, BYTE);
	_printer->print(61, BYTE);
	_printer->print(1, BYTE);
}


void Thermal::sleep(){
	_printer->print(27, BYTE);
	_printer->print(61, BYTE);
	_printer->print(0, BYTE);
}

void Thermal::tab(){
	_printer->print(9, BYTE);
}

void Thermal::doubleHeightOn(){
	_printer->print(27, BYTE);
	_printer->print(14, BYTE);
}


void Thermal::doubleHeightOff(){
	_printer->print(27, BYTE);
	_printer->print(20, BYTE);
}

void Thermal::inverseOn(){
	_printer->print(29, BYTE);
	_printer->print(66, BYTE);
	_printer->print(1, BYTE);
	_printer->print(10, BYTE);
}


void Thermal::inverseOff(){
	_printer->print(29, BYTE);
	_printer->print(66, BYTE);
	_printer->print(0, BYTE);
	_printer->print(10, BYTE);
}


void Thermal::justify(char value){
	int pos = 0;
	
	if(value == 'l' || value == 'L') pos = 0;
	if(value == 'c' || value == 'C') pos = 1;
	if(value == 'r' || value == 'R') pos = 2;


	_printer->print(0x1B, BYTE);
	_printer->print(0x61, BYTE);
	_printer->print(pos, BYTE);
}

void Thermal::setDefault(){
	wake();
	justify('L');
	inverseOff();
	doubleHeightOff();
	boldOff();
	setBarcodeHeight(50);
	setSize('s');
}

void Thermal::feed(){
	_printer->print(10, BYTE);
}


void *operator new(size_t size_) { return malloc(size_); }
void* operator new(size_t size_,void *ptr_) { return ptr_; }