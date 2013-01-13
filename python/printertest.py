#!/usr/bin/python

from Adafruit_Thermal import *

printer = Adafruit_Thermal("/dev/ttyAMA0", 19200, timeout=5)

# Test inverse on & off
printer.inverseOn()
printer.println("Inverse ON")
printer.inverseOff()

# Test character double-height on & off
printer.doubleHeightOn()
printer.println("Double Height ON")
printer.doubleHeightOff()

# Set justification (right, center, left) -- accepts 'L', 'C', 'R'
printer.justify('R')
printer.println("Right justified")
printer.justify('C')
printer.println("Center justified")
printer.justify('L')
printer.println("Left justified")

# Test more styles
printer.boldOn()
printer.println("Bold text")
printer.boldOff()

printer.underlineOn()
printer.println("Underlined text")
printer.underlineOff()

printer.setSize('L')   # Set type size, accepts 'S', 'M', 'L'
printer.println("Large")
printer.setSize('M')
printer.println("Medium")
printer.setSize('S')
printer.println("Small")

printer.justify('C')
printer.println("normal\nline\nspacing")
printer.setLineHeight(50)
printer.println("Taller\nline\nspacing")
printer.setLineHeight() # Reset to default
printer.justify('L')

# Barcode examples
printer.feed(1)
# CODE39 is the most common alphanumeric barcode
printer.printBarcode("ADAFRUT", printer.CODE39)
printer.setBarcodeHeight(100)
# Print UPC line on product barcodes
printer.printBarcode("123456789123", printer.UPC_A)

# Print the 75x75 pixel logo in adalogo.py
import adalogo
printer.printBitmap(adalogo.width, adalogo.height, adalogo.data)

# Print the 135x135 pixel QR code in adaqrcode.py
import adaqrcode
printer.printBitmap(adaqrcode.width, adaqrcode.height, adaqrcode.data)
printer.println("Adafruit!")
printer.feed(1)

printer.sleep()      # Tell printer to sleep
printer.wake()       # Call wake() before printing again, even if reset
printer.setDefault() # Restore printer to defaults

