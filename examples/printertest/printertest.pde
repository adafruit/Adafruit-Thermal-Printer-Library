/*************************************************** 
  This is an example sketch for the Adafruit Thermal Printer
  
  Pick one up at --> http://www.adafruit.com/products/597
  These printers use TTL serial to communicate, 2 pins are required

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  MIT license, all text above must be included in any redistribution
 ****************************************************/


// if you're using Arduino 1.0 uncomment the next line
#include "SoftwareSerial.h"
// if you're using Arduino 23 or earlier, uncomment the next line
//#include "NewSoftSerial.h"

#include "Adafruit_Thermal.h"
#include "adalogo.cpp"
#include "adaqrcode.cpp"

int printer_RX_Pin = 5;  // this is the green wire
int printer_TX_Pin = 6;  // this is the yellow wire

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);


void setup(){
  Serial.begin(9600);
  printer.begin();
  
  ////////////////////////////////////////////////////////////////////
  //Following are in setup, but do not need to be. Use them anywhere. 
  //Just here so they do not just keep printing over and over
  //Printer will ignore commands during printing, so use delay(3000)
  //after prints to ensure it see everything you want to print.
  //SOME FUNCTIONS WILL FEED A LINE WHEN CALLED TO SOLIDIFY SETTING
  ////////////////////////////////////////////////////////////////////
 
  // test inverse on & off
  printer.inverseOn();
  printer.println("Inverse ON");
  printer.inverseOff();

  // test character double height on & off
  printer.doubleHeightOn();
  printer.println("Double Height ON");
  printer.doubleHeightOff();
 
  //sets text justification (right, left, center) accepts 'L', 'C', 'R'
  printer.justify('R'); 
  printer.println("Right justified");
  printer.justify('C'); 
  printer.println("Center justified");
  printer.justify('L');  
  printer.println("Left justified");
  
  printer.boldOn();
  printer.println("Bold text");
  printer.boldOff();
  
  printer.underlineOn(); 
  printer.println("Underlined text ");
  printer.underlineOff();

  printer.setSize('L');      // set type size, accepts 'S', 'M', 'L'
  printer.print("Large");    // print line
  printer.setSize('M');      // setting the size adds a linefeed 
  printer.print("Medium");    // print line
  printer.setSize('S');      // setting the size adds a linefeed
  printer.println("Small");    // print line
  
  printer.justify('C'); 
  printer.println("normal\nline\nspacing");
  printer.setLineHeight(50);
  printer.println("Taller\nline\nspacing");
  printer.setLineHeight();    // reset to default
  printer.justify('L');  

  printer.feed(1);
  printer.printBarcode("ADAFRUIT", CODE39); // print a code39, most common alphanumeric barcode
  printer.setBarcodeHeight(100);
  printer.printBarcode("123456789123", UPC_A); // print UPC line on product barcodes

  // print the 57x57 pixel logo included in adalogo.cpp
  printer.printBitmap(57, 57, adalogo);
  
  // print the 135 x 135 pixel QR code in adaqrcode.cpp
  printer.printBitmap(135, 135, adaqr);
  printer.println("Adafruit!");
  printer.feed(1);

  printer.sleep(); //Tell printer to sleep. MUST call wake before printing again, even if reset
  printer.wake(); //Wake printer.
  printer.setDefault(); //restore printer to defaults
}

void loop(){
}
