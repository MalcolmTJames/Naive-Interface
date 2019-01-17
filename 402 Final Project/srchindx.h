// VADIM CASTRO 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <ftw.h>
#include <libgen.h>
#include <unistd.h>

#define reverseIndexFileDefault "invind.txt"

#define _GNU_SOURCE

/* ---------------------------- DATA STRUCTURES ---------------------------- */

typedef struct record record;

struct record{
    int head;
    char* airport;
    int outgoingAirlines;
    record* next;

};

typedef struct {
    char *fileName;
    int count;
} file_count;

typedef struct {
    char *name;
    file_count *files;
    int files_cap;
    int files_alloc_size;
} index_entry;

/* ------------------------------  HASH  --------------------------------- */

int  hashFunction(char *s,  int T) {

   /* The parameter s represents the symbol to be hashed and  */
   /* the parameter T represents the size of the hash table.  */
   /* The function returns the hash value for the symbol s.   */

   /* String s is assumed to be terminated with '\0'.         */
   /* It is also assumed that T is at least 2. The returned   */
   /* hash value is an integer in the range 0 to T-1.         */

   /* The function computes the hash value using arithmetic   */
   /* based on powers of the BASE value defined below.        */

   #define  BASE   127

   int h = 0;     /* Will hold the hash value at the end. */
   int temp;      /* Temporary.                           */

   /* The hash value is computed in the for loop below. */
   for (;  *s != 0;  s++) {
       temp = (BASE * h + *s);
       if (temp < 0) temp = -temp;
       h = temp % T;
   }

   /* The hash value computation is complete. So, */
   return h;

}
