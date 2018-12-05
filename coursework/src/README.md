# src
This folder contains:
* The `Makefile`
* an `etc` directory that will contain the classes used in the project a `main.c` file that will be used to tie everything together
* a bin Lib directory that contains the main source file.

# building
To build the project simply run make in this directory
* The makefile will generate two different executables. only openMP run in the serial test. A new directory called `bin/` will be created and both executables can be found in there  


# running
Both executables are located in the `bin\` directory
* The time each took to run will be printed at the end.

# results
The results will be printed directly to STDOUT. A time for serial and a time for each implementation.

# extras
Passing a string into `encrypt.sh` will encrypt that string.

