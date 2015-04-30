// THIS IS NOT ARDUINO CODE!  Runs in Processing IDE (www.processing.org).
// Convert image to C header file suitable for the Adafruit_Thermal library.

void setup() {
  // Select and load image
  selectInput("Select image file to convert:", "processImage");
}

void processImage(File image) {
  String      filename, basename;
  PImage      img;
  PrintWriter output;
  int         pixelNum, byteNum, bytesOnLine = 99,
              x, y, b, rowBytes, totalBytes, lastBit, sum;
  println("Loading image...");
  filename = image.getPath();
  img      = loadImage(image.getPath());

  // Morph filename into output filename and base name for data
  x = filename.lastIndexOf('.');
  if(x > 0) filename = filename.substring(0, x);  // Strip current extension
  x = filename.lastIndexOf('/');
  if(x > 0) basename = filename.substring(x + 1); // Strip path
  else      basename = filename;
  filename += ".h"; // Append '.h' to output filename
  println("Writing output to " + filename);

  // Calculate output size
  rowBytes   = (img.width + 7) / 8;
  totalBytes = rowBytes * img.height;
  // Convert image to B&W, make pixels readable
  img.filter(THRESHOLD);
  img.loadPixels();

  // Open header file for output (NOTE: WILL CLOBBER EXISTING .H FILE, if any)
  output = createWriter(filename); 

  // Write image dimensions and beginning of array
  output.println("#ifndef _" + basename + "_h_");
  output.println("#define _" + basename + "_h_");
  output.println();
  output.println("#define " + basename + "_width  " + img.width);
  output.println("#define " + basename + "_height " + img.height);
  output.println();
  output.print("static const uint8_t PROGMEM " + basename + "_data[] = {");

  // Generate body of array
  for(pixelNum=byteNum=y=0; y<img.height; y++) { // Each row...
    for(x=0; x<rowBytes; x++) { // Each 8-pixel block within row...
      lastBit = (x < rowBytes - 1) ? 1 : (1 << (rowBytes * 8 - img.width));
      sum     = 0; // Clear accumulated 8 bits
      for(b=128; b>=lastBit; b >>= 1) { // Each pixel within block...
        if((img.pixels[pixelNum++] & 1) == 0) sum |= b; // If black pixel, set bit
      }
      if(++bytesOnLine >= 10) { // Wrap nicely
          output.print("\n ");
          bytesOnLine = 0;
      }
      output.format(" 0x%02X", sum); // Write accumulated bits
      if(++byteNum < totalBytes) output.print(',');
    }
  }

  // End array, close file, exit program
  output.println();
  output.println("};");
  output.println();
  output.println("#endif // _" + basename + "_h_");
  output.flush();
  output.close();
  println("Done!");
  exit();
}
