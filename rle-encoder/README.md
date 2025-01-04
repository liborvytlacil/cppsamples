# Compression/Decompresion via Run-Length encoding
A simple program with CLI interface that can encode files using RLE algorithm or decode already encoded files.

# Build
Build a single compilation unit `main.cpp` with c++11 standard enabled, e.g.
```
g++ --std=c++11 -Wall -pedantic -o rle.out main.cpp
```
(The development was done using Apple clang 16.0.0)/

# Usage
```
rle.out {-c|-d} [-o outputfile] inputfile
```
* `-c` indicates encode mode.
* `-d` indicated decode mode.
* `-o outputfile` allows to optionally specify the output file name. If omitted, the name of the output file is the name of the input file with `.out` appended.

## Sample
As an example, you can try encoding and then decoding a simple bitmap image `sample/sample.bmp`.

To encode, run:

`rle.out -c -o sample/sample.enc sample/sample.bmp`

To decode, run
`rle.out -d -o sample/sample.orig.bmp sample/sample.enc`

which should produce the original bitmap.


# Tests
Unit tests are provided in `test.cpp`. Build them using
```
g++ --std=c++11 -Wall -pendatic -o test.out test.cpp
```
and then simply run them using
```
./test.out
```