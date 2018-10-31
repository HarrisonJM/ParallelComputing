# src
This folder contains:\
* The `Makefile`\
* an `OpenMP` directory\
* an `MPI` directory\
* an `etc` directory that will contain utility classes not directly related\
* a `main.c` file that will be used to tie everything together

# building
To build the project simply run make in this directory\
* Each `openMP` and `MPI` directory will be compiled into a header archive and
linked against the final executable; which will include main.c

# results
The results will be printed out as a part of the main running of the program.\
The program can be executed by typing `$ ./cipherCracker.out`. Optionally\
passing an encrypted textfile path into the program will instead decipher that.\

# extras
Passing a string into `encrypt.sh` will encrypt that string and \
Output a file that will automatically be picked up by the program for deciphering.

