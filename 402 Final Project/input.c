//
//  input.c
//  NaiveInterface
//
//  Created by Malcolm James on 12/4/18.
//  Copyright © 2018 MalcolmSchoolWorkAtUAlbany. All rights reserved.
//

#include "input.h"
#include "interfaceFunctions.h"


#define MAX_LIMIT 150
#define MAX_ARGS 10

void getInput()
{
    //String for the comand entered by user
    char CMD[MAX_LIMIT];

    int numToQuit = 1;
    //Used to keep looping and asking user for a command until quit is pressed
    while (numToQuit) {
        printf("Please enter a command: ");
        scanf("%[^\n]%*c", CMD); //Gets command
        removeSubstring(CMD, "\n");
        inputParser(CMD);
    }
    printf("\n\n");
}

void getScriptInput(const char *PassedArg)
{
    //Gets the Script file
    FILE *Script = fopen(PassedArg,"r");

    //String for command by a script
    char *cmd = NULL;
    size_t len = 0;
    ssize_t read;

    //Check is the file can be opened
    if (Script == NULL) {
        printf("This File could not be ran\n");

    } else {
        //Keeps reading script until user hits quit
        while ((read = getline(&cmd, &len, Script)) != -1) {
            //printf("Retrieved line of length %zu: \n", read);
            printf("\n%s", cmd);
            inputParser(cmd);

        }
        printf("\n\n");
        fclose(Script);
    }
}


void inputParser (char *command) {

    char *Args[MAX_ARGS]; // set array for passing arguments to executable
    char newCMD[MAX_LIMIT];   // array used to copy command and check for output redirect
    int CMDcount = 0; //for keeping count of arguments passed

    //copy command
    strcpy(newCMD, command);

    //Checks for '>' for output redirect
    char *s;
    s = strchr (newCMD, '>');
    if (s != NULL) {
        // If Output Redirect then Pass Command and Redirect text
        // stores copy of string after '>'
        char RedirectText[strlen(s)];
        strcpy(RedirectText, s);

        // if there is output redirect...
        //printf ("found a '>' at %s\n", s);
        removeSubstring(newCMD, s);

        //Preforms Output Redirect in interfaceFunctions.c
        outputRedirect(newCMD, RedirectText);

    } else {

        // if isn't a output redirect
        //Check for Quit
        char *keyWord = strtok(command," ");
        if (keyWord != NULL) {
            if (strcmp("quit", keyWord) == 0) {
                int isQuit = quit();

                if (isQuit == 0) {
                    exit(0);
                } else {
                    return;
                }
            }
        }

        //printf("This the new CMD: %s\n", newCMD);
        removeSubstring(newCMD, "\n");
        //Breaks the command into Args[] to go to Executable
        //printf ("Splitting string \"%s\" into tokens:\n\n", newCMD);
        char *token = strtok(newCMD, " ");
        while (token != NULL)
        {
            //printf ("%s\n",token);
            Args[CMDcount] = token;
            token = strtok (NULL, " ");
            CMDcount++;
        }
        //Organ
        Args[CMDcount] = NULL;
        //---char *Args[]={ "one", "two", "three", NULL };
        executeCommand(Args, CMDcount);
    }

}

void executeCommand(char *command[], int count){
    int i=0;    // To grab the function to preform

    //printf("The count is %d: \n", count);

    //Checks what the function is to
    if(strcmp(command[i], "create") == 0) {

        // checks for valid number of arguements
        if ((count == 3) || (count == 4)) {
            executeFuntion(command, count);
        } else {
            printf("Invalid number of arguements\n");
            exit(1);
        }

    } else if(strcmp(command[i], "fileconverter") == 0){

        // checks for valid number of arguements
        if (count == 3) {
            executeFuntion(command, count);
        } else {
            printf("Invalid number of arguements\n");
            exit(1);
        }

    }else if(strcmp(command[i], "indexer") == 0){

        // checks for valid number of arguements
        if ((count == 2) || (count == 3)) {
           executeFuntion(command, count);
        } else {
            printf("Invalid number of arguements\n");
            exit(1);
        }

    }else if(strcmp(command[i], "srchindx") == 0) {

        // checks for valid number of arguements
        if ((count == 4) || (count == 5)) {
            executeFuntion(command, count);
        } else {
            printf("Invalid number of arguements\n");
            exit(1);
        }

    } else if(strcmp(command[i], "quit") == 0){
        return;
    } else {
        printf("Command not found\n");
    }
}

//Actually execute the command
void executeFuntion(char *command[], int count) {

     int i=0;    // To grab the function to preform

    //for child process
    pid_t  pid;
    int status;

    //Creates child process to run commands
    if ((pid = fork()) < 0) {     //fork a child process
        printf("ERROR: forking child process failed\n");
        exit(1);
    } else if (pid == 0) {          //for the child process
        //char *args[]={"./NewNew",NULL};
        if (execv(concat("./", command[i]),command) < 0) {     //execute the command
            printf("*ERROR: exec failed\n");
            exit(1);
        }
    } else {                                  //for the parent:
        while (wait(&status) != pid);       //wait for completion of child: No Zombies Here
    }

}

//Get line for readind input line by line
char* getLine(FILE* stream){

    //String for command by a script
    char *cmd = NULL;
    size_t len = 0;
    ssize_t read;

    //check for Null
    if (stream != NULL) {
        while ((read = getline(&cmd, &len, stream)) != -1) {
            //printf("Retrieved line of length %zu: \n", read);
            return cmd;
        }
        printf("\n\n");
        fclose(stream);
    }
    return cmd;
}


//Removes a substring
void removeSubstring(char *str, const char *toRemove) {
    //Get's length of
    size_t length = strlen(toRemove);
    char *found;
    char *next = strstr(str, toRemove); //finds the string to remove

    //loops to remove the substring
    size_t bytesRemoved;
    for (bytesRemoved = 0; (found = next); bytesRemoved += length){
        char *rest = found + length;
        next = strstr(rest, toRemove);
        memmove(found - bytesRemoved, rest, next ? next - rest: strlen(rest) + 1);

    }
}

//concatinate Strings
char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // creates new string thats the size of both strings for +1 for the null-terminator
    strcpy(result, s1); //copies to resault
    strcat(result, s2); //concatinates to end of second
    return result;
}
