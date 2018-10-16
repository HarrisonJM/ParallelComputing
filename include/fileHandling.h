#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

int checkExists(const char* fileName);
FILE* openFile(const char* fileName, const char* mode);
int closeFile(FILE* fp);
int printToFile(FILE* fp, const char* format, ...);
char* readLine(char* buf, size_t bufSize, FILE* fp);
