#ifndef Thermal_h
#define Thermal_h

#include <NewSoftSerial.h>
#include <WProgram.h>
#include <WConstants.h>

class Thermal{
  public:

    Thermal(int RX_Pin, int TX_Pin);  // constructor
    void test();
    void print(char * text);
    void print(float text, int digits);
    void println(char * text);
    void println(float text, int digits);
	void printBarCode(char * text);
	void printFancyBarCode(char * text);
	void boldOn();
	void boldOff();
	void setLineHeight(int val);
	void sleep();
	void wake();
	void setSize(char value);
	void setBarcodeHeight(int val);
	void feed();
	void tab();
	void justify(char value);
	void doubleHeightOn();
	void doubleHeightOff();
	void inverseOn();
	void inverseOff();
	void setDefault();
	
  private:
	NewSoftSerial * _printer;
	
	int _RX_Pin;
  	int _TX_Pin;
  	
	int heatTime;
	int heatInterval;
	char printDensity;
	char printBreakTime;
};

#endif