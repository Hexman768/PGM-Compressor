# CBinaryEncoder
This program reads in data from a file specified from a command line argument and encodes the data into binary.

# Rules
* The first bit represents the value being positive (0) or negative (1)
* The next 6 bits reprsent the occurence (the second number on the line), this value is decreased by 1 for encoding purposes
* The last 10 bits represent the absolute value of the value (first number).

# Example
Command line: ./a.out data.dat

data.dat: -20 4

output: 10000110000010100
