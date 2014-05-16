// Convert image to a C header file suitable for the Adafruit_Thermal library.
// This is NOT an Arduino sketch.  Runs in Processing IDE (www.processing.org)

void fileSelected(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
    exit();
  }
  
  String      filename = selection.getAbsolutePath();
  String      basename;
  PImage      img;
  PrintWriter output;
  int         i, x, y, b, rowBytes, totalBytes, lastBit, sum, n;

  println("Loading image " + filename + "..");
  img        = loadImage(filename);
  
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
  output.print("static PROGMEM prog_uchar " + basename + "_data[] = {");

  // Generate body of array
  for(i=n=y=0; y<img.height; y++) { // Each row...
    output.print("\n  ");
    for(x=0; x<rowBytes; x++) { // Each 8-pixel block within row...
      lastBit = (x < rowBytes - 1) ? 1 : (1 << (rowBytes * 8 - img.width));
      sum     = 0; // Clear accumulated 8 bits
      for(b=128; b>=lastBit; b >>= 1) { // Each pixel within block...
        if((img.pixels[i++] & 1) == 0) sum |= b; // If black pixel, set bit
      }
      output.format("0x%02X", sum); // Write accumulated bits
      if(++n < totalBytes) output.print(',');
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

void setup() {
  selectInput("Select a file to convert:", "fileSelected");
}
