# png2histo

Small set of code to take a png image file from an oscilloscope screenshot and create a histogram. 

List of files:
===================================================
decode_png.C 

This is the code that I wrote, using picopng.cpp, to convert example.png
into extracted_histo.png

This code relies on ROOT v5 to produce the histogram.

===================================================
picopng.cpp

picoPNG version 20101224
Copyright (c) 2005-2010 Lode Vandevenne
decodePNG: The picoPNG function, decodes a PNG file buffer in memory, into a raw pixel buffer.
out_image: output parameter, this will contain the raw pixels after decoding.

===================================================
example.png

This is an typical example of the oscilloscope screenshot images which needed
to be converted into histogram objects. note the histogram is in teal.

===================================================
extracted_histo.png

This is the an image of the output of decode_png.C from the image example.png.
note how each individual peak can be resolved. It was important to resolve each peak
for scientific applications.
