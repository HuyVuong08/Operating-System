#include    "readline.h"
#include    <stdio.h>
#include    <string.h>

int s = 0;

/** @description Determines the area of a circle that has the specified radius parameter.  
 * @param {str} radius The radius of the circle.  
 * @return {int}  
 */  
int read_line(char *str){
    if (fgets(str, MAX_BUFF, stdin) != NULL){
        s = strlen(str);
        if (*(str + s - 1) == 10){
            *(str + s - 1) = 0;
            return s - 1;
        }
    }
                
    return -1;    
}