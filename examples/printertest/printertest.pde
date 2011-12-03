#include <NewSoftSerial.h>
#include <Thermal.h>

int printer_RX_Pin = 2;
int printer_TX_Pin = 3;

Thermal printer(printer_RX_Pin, printer_TX_Pin);

void setup(){
  ////////////////////////////////////////////////////////////////////
  //Following are in setup, but do not need to be. Use them anywhere. 
  //Just here so they do not just keep printing over and over
  //Printer will ignore commands during printing, so use delay(3000)
  //after prints to ensure it see everything you want to print.
  //SOME FUNCTIONS WILL FEED A LINE WHEN CALLED TO SOLIDIFY SETTING
  ////////////////////////////////////////////////////////////////////
 
  printer.justify('R'); //sets text justification (right, left, center) accepts 'L', 'C', 'R'
  
  printer.setSize('L'); // set type size, accepts 'S', 'M', 'L'
  printer.println("hello"); //print line
  
  printer.setSize('M'); // set type size, accepts 'S', 'M', 'L'
  printer.println("hello"); //print line
  
  printer.setSize('S'); // set type size, accepts 'S', 'M', 'L'
  printer.println("hello"); //print line
  
  printer.feed(); //advance one line
  printer.feed(); //advance one line
  
  
  
  
  //printer.print("hello"); //add text to print line without printing it. Call println to print.
  //printer.print(1234);
  
  //printer.println("hello"); //print line
  //printer.println(1234);
  
  //printer.printBarCode("123456789123"); //print simple bar code - up to 12 characters long
  //printer.printFancyBarCode("WWW.BILDR.ORG"); // print fancy barcode. Cap latters and some symbols

  //printer.setBarcodeHeight(50); // set barcode px height: 0-255

  //printer.boldOn(); // Turn bold on
  //printer.boldOff(); //Rurn bold off
  
  //printer.doubleHeightOn(); // sets type to print double hight
  //printer.doubleHeightOff(); //Turn off double hight printing
  
  //printer.inverseOn(); //set to print white on black
  //printer.inverseOff(); //set to print black on white (default)
  
  //printer.sleep(); //Tell printer to sleep. MUST call wake before printing again, even if reset
  //printer.wake(); //Wake printer.
  
  //printer.setSize('L'); // set type size, accepts 'S', 'M', 'L'
  
  //printer.feed(); //advance one line

  //printer.tab(); //Tabs text over 8 spaces
  
  //printer.justify('R'); //sets text justification (left, center, right) accepts 'L', 'C', 'R'
  
  //printer.setDefault(); //set printer to defaults. ****WILL FEED SEVERAL LINES WHEN CALLED***
  
  
  
}

void loop(){
  
  
}