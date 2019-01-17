//
//  interfaceFunctions.c
//  NaiveInterface
//
//  Created by Malcolm James on 12/4/18.
//  Copyright © 2018 MalcolmSchoolWorkAtUAlbany. All rights reserved.
//

#include "interfaceFunctions.h"
#include "input.h"

//Called when quit is called in NaiveInterface
int quit(){

    char quit; // Char to get quit confirmation

    printf("\nAre you sure you want to exit? All files will be lost! Y/N: ");
    //Get user input
    quit = getchar();

    //If yes Walk the current directory, unlink non .c and .h files, and retun a value indicating sucessful quit
    if ((quit == 'Y') || (quit == 'y')){

        ftw(".", list, 1);
        ftw(".", list, 1);

        return 0;
    } else if ((quit == 'N') || (quit == 'n')){
        //retun a value indicating unsucessfull quit
        return 1;
    } else {
        //retun a value indicating unsucessfull quit
        printf("That wasn't recognized \n");
        return 2;
    }
}

int list (const char *name, const struct stat *status, int type) {

    //Status for unlink
    int deleteStatus;

    //Checks if program can walk the tree
    if (type == FTW_NS)
        return 0;

    //Checks if the file is a regular file
    if (type == FTW_F) {

        //checks if the program is our executable files
        if (status->st_mode& S_IXUSR) {
            //printf("REG_FILE: %-30s Don't Delete\n", name);
        } else {
            //Checks if the file is a .C or .H file
            if (strstr(name, ".c") || strstr(name, ".h") || strstr(name, "Makefile")) {
                //printf("We Out Here!!!!!\n");
                //printf("REG_FILE: %-30s Don't Delete\n", name);
            } else {
                //Otherwise delete the file
                //printf("REG_FILE: %-30s Delete\n", name);
                deleteStatus = unlink(name);

            }
        }
    } else { //Else block checks if it is a directory or and other member of FTW family

        if (type == FTW_D)
        {
            //Check if it reads the current directory and delets if it isn't the current directory
            if (strcmp(name, ".") != 0) {
                deleteStatus = remove(name);
            }

        }
    }

    return 0;
}

//***********************************************************************************************************************

void outputRedirect(char *command, char *dstFile) {

    char *Args[150];
    int CMDcount = 0;


//    printf("Output Redirect!!!!!!\n");
//    printf("New String is: %s\n", command);
//    printf("This is still here?: %s", dstFile);

    removeSubstring(dstFile, ">");
    removeSubstring(dstFile, " ");
    //printf("This is still here?: %s", dstFile);
    removeSubstring(dstFile, "\n");
    int reDirectFile = open(dstFile, O_CREAT|O_TRUNC|O_WRONLY);
    //FILE *dst = fopen(dstFile,"w");

    //Checks if file works
    if (reDirectFile < 0) {
        perror(dstFile);    /* open failed */
        exit(1);
    }

    //breaks command into tokens to run program
    char *token = strtok(command, " ");
    while (token != NULL)
    {
        //printf ("%s\n",token);
        Args[CMDcount] = token;
        token = strtok (NULL, " ");
        CMDcount++;
    }

    int i= 0;

    //Checks what the function is to
    if(strcmp(Args[i], "create") == 0) {

        // checks for valid number of arguements
        if ((CMDcount == 3) || (CMDcount == 4)) {

            //for child process
            pid_t  pid;
            int status;

            //Creates child process to run commands
            if ((pid = fork()) < 0) {     //fork a child process
                printf("ERROR: forking child process failed\n");
                exit(1);
            } else if (pid == 0) {          //for the child process
                //char *args[]={"./NewNew",NULL};
                dup2(reDirectFile, 1); // redirects byte output to a file
                if (execv(concat("./", Args[i]),Args) < 0) {     //execute the command
                    printf("*ERROR: exec failed\n");
                    exit(1);
                }
            } else {                                  //for the parent:
                while (wait(&status) != pid);       //wait for completion of child: No Zombies Here
            }

        } else {
            printf("Invalid number of arguements\n");
            exit(1);
        }

    } else if(strcmp(Args[i], "fileconverter") == 0){

        // checks for valid number of arguements
        if (CMDcount == 3) {

            //for child process
            pid_t  pid;
            int status;

            //Creates child process to run commands
            if ((pid = fork()) < 0) {     //fork a child process
                printf("ERROR: forking child process failed\n");
                exit(1);
            } else if (pid == 0) {          //for the child process
                //char *args[]={"./NewNew",NULL};
                dup2(reDirectFile, 1); // redirects byte output to a file
                if (execv(concat("./", Args[i]),Args) < 0) {     //execute the command
                    printf("*ERROR: exec failed\n");
                    exit(1);
                }
            } else {                                  //for the parent:
                while (wait(&status) != pid);       //wait for completion of child: No Zombies Here
            }

        } else {
            printf("Invalid number of arguements\n");
            exit(1);
        }

    }else if(strcmp(Args[i], "indexer") == 0){

        // checks for valid number of arguements
        if ((CMDcount == 2) || (CMDcount == 3)) {

            //for child process
            pid_t  pid;
            int status;

            //Creates child process to run commands
            if ((pid = fork()) < 0) {     //fork a child process
                printf("ERROR: forking child process failed\n");
                exit(1);
            } else if (pid == 0) {          //for the child process
                //char *args[]={"./NewNew",NULL};
                dup2(reDirectFile, 1); // redirects byte output to a file
                if (execv(concat("./", Args[i]),Args) < 0) {     //execute the command
                    printf("*ERROR: exec failed\n");
                    exit(1);
                }
            } else {                                  //for the parent:
                while (wait(&status) != pid);       //wait for completion of child: No Zombies Here
            }

        } else {
            printf("Invalid number of arguements\n");
            exit(1);
        }

    }else if(strcmp(Args[i], "srchindx") == 0) {

        // checks for valid number of arguements
        if ((CMDcount == 4) || (CMDcount == 5)) {

            //for child process
            pid_t  pid;
            int status;

            //Creates child process to run commands
            if ((pid = fork()) < 0) {     //fork a child process
                printf("ERROR: forking child process failed\n");
                exit(1);
            } else if (pid == 0) {          //for the child process
                //char *args[]={"./NewNew",NULL};
                dup2(reDirectFile, 1); // redirects byte output to a file
                if (execv(concat("./", Args[i]),Args) < 0) {     //execute the command
                    printf("*ERROR: exec failed\n");
                    exit(1);
                }
            } else {                                  //for the parent:
                while (wait(&status) != pid);       //wait for completion of child: No Zombies Here
            }

        } else {
            printf("Invalid number of arguements\n");
            exit(1);
        }

    }
}
