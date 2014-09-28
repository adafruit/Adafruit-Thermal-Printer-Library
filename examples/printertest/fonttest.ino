/*
 * Two Font Sizes, regular and compressed, for Adafruit Thermal Printer:
 * http://www.adafruit.com/products/597 and
 * Leigh L. Klotz, Jr <Leigh@WA5ZNU.org>
 */

#include <Process.h>
#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"
#include <avr/pgmspace.h>

int printer_RX_Pin = 5;  // This is the green wire
int printer_TX_Pin = 6;  // This is the yellow wire

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

void testFont(char font, char size, char *msg) {
  printer.setFont(font); 
  printer.setSize(size); 
  printer.println(msg);
  printer.print("maxCol=");
  printer.print(printer.getMaxColumn());
  printer.print(" charHeight=");
  printer.print(printer.getCharHeight());
  printer.print(" charWidth=");
  printer.println(printer.getCharWidth());
  printer.print("<");
  for (uint8_t i = 1; i < printer.getMaxColumn()-2; i++) {
      printer.print('-');
  }
  printer.println(">");
}

void setup() {
  Bridge.begin();
  printer.begin();
  testFont('A', 'S', "Default Font Small");
  testFont('A', 'M', "Default Font Medium");
  testFont('A', 'L', "Default Font Large");
  testFont('B', 'S', "Compressed Font Small");
  testFont('B', 'M', "Compressed Font Medium");
  testFont('B', 'L', "Compressed Font Large");
}

void loop() {
  // Do nothing here.
}
