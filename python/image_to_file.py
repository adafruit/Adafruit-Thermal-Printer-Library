#!/usr/bin/env python
#
# This Pythons script uses PIL/Pillow to read in an image file
# and convert it to a header file for use with printBitmap().
#

import math
import argparse
from PIL import Image

# command line parsing
parser = argparse.ArgumentParser(description='Convert image to header file.')
parser.add_argument('input_image', help='the input image file')
parser.add_argument('-o', '--out_name', help='output name')
parser.add_argument('-d', '--dither', help='dither image')
args = parser.parse_args()

IN_FILE = args.input_image
NAME = args.out_name if args.out_name else IN_FILE.split('.')[0]
DITHER = Image.FLOYDSTEINBERG if args.dither else Image.NONE

# open image file
print("Reading input file", IN_FILE)
img = Image.open(IN_FILE).convert('1', dither=DITHER)

# create byte chunks
print("Converting to bytes.")
CHUNKS = math.ceil(img.width / 8)
img_bytes = bytearray()
for y in range(img.height):
    for chunk in range(CHUNKS):
        start = chunk * 8
        byte = 0
        for shift, x in enumerate(range(start, start+8)):
            p = img.getpixel((x, y)) if x < img.width else 1
            p = int(not p)
            byte |= p << (7-shift)
        img_bytes.append(byte)

# write header file
header_file = NAME+'.h'
print("Writing header file", header_file)
with open(header_file, 'w') as fp:
    fp.write("#ifndef _{}_h_\n".format(NAME))
    fp.write("#define _{}_h_\n\n".format(NAME))
    fp.write("#define {}_width {}\n".format(NAME, img.width))
    fp.write("#define {}_height {}\n\n".format(NAME, img.height))
    fp.write("static const uint8_t PROGMEM {}_data[] = ".format(NAME))
    fp.write("{\n")
    row_count = 0
    for b in img_bytes:
        fp.write("0x{:02x}, ".format(b))
        row_count += 1
        if row_count >= 12:
            fp.write("\n")
            row_count = 0
    fp.write("};\n")
    fp.write("\n#endif // _{}_h_\n".format(NAME))

print("DONE.")