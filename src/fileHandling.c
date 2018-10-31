#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

int checkExists(const char* fileName)
{
    return !access(fileName, F_OK);
}

FILE* openFile(const char* fileName, const char* mode)
{
    FILE* fp = fopen(fileName, mode);

    if(fp == NULL)
    {        
        printf("Failed to open file");
        exit(1);
    }

    return fp;
}

int closeFile(FILE* fp)
{
    return fclose(fp);
}

int printToFile(FILE* fp, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    return vfprintf(fp, format, ap);
}

char* readLine(char* buf, size_t bufSize, FILE* fp)
{
     return fgets(buf, bufSize, fp);
}