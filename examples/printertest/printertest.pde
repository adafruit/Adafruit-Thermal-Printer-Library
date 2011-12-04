#include <NewSoftSerial.h>
#include <Thermal.h>

int printer_RX_Pin = 2;  // this is the green wire
int printer_TX_Pin = 3;  // this is the yellow wire

Thermal printer(printer_RX_Pin, printer_TX_Pin);

void setup(){
  Serial.begin(9600);
  
  ////////////////////////////////////////////////////////////////////
  //Following are in setup, but do not need to be. Use them anywhere. 
  //Just here so they do not just keep printing over and over
  //Printer will ignore commands during printing, so use delay(3000)
  //after prints to ensure it see everything you want to print.
  //SOME FUNCTIONS WILL FEED A LINE WHEN CALLED TO SOLIDIFY SETTING
  ////////////////////////////////////////////////////////////////////
 
  // test inverse on & off
  printer.begin();
  /*
  printer.inverseOn();
  printer.print("inverse ON");
  printer.inverseOff();  // this adds a line feed
 

  // test character double width on & off
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
  printer.print("Bolded text");
  printer.boldOff();  // this adds a line feed
  
  printer.underlineThin(); 
  printer.println("Thin underline ");
  printer.underlineThick();
  printer.print("Thick underline ");
  printer.underlineOff();   // adds a linefeed
  printer.println("No underline ");

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
*/
  printer.printBarcode("ADAFRUIT", CODE39); // print a code39, most common alphanumeric barcode
  printer.printBarcode("123456789123", UPC_A); // print UPC line on product barcodes
 
  /*

  printer.setCharSpacing(20);
  printer.println("wide spacing");

  printer.tab();printer.print("Tab");
   printer.tab();
  printer.print("Works As Well");
  /*
  
    //printer.printBarCode("123456789123"); //print simple bar code - up to 12 characters long
  //printer.printFancyBarCode("WWW.BILDR.ORG"); // print fancy barcode. Cap latters and some symbols

  //printer.setBarcodeHeight(50); // set barcode px height: 0-255

  //printer.boldOn(); // Turn bold on
  //printer.boldOff(); //Rurn bold off
  
  
  //printer.sleep(); //Tell printer to sleep. MUST call wake before printing again, even if reset
  //printer.wake(); //Wake printer.
  
  //printer.setSize('L'); // set type size, accepts 'S', 'M', 'L'
  
  //printer.feed(); //advance one line

  //printer.tab(); //Tabs text over 8 spaces
  
  //printer.justify('R'); //sets text justification (left, center, right) accepts 'L', 'C', 'R'
  
  //printer.setDefault(); //set printer to defaults. ****WILL FEED SEVERAL LINES WHEN CALLED***
  */
  
  
}

void loop(){
  
  
}
