//
//  main.c
//  NaiveInterface
//
//  Created by Malcolm James on 11/30/18.
//  Copyright © 2018 MalcolmSchoolWorkAtUAlbany. All rights reserved.
//

#include "input.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, const char * argv[]) {
    // insert code here...
    
    //Gets the input from user
    if (argv[1] == NULL) {
        getInput();
    } else {
        getScriptInput(argv[1]);
    }
    
    return 0;
}
