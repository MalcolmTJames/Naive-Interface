#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


void fileconverter();
int execv (const char *filename, char *const argv[]);
void getDate(char *line, char *date);
void sort(char lines[100][80], char dates[100][20], int count);
int fileExists(const char *filename);
