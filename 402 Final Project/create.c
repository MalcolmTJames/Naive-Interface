//Designed by Kelvin Almonte

#include "create.h"

int main(int argc, char** argv){

    //error checking for incorrect number of arguments
    if(argc < 3 || argc > 4){
        printf("ERROR, Incorrect number of arguments given to create.\n");
        return -1;
    }

    char argument[1024] = "";
	strcpy(argument, argv[1]);
	strcat(argument, " ");
	strcat(argument, argv[2]);

    if(argc == 4){
        strcat(argument, " ");
        strcat(argument, argv[3]);
    }
    create(argument);
}

void create(char* commandAndOthers){
    int len = strlen(commandAndOthers);
    char fp[10] = "-f";
    char dp[10] = "-d";
    char hl[10] = "-h";
    char sl[10] = "-s";
    char dest[140];

    //take first 9 characters which will take '-f', '-d', '-h', or '-s'    .
    strncpy(dest, commandAndOthers, 2);
    dest[2] = 0;

    //take substring after 2 characters
    char substr[1024];
    int length = strlen(commandAndOthers) - 3;
    if(length >= 0) {
        strncpy(substr, (commandAndOthers + 3), length);
        substr[length] = '\0';
    }

    //call method depending on what the dest string is holding
    if (strcmp(dest, fp) == 0) {
        filepath(substr);
    }
    if (strcmp(dest, dp) == 0) {
        dirpath(substr);
    }
    if (strcmp(dest, hl) == 0) {
        hardLink(substr);
    }
    if (strcmp(dest, sl) == 0) {
        symbolicLink(substr);
    }
}

void filepath(char *enteredName){
    //– create -f filepath creates an empty, ordinary file
    //The new file should have permission 0640 specified in octal.

    //creating a new file ONLY if the filename is placed in filepath's place
    //Just call fopen with the file name and the "w" parameter and it will
    //create/overwrite a file in the executable's directory...

    umask(0);
    int filehandle = open (enteredName, O_WRONLY | O_CREAT, 0640);
    close(filehandle);
}

void dirpath(char *enteredName){
    mkdir(enteredName, 0750);
}

void hardLink(char *enteredName){
    //delimiters
    char* newString;
    newString = strtok(enteredName, " ,.-");

    const char *oldNameAndLinkName[2];
    oldNameAndLinkName[0] = newString;

    newString = strtok(NULL, " ,.-");

    oldNameAndLinkName[1] = newString;
    link(oldNameAndLinkName[0], oldNameAndLinkName[1]);

}

void symbolicLink(char *enteredName){

    //delimiters
    char* newString;
    newString = strtok(enteredName, " ,.-");

    const char *oldNameAndLinkName[2];

    oldNameAndLinkName[0] = newString;

    newString = strtok(NULL, " ,.-");

    oldNameAndLinkName[1] = newString;

    //doesnt work if you do create -s testing.txt working
    symlink(oldNameAndLinkName[0], oldNameAndLinkName[1]);
}
