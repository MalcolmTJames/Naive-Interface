//
//  input.h
//  NaiveInterface
//
//  Created by Malcolm James on 12/4/18.
//  Copyright © 2018 MalcolmSchoolWorkAtUAlbany. All rights reserved.
//

#ifndef input_h
#define input_h

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

void getInput(void);
void getScriptInput(const char *PassedArg);
void inputParser (char *command);
void executeParser (char *command[], int count);
void executeCommand(char *command[], int count);
void removeSubstring(char *str, const char *toRemove);
char* concat(const char *s1, const char *s2);
void executeFuntion(char *command[], int count);
char* getLine(FILE* stream);
#endif /* input_h */
