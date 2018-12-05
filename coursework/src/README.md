# src
This folder contains:
* The `Makefile`
* an `etc` directory that will contain the classes used in the project a `main.c` file that will be used to tie everything together
* a bin Lib directory that contains the main source file.

# building
To build the project simply run make in this directory
* The makefile will generate two different executables. Both will run in serial, however one will contain the openMPI threading and the other will have openMP. The time each took to run will be printed at the end.

# running
There is a script called RUNME.sh. Running this script will take care of all the tasks required to view this project (build and run).

# results
The results will be printed directly to STDOUT. A time for serial and a time for each implementation.

# extras
Passing a string into `encrypt.sh` will encrypt that string.

