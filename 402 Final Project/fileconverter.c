//Designed by Kelvin Almonte

#include "fileconverter.h"

int main(int argc, char** argv){

    if(argc < 3 || argc > 3){
        printf("ERROR, Incorrect number of arguments given to fileconverter.\n");
        return -1;
    }
    //pass infile and out directory arguments to fileconverter
    char argument[2048] = "";
	strcpy(argument, argv[1]);
	strcat(argument, " ");
	strcat(argument, argv[2]);
	fileconverter(argument);

}

void fileconverter(char *inFileAndOutDirectory){

    const char *a[2];
    char* token = strtok(inFileAndOutDirectory, " "); //tokenize infile and outdirectory
    int i;
    for(i = 0; i < 2; i++){
        a[i] = token;
        token = strtok(NULL," ");
    }

    char unorganizedFlights[200]; // 200 will be hopefully enough

    strcpy(unorganizedFlights, a[1]);

    char *ray[4];
    ray[0] = "create";
    ray[1] = "-d";
    ray[2] = unorganizedFlights;

    //make sure you try the binary file without a .bin extension or with one.
    FILE *ipFile;

    ipFile = fopen(a[0], "rb");
    if(ipFile){
        //file opened
    } else {
        char addingBin[1024] = "";
        strcpy(addingBin, a[0]);
        strcat(addingBin, ".bin");
        ipFile = fopen(addingBin, "rb");
    }

    if(!ipFile){
        printf("ERROR: INPUT FILE DOES NOT EXIST - exiting fileconverter now.\n");
        return;
    }

    //for child process
    pid_t  pid;
    int status;

    //Creates child process to run commands
    if ((pid = fork()) < 0) {     //fork a child process
        printf("ERROR: forking child process failed\n");
        exit(1);
    } else if (pid == 0) {          //for the child process
        if (execv("./create", ray) < 0) {     //execute the command
            printf("*ERROR: exec failed\n");
            exit(1);
        }
    } else {                                  //for the parent:
        while (wait(&status) != pid);       //wait for completion of child: No Zombies Here
    }

	int n = 0;
	int c;

    //open binary file, and store text conversion in outdirectory/outunorganized.txt
    strcat(unorganizedFlights, "/outunorganized.txt");



	FILE *opFile = fopen(unorganizedFlights, "w");

    //find position of last thing in binary file, set pos to it.
	fseek(ipFile, 0, SEEK_END);
	long pos = ftell(ipFile);

    fseek(ipFile, 0, SEEK_SET);

    //flag for getting airline
    int flag = 0;

    char binaryArray[8];
    int arrayCounter = 0;

    char airline[1024] = "";
    long count = 0;

    int tempC = 0;

    /*===================================================================*/

    //create text files for each unique airline, and copy to outunorganized.txt

    //run loop through binary file while count < pos && c is not EOF
    while (count < pos && (c = fgetc(ipFile)) != EOF) {
		binaryArray[arrayCounter] = c;
		arrayCounter++;

        //every eight binary digits, reset arraycounter to 0
		if(arrayCounter % 8 == 0){

			arrayCounter = 0;

			//convert char array of binary digits to an integer
			int binary;

			sscanf(binaryArray, "%d", &binary);

			//convert integer binary number to integer decimal number
			int dec_value = 0;
			int base = 1;
			int temp = binary;

			while(temp > 0){
				int last_digit = temp % 10;
				temp = temp / 10;
				dec_value += last_digit * base;
				base = base * 2;
			}

            int companyCounter = 0;

            //If it is the beginning of the current line,
            //and no instances of a non-alpha character has occured,
            //add letter to the flight company string.
            if(isalpha(dec_value) && flag == 0){

                char tmp = dec_value;
                size_t len = strlen(airline);

                //add the current letter to
                snprintf(airline + len, sizeof airline - len, "%c", tmp);

            } else {

                flag = 1;

                char tempDirName[200];  // 200 will be hopefully enough
                strcpy(tempDirName, a[1]);
                strcat(tempDirName, "/");
                strcat(airline, ".txt");
                strcat(tempDirName, airline);

                FILE *tempPointer = fopen(tempDirName, "rb");

                if(tempC == 0 && (fileExists(tempDirName) == 1)){

                    ray[0] = "create";
                    ray[1] = "-f";
                    ray[2] = tempDirName;

                    //for child process
                    pid_t  pid2;
                    int status2;

                    //Creates child process to run commands
                    if ((pid2 = fork()) < 0) {     //fork a child process
                        printf("ERROR: forking child process failed\n");
                        exit(1);
                    } else if (pid2 == 0) {          //for the child process
                        if (execv("./create", ray) < 0) {     //execute the command
                            printf("*ERROR: exec failed\n");
                            exit(1);
                        }
                    } else {                                  //for the parent:
                        while (wait(&status2) != pid2);       //wait for completion of child: No Zombies Here
                    }


                }
                tempC = 1;
                strcpy(airline, ""); //reset airline string

            }

            fprintf(opFile, "%c", dec_value);

            //Carriage Return ASCII: 13 & Line Feed ASCII: 10
            //These usually mean that a new line is coming
            if(dec_value == 10){
                flag = 0;
                int c;
                tempC = 0;
            }
		}
		count++;
	}
    fclose(ipFile);
    fclose(opFile);

    /*===================================================================*/

    //Now, parse the text file of unorganized flights.
    //We will sort the records by date and store them in a new text file.

    FILE *flightLines;

    flightLines = fopen(unorganizedFlights, "r");

    //two 2D arrays for storing lines and dates
    char lines[200][80];
    char dates[200][20];

    int recordCount = 0;

    char line[80];
    char date[20];

    //looping until the end of unorganized text file

    while(fgets(line, sizeof(line), flightLines) != NULL){

        line[strlen(line)]='\0'; //null terminating line
        getDate(line, date); //extracting date from line

        //store lines and dates in respective arrays
        strcpy(lines[recordCount], line);
        strcpy(dates[recordCount], date);

        recordCount++;
    }

    fclose(flightLines);

    sort(lines, dates, recordCount); //sort flight records based on arrays
    //OUTDIRECTORY
    char outDirectory[1000];

    strcpy(outDirectory, a[1]);

    strcat(outDirectory, "/outsorted.txt");

    FILE *sortedOutput = fopen(outDirectory, "w+");

    //writing lines to outsorted.txt
    for(i = 0; i < recordCount; i++){
        fprintf(sortedOutput, "%s\n", lines[i]);
    }

    //remove white lines from

    fclose(sortedOutput);

    //send each line to it's respective text file

    int lineNumber = 0;

    FILE *input_file = fopen(outDirectory, "r");

    char where[300];  // 200 will be hopefully enough
    char debug[300];  // 200 will be hopefully enough


    int counter = 0;

    if (input_file != NULL) {

        char line[500]; /* or other suitable maximum line size */
        int flaggers = 0;

        while (fgets(line, sizeof line, input_file) != NULL) {

            if (counter == lineNumber) {

                char currentCharacter;
                int lengthOfLine = strlen(line);

                //cast it to the variable u are assigning to
                char *name = malloc(80);

                for(i = 0; i < lengthOfLine; i++){
                    currentCharacter = line[i];

                    //code to retrieve airline so we know which file we put line

                    if(isalpha(currentCharacter) && flaggers == 0){
                        //add character to name
                        size_t len = strlen(name);
                        snprintf(name + len, sizeof name - len, "%c", currentCharacter);
                    } else {
                        flaggers = 1;
                    }

                    //if current character is line feed
                    if(currentCharacter == 10){
                        flaggers = 0;

                        //store name of output airline file for writing the line
                        strcpy(where, a[1]);
                        strcat(where, "/");
                        strcat(name, ".txt");
                        strcat(where, name);

                        strcpy(name, "");

                        strcpy(debug, a[1]);
                        strcat(debug, "/");
                        strcat(debug, ".txt");
                    }
                }
                //write line to the airline file where it belongs, ignoring .txt files

                if(strcmp(where, debug) == 0){
                    //this is a blank line
                } else {
                    FILE *h = fopen(where, "a");
                    if (h) {
                      fputs(line, h);
                      fclose(h);
                    }
                }


            } else {
                counter++;
            }
        }
        fclose(input_file);
    }

    //remove unorganized files after you finish using it
    remove(unorganizedFlights);
    remove(outDirectory);
}

//method to extract the date field from line and store in date parameter

void getDate(char *line, char *date){

    int last = strlen(line) - 1;
    int start = 0;
    int spaces = 0;

    //finding the index of second space from the end
    //because <Departure Date> begins two spaces from the end of a line
    int i;
    for(i = last; i >= 0; i--){
        if(line[i] == ' '){
            spaces++;

            if(spaces == 2){
                start = i;
                break;
            }
        }
    }

    int index = 0;

    //store characters between start + 1 and last in date
    for(i = start + 1; i <= last; i++){
        date[index]=line[i];
        index++;
    }

    date[index] = '\0';
}

//method to sort the lines with dates of size count

void sort(char lines[300][80], char dates[300][20], int count) {

    //parsing all rows of records
    int i;
    for(i = 0; i < count; i++){

    //parsing all columns in records
        int j;
        for(j = 0; j < count - 1; j++){

            //sorting based on date and time
            if(strcmp(dates[j], dates[j+1]) > 0){

                char temporary[300];

                //swap dates, then swap lines to match
                strcpy(temporary, dates[j]);
                strcpy(dates[j], dates[j+1]);
                strcpy(dates[j+1], temporary);

                strcpy(temporary, lines[j]);
                strcpy(lines[j], lines[j+1]);
                strcpy(lines[j+1], temporary);
            }
        }
    }
}

int fileExists(const char *fName){
    //METHOD TO CHECK IF FILE EXISTS
    FILE *file;
    if (file == fopen(fName, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}
