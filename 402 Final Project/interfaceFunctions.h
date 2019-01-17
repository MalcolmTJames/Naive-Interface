//
//  interfaceFunctions.h
//  NaiveInterface
//
//  Created by Malcolm James on 12/4/18.
//  Copyright © 2018 MalcolmSchoolWorkAtUAlbany. All rights reserved.
//

#ifndef interfaceFunctions_h
#define interfaceFunctions_h

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <ftw.h>

int quit(void);
void outputRedirect(char *command, char *dstFile);
int list (const char*, const struct stat *, int);

#endif /* interfaceFunctions_h */
