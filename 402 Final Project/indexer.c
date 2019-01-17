// Vadim Castro
// ICSI 402
// Final Project
// indexer.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <ftw.h>
#include <libgen.h>
#include <unistd.h>

#define reverseIndexFileDefault "invind.txt"

#define _GNU_SOURCE


// custom struct to hold name of airport and respective hit count per directory entry
typedef struct {
    char *name;
    int count;
} airportData;

typedef struct {
    char *fileName;
    int count;
} file_count;

typedef struct {
    char *name;
    file_count data;  // MAYBE CHANGE BACK TO POINTER
} airport_file_count;

typedef struct {
    char *name;
    file_count *files;  // dynamically allocate memory at creation since not allowed within struct def
    int files_cap;
    int files_alloc_size;
} index_entry;

typedef struct {
    char *name;
    int indexer_position;
} airport_to_index;



int nameComparator(const void *p1, const void *p2){
    return strcmp( (*((index_entry**)p1))->name, (*((index_entry**)p2))->name );
}

int countComparator(const void *p1, const void *p2){
    if(((file_count*)p1)->count < ((file_count*)p2)->count){
        return 1;
    }
    else if(((file_count*)p1)->count > ((file_count*)p2)->count){
        return -1;
    }
    else{
        return 0;
    }
}


/* -------------------------------  MAIN  --------------------------------- */


int main(int argc, char** argv){

    char *dir_target = (char*)malloc(sizeof(char) * 80);
    char* outputPath = (char*)malloc(sizeof(char) * 110);

    DIR *directoryToParse = NULL;
    FILE *reverseIndexOutFile = NULL;



    // if-else cluster to determine proper file I/O configuration
    if(argc == 2){

        dir_target = argv[1];

        directoryToParse = opendir(dir_target);

        strcpy(outputPath, dir_target);
        strcat(outputPath, "/");
        strcat(outputPath, reverseIndexFileDefault);

        //printf("%s\n", outputPath);

    }

    else if(argc == 3){

        dir_target = argv[2];

        directoryToParse = opendir(dir_target);


        strcpy(outputPath, dir_target);
        strcat(outputPath, "/");
        strcat(outputPath, argv[1]);

        //printf("%s\n", outputPath);

    }

    else{
        printf("Too many arguments passed!");
        return -1;

    }



    struct dirent *directoryEntry; // directory entry pointer for directory elements



    int indexer_mem = 30;

    int currentCapacityIndexer = 0;

    airport_file_count *indexer = (airport_file_count*)malloc(indexer_mem * sizeof(airport_file_count)); // final struct array to hold info for output file






    if(directoryToParse != NULL){

        FILE *directoryFile = NULL;
        char *fileName = (char*)malloc(sizeof(char) * 30);
        char *file_path = (char*)malloc(sizeof(char) * 150);

        // DIRECTORY LOOP
        while((directoryEntry = readdir(directoryToParse)) != NULL){

            // ORDER

            if(directoryEntry->d_type == DT_REG && strcmp(directoryEntry->d_name, ".DS_Store")){

                fileName = directoryEntry->d_name;


                strcpy(file_path, dir_target);
                strcat(file_path, "/");
                strcat(file_path, fileName);

                //printf("\nFILE PATH: %s\n", file_path);


                directoryFile = fopen(file_path, "r");


                if(directoryFile == NULL){
                    perror("Unable to open directory entry.\n");
                    //free(file_path);
                    continue;
                }
                else{
                  //free(file_path);
                }



            }
            else{
                continue;
            }



            int AFHA_mem = 10;

            int currentCapacityAFHA = 0;

            airportData *airportFileHitsArray = (airportData*)malloc(sizeof(airportData) * AFHA_mem); // don't forget to initialize structs



            // NO CAP
            char *line = (char*)malloc(sizeof(char) * 100);

            char *origin = (char*)malloc(sizeof(char) * 30);

            char *destination = (char*)malloc(sizeof(char) * 30);


            int newO = 1;
            int newD = 1;
            int *NOptr = &newO;
            int *NDptr = &newD;

            // FILE LOOP
            while(fgets(line, sizeof(char) * 100, directoryFile)){


                //printf("\n%s\n", line);

                // retrives origin airport value from line
                origin = strtok(line, " ");
                origin = strtok(NULL, " ");

                // printf("%s\n", origin);

                // retrives destinatoin airport value from line
                destination = strtok(NULL, " ");

                // printf("%s\n", destination);




                int i;
                for(i = 0; i < currentCapacityAFHA; i++){

                    if(!strcmp(airportFileHitsArray[i].name, origin)){

                        //printf("CAP: %d\n",currentCapacityAFHA );

                        //printf("matchO: @ i = %d --- %s --- %s\n", i, airportFileHitsArray[i].name, origin);
                        airportFileHitsArray[i].count += 1;

                        //printf("matchO: %d\n",airportFileHitsArray[i].count );
                        *NOptr = 0;

                    }

                    else if(!strcmp(airportFileHitsArray[i].name, destination)){

                        //printf("CAP: %d\n",currentCapacityAFHA );

                        //printf("matchD: @ i = %d --- %s --- %s\n", i, airportFileHitsArray[i].name, destination );
                        airportFileHitsArray[i].count += 1;

                        //printf("matchD: %d\n",airportFileHitsArray[i].count );
                        *NDptr = 0;

                    }


                    else{
                        // printf("NO MATCH @ i = %d --- name: %s --- AFHA cap: %d\n", i, airportFileHitsArray[i].name, currentCapacityAFHA);

                    }
                }


                //sleep(5);


                if(newO == 1){

                  // ORIGIN

                  airportFileHitsArray[currentCapacityAFHA].name = (char*)malloc(sizeof(char) * 10);
                  strcpy(airportFileHitsArray[currentCapacityAFHA].name, origin);


                  airportFileHitsArray[currentCapacityAFHA].count = 1;

                  currentCapacityAFHA += 1;





                  if(currentCapacityAFHA == AFHA_mem){
                      AFHA_mem += 10;
                      airportFileHitsArray = (airportData*)realloc(airportFileHitsArray, AFHA_mem * sizeof(airportData));
                  }


                }



                if(newD == 1){

                  // DESTINATION
                  airportFileHitsArray[currentCapacityAFHA].name = (char*)malloc(sizeof(char) * 10);
                  strcpy(airportFileHitsArray[currentCapacityAFHA].name, destination);

                  airportFileHitsArray[currentCapacityAFHA].count = 1;

                  currentCapacityAFHA += 1;





                  if(currentCapacityAFHA == AFHA_mem){
                      AFHA_mem += 10;
                      airportFileHitsArray = (airportData*)realloc(airportFileHitsArray, AFHA_mem * sizeof(airportData));
                  }


                }

                // KILL


                // traverses airportArray of airportData structs to transfer values to airport_file_count struct
                for(i = 0; i < currentCapacityAFHA; i++){


                     indexer[currentCapacityIndexer].name = (char*)malloc(sizeof(char) * 10);
                     strcpy(indexer[currentCapacityIndexer].name, airportFileHitsArray[i].name);

                     indexer[currentCapacityIndexer].data.fileName = (char*)malloc(sizeof(char) * 20);
                     strcpy(indexer[currentCapacityIndexer].data.fileName, fileName);

                     indexer[currentCapacityIndexer].data.count = airportFileHitsArray[i].count;
                     currentCapacityIndexer++;


                     if(currentCapacityIndexer == indexer_mem){
                       indexer_mem += 10;
                       indexer = (airport_file_count*)realloc(indexer, indexer_mem * sizeof(airport_file_count));
                     }

                }


            }

            fclose(directoryFile);


            free(airportFileHitsArray);
            airportFileHitsArray = NULL;



            free(line);
            //free(origin);
            //free(destination);


        }

        //free(fileName);
        closedir(directoryToParse);






        /* write code to take indexer array of airport_file_count structs and organize them into a single appearance (airport)
         to multiple file_count struct data structure

         example:

         ATL --> [ ("file1.txt", 3) , ("file2.txt", 9) , ("file3.txt", 5) ]

         JFK --> [ ("file1.txt", 1) , ("file2.txt", 6) , ("file3.txt", 3) ]

         */


        // Creates backbone for indexer to mf_indexer transfer and transfers first element over

        int index_alloc_size = 10;
        int files_alloc_size = 10;

        /* --------------------------------------------- Multiple File Indexer ----------------------------------------------- */


        index_entry** mf_indexer = (index_entry**)malloc(index_alloc_size * sizeof(index_entry*));

        int mf_indexer_cap = 0;

        int i;
        for(i = 0; i < index_alloc_size; i++){
            mf_indexer[i] = (index_entry*)malloc(sizeof(index_entry));
        }



        airport_to_index* indexer_members = (airport_to_index*)malloc(index_alloc_size * sizeof(airport_to_index));
        int indexer_members_cap = 0;


        /* ------------------------------------------------------------------------------------------------------------------- */


        // Transfers rest of old indexer into multiple file count indexer

        for(i = 0; i < currentCapacityIndexer; i++){

            int isNew = 1;
            int newFile = 1;

            int j;
            for(j = 0; j < indexer_members_cap; j++){

                if(!strcmp(indexer[i].name, indexer_members[j].name)){

                    isNew = 0;

                    int k;
                    for(k = 0; k < mf_indexer[indexer_members[j].indexer_position]->files_cap; k++){

                      if(!strcmp(mf_indexer[indexer_members[j].indexer_position]->files[k].fileName, indexer[i].data.fileName)){
                        mf_indexer[indexer_members[j].indexer_position]->files[k].count = indexer[i].data.count;
                        newFile = 0;
                      }

                    }

                    if(newFile == 1){

                      mf_indexer[indexer_members[j].indexer_position]->files[mf_indexer[indexer_members[j].indexer_position]->files_cap].fileName = (char*)malloc(sizeof(char) * 30);
                      strcpy(mf_indexer[indexer_members[j].indexer_position]->files[mf_indexer[indexer_members[j].indexer_position]->files_cap].fileName, indexer[i].data.fileName);

                      mf_indexer[indexer_members[j].indexer_position]->files[mf_indexer[indexer_members[j].indexer_position]->files_cap++].count = indexer[i].data.count;

                    }



                    if(mf_indexer[indexer_members[j].indexer_position]->files_cap == mf_indexer[indexer_members[j].indexer_position]->files_alloc_size){

                        mf_indexer[indexer_members[j].indexer_position]->files_alloc_size += 5;

                        mf_indexer[indexer_members[j].indexer_position]->files = (file_count*)realloc(mf_indexer[indexer_members[j].indexer_position]->files, mf_indexer[indexer_members[j].indexer_position]->files_alloc_size * sizeof(file_count));
                    }


                }
            }



            if(isNew == 1){


                mf_indexer[mf_indexer_cap]->name = (char*)malloc(sizeof(char) * 20);
                strcpy(mf_indexer[mf_indexer_cap]->name, indexer[i].name);

                mf_indexer[mf_indexer_cap]->files_cap = 0;
                mf_indexer[mf_indexer_cap]->files = (file_count*)malloc(files_alloc_size * sizeof(file_count));
                mf_indexer[mf_indexer_cap]->files_alloc_size = files_alloc_size;

                mf_indexer[mf_indexer_cap]->files[mf_indexer[mf_indexer_cap]->files_cap].fileName = (char*)malloc(sizeof(char) * 30);
                strcpy(mf_indexer[mf_indexer_cap]->files[mf_indexer[mf_indexer_cap]->files_cap].fileName, indexer[i].data.fileName);

                mf_indexer[mf_indexer_cap]->files[mf_indexer[mf_indexer_cap]->files_cap++].count = indexer[i].data.count;
                mf_indexer_cap++;



                indexer_members[indexer_members_cap].name = (char*)malloc(sizeof(char) * 20);
                strcpy(indexer_members[indexer_members_cap].name, indexer[i].name);


                indexer_members[indexer_members_cap].indexer_position = mf_indexer_cap - 1;
                indexer_members_cap++;


                if(indexer_members_cap == index_alloc_size){


                    index_alloc_size += 5;
                    mf_indexer = (index_entry**)realloc(mf_indexer, index_alloc_size * sizeof(index_entry*));


                    int i;
                    for(i = 0; i < index_alloc_size; i++){
                        mf_indexer[i] = (index_entry*)realloc(mf_indexer[i], sizeof(index_entry));
                    }


                    indexer_members = (airport_to_index*)realloc(indexer_members, index_alloc_size * sizeof(airport_to_index));
                }


                // free(NEW.files); // FREE NEW



            }


          //  }

        }


        for(i = 0; i < currentCapacityIndexer; i++){
          free(indexer[i].name);
          indexer[i].name = NULL;
          free(indexer[i].data.fileName);
          indexer[i].data.fileName = NULL;
        }

        free(indexer);
        indexer = NULL;

        for(i = 0; i < indexer_members_cap; i++){
          free(indexer_members[i].name);
        }

        free(indexer_members);
        indexer_members = NULL;





        /* ------------------------------------------------------------------------------------------------------------------- */


        /*
         printf("\n\n");
         for(int i = 0; i < mf_indexer_cap; i++){
         printf("%s --- %d\n", mf_indexer[i]->name, mf_indexer[i]->files[0].count);
         }
         */


         // printf("Starting sort: \n");
        qsort(mf_indexer, mf_indexer_cap, sizeof(index_entry*), nameComparator);

        for(i = 0; i < mf_indexer_cap; i++){
            qsort(mf_indexer[i]->files, mf_indexer[i]->files_cap, sizeof(file_count) , countComparator);
        }


        reverseIndexOutFile = fopen(outputPath, "w");

        if(reverseIndexOutFile == NULL) printf("Could not open indexer output file");

        free(outputPath);
        outputPath = NULL;


        for(i = 0; i < mf_indexer_cap; i++){

            fprintf(reverseIndexOutFile, "%s\n", mf_indexer[i]->name);

            int j;

            for(j = 0; j < mf_indexer[i]->files_cap; j++){
                fprintf(reverseIndexOutFile, "%s %d ", mf_indexer[i]->files[j].fileName, mf_indexer[i]->files[j].count);
            }
            fprintf(reverseIndexOutFile, "\n");


        }


        fclose(reverseIndexOutFile);


        for(i = 0; i < mf_indexer_cap; i++){
            free(mf_indexer[i]->files);
            free(mf_indexer[i]->name);
        }

        for(i = 0; i < index_alloc_size; i++){
            free(mf_indexer[i]);
        }

        free(mf_indexer);

        mf_indexer = NULL;


        return 0;

    }

    else{
        printf("Unable to reach directory.");
        return 0;
    }



    /* TO DO:

     FINISH DYNAMIC MEMORY ALLOC (indexer[100]                                                                          √√√√√√√

     CREATE FILE OUTPUT METHODS AND TEST WITH KELVINS CODE                                                              √√√√√√

     MAKE SURE THAT DIRECTORY METHODS ARE CORRECT AND VISIT ANY PATH SPECIFIED CORRECTLY

     DOUBLE CHECK ALL CODE AND CHOSEN VALUES [refer to comment breadcrumbs]

    */



    }
