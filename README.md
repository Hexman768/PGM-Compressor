# PGM Compressor 
This program reads in data from a PGM image specified as a command line argument and encodes it into unsigned characters in a much smaller file. This program is essentially a PGM image compressor. 
This program was written as part of a programming assignment in my C and Unix class, I just thought it was interesting enough to continue to build. 

# Algorithm 
This lossy compression program uses the discrete cosine transform algorithm to compress PGM images and decompress them. That means each pgm image is read into an 8x8 matrix that is then transformed with the aforementioned algorithm. 

# Compilation and Execution Instructions 
* Run "make all" to compile program
* Run "./main [input file] [output file]"
* Program code will execute and create an output file with the compressed data inside 

# Example
./main input.pgm output.dat
