// Vadim Castro
// ICSI 402
// Final Project
// srchindx.c

#include "srchindx.h"

/* ------------------------------  MAIN  ---------------------------------- */

int main(int argc, char **argv){

  int i;

  char* indexerFileName = (char*)malloc(sizeof(char) * 30);
  FILE* indexerFile = NULL;

  char* directoryName = (char*)malloc(sizeof(char) * 30);
  DIR* directoryToSearch = NULL;

  char* flagTerm = (char*)malloc(sizeof(char) * 5);

  char* airportsGiven = (char*)malloc(sizeof(char) * 100);


  if(argc == 4){

    // LOADS INDEXER FILE NAME AND OPENS FILE FOR READING
    strcpy(indexerFileName, reverseIndexFileDefault);
    indexerFile = fopen(indexerFileName, "r");

    if(!indexerFile){
      perror("Unable to open indexer file, please make sure it exists!");
      return -1;
    }


    // LOADS FLAG TERMS
    strcpy(flagTerm, argv[1]);

    // LOADS AIRPORTS ARGUMENT FOR PARSING
    strcpy(airportsGiven, argv[2]);

    // LOADS AND OPENS DIRECTORY TO SEARCH
    strcpy(directoryName, argv[3]);
    directoryToSearch = opendir(directoryName);

    if(!directoryToSearch){
      perror("Unable to open directory, please make sure it exists!");
      return -1;
    }

  }

  else if(argc == 5){

    // LOADS INDEXER FILE NAME AND OPENS FILE FOR READING
    strcpy(indexerFileName, argv[3]);
    indexerFile = fopen("./DATA/DATA2/jackson/nocap.txt", "r");

    if(!indexerFile){
      perror("Unable to open indexer file, please make sure it exists!");
      return -1;
    }

    // LOADS FLAG TERMS
    strcpy(flagTerm, argv[1]);

    // LOADS AIRPORTS ARGUMENT FOR PARSING
    strcpy(airportsGiven, argv[2]);

    // LOADS AND OPENS DIRECTORY TO SEARCH
    strcpy(directoryName, argv[4]);
    directoryToSearch = opendir(directoryName);

    if(!directoryToSearch){
      perror("Unable to open directory, please make sure it exists!");
      return -1;
    }

  }

  else{
    perror("Inappropriate number of parameters, please try again!");
    printf("ARGC: %d\n", argc );
    return -1;
  }


  /* --------------------- AIRPORT PARAMETER PARSING ---------------------- */

  char* token = (char*)malloc(sizeof(char) * 10);
  char** airportsParsed = (char**)malloc(sizeof(char*) * 10);

  for(i = 0; i < 10; i++){
    airportsParsed[i] = (char*)malloc(sizeof(char) * 10);
  }

  int airportsParsedCap = 0;


  token = strtok(airportsGiven, ";");

  while(token != NULL){
    strcpy(airportsParsed[airportsParsedCap++], token);
    token = strtok(NULL, ";");
  }

  printf("\nNum Airports: %d\n", airportsParsedCap);

/* -------------------- HASH TABLE vs BST BREAKDOWN ----------------------- */

if(!strcmp(flagTerm, "-o")){

  // BUILD HASH TABLE
  record **hashTable = (record**)malloc(sizeof(record*) * 7);

  for(i = 0; i < 7; i++){
    hashTable[i] = (record*)malloc(sizeof(record) * 20);
  }

  printf("Hash table made\n");



  // PREPROCCESING

  struct dirent* directoryEntry = NULL;

  // FOR AIRPORT in TERMS: search indexer file and traverse container files
  for(i = 0; i < airportsParsedCap; i++){

      int operatingAirports = 0;
      char* line = (char*)malloc(sizeof(char) * 300);
      // Traverse indexer file line by line checking for airport match
      while(fgets(line, sizeof(char) * 300, indexerFile)){

          printf("\nCompare time\n" );
          printf("SKEE: %s\n",line);
          printf("PEE: %s\n",airportsParsed[i]);

          char* air = (char*)malloc(sizeof(char) * 20);
          strcpy(air, airportsParsed[i]);
          strcat(air, "\n");

          if(strcmp(line, air) == 0){
              printf("matched\n");
              if(fscanf(indexerFile, "%[^\n]", line)){

                  char* fileName = (char*)malloc(sizeof(char) * 30);
                  int count;

                  token = strtok(line, " ");
                  printf("TOKEN: %s\n", token);

                  int flagger = 1;
                  int* flagPtr = &flagger;

                  while(token != NULL){

                    strcpy(fileName, token);
                    printf("FILENAME %s\n", fileName );

                    token = strtok(line, " ");
                    char* c = (char*)malloc(sizeof(char) * 3);
                    strcpy(c, token);

                    count = atoi(c);
                    printf("C: %d\n", count);

                    char* inPath = (char*)malloc(sizeof(char) * 50);
                    strcat(inPath, directoryName);
                    strcat(inPath, "/");
                    strcat(inPath, fileName);

                    FILE* fn = fopen(inPath, "r");

                    if(fn == NULL){
                      perror("Could not open file for reading");
                    }

                    printf("While fn: %s\n", inPath);


                    char* temp = (char*)malloc(sizeof(char) * 100);
                    char* token2 = (char*)malloc(sizeof(char) * 20);

                    while(flagger == 1){

                      while(fgets(temp, sizeof(char) * 100, fn)){

                        token2 = strtok(temp, " ");
                        token2 = strtok(NULL, " ");

                        if(!strcmp(token2, airportsParsed[i])){

                            operatingAirports++;
                            printf("OA T: %d\n", operatingAirports);
                            *flagPtr = 0;
                            break;

                        }

                        else{
                          flagger = 1;
                          continue;
                        }

                      }



                    }



                    fclose(fn);

                    token = strtok(line, " ");

                  }

              }

          }

      }

      int hash = hashFunction(airportsParsed[i], 7);

      if(hashTable[hash]->airport == NULL){

        printf("first hash entry \n" );
        hashTable[hash]->airport = (char*)malloc(sizeof(char) * 20);
        strcpy(hashTable[hash]->airport, airportsParsed[i]);

        hashTable[hash]->outgoingAirlines = operatingAirports;
        printf("OA: %d\n", operatingAirports);

        hashTable[hash]->head = 1;
        hashTable[hash]->next = NULL;

      }

      else{

        // ADD NODE TO LINKED LIST
        record* current = (record*)malloc(sizeof(record));
        current = hashTable[hash];

        while(current->next != NULL){

            current = hashTable[hash]->next;

        }

        current->next = (record*)malloc(sizeof(record));
        current->next->airport = (char*)malloc(sizeof(char) * 20);
        strcpy(current->next->airport, airportsParsed[i]);

        current->next->outgoingAirlines = operatingAirports;

        current->next->head = 0;
        current->next->next = NULL;

      }




  }


  for(i = 0; i < 7; i++){

      printf("i = %d: ", i);

      if(hashTable[i] != NULL){

        record* currentNode = (record*)malloc(sizeof(record));
        currentNode->airport = (char*)malloc(sizeof(char) * 20);


        // strcpy(currentNode->airport, hashTable[i]->airport);
        currentNode = hashTable[i];

        //currentNode->outgoingAirlines = hashTable[i]->outgoingAirlines;

        //currentNode->next = (record*)malloc(sizeof(record));
        //currentNode->next = hashTable[i]->next;

        while(currentNode != NULL){

          printf("Airport: %s --- Outgoing Airlines: %d ", currentNode->airport, currentNode->outgoingAirlines);
          currentNode = currentNode->next;

        }

        printf("\n");

        }

        else{
          continue;
        }


  }




}

else if(!strcmp(flagTerm, "-d")){

  // BUILD BST



}

else{
  perror("Invalid flag, please enter -o for origin or -d for destination.");
  return -1;
}











return 0;

}
