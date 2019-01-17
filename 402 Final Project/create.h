#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close and symlink and hardlink


void create(char* commandAndOthers);

void filepath(char *enteredName);
void dirpath(char *enteredName);
void symbolicLink(char *enteredName);
void hardLink(char *enteredName);

mode_t umask(mode_t mask);
int mkdir(const char *pathname, mode_t mode);
int symlink(const char *name1, const char *name2);
