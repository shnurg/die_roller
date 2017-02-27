#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

char* ERROR   = "Malformed roll argument: must have form '[num_dice]d[num_sides][+/-][modifier]'\n";
char* ROLLING = "Rolling";

void showError()
{
    printf("%s", ERROR);
    exit(1);
}

int isDelimitedCorrectly(char* str, char delimiter)
{
    // this part checks for the case in which arg is formed like 'd+3'.
    // if delimiter is not immediately followed by a digit, then die
    int i = 0;
    while(*(str+i) != delimiter) i++;
    return isdigit(*(str+i+1));
}

int getRollSign(char* str)
{
    char* it = str;
    while(*it++ != '\0') {
        if(*it == '+' || *it == '-') return *it;
    }
    return 'x';
}

void parseRollString(char* str, int* d, int* s, int* m)
{
    if (!isDelimitedCorrectly(str, 'd')) showError();

    // '-' if minus sign found, '+' if plus sign found, 'x' otherwise
    char sign = getRollSign(str);

    /*  How sscanf can fail here :
        - arg is of form 2d+3
            d assigned, s not found so returns 1. never valid.
        - arg is of form d6+2, d+2, or d+.
            returns 0. need to rerun sscanf "d%d+%d". if 0, then invalid.
        - arg is of form 2d6
            returns 2. and should be a valid thing.
    */
    int c = 0;
    if (sign == '+') {
        c = sscanf(str, "%dd%d+%d", d, s, m);
    } else {
        c = sscanf(str, "%dd%d-%d", d, s, m);
    }
    
    // Only 1 token - can't compute
    if (c==1) showError();
    
    // Multiple tokens, but malformed
    if (c < 2 && (
        ((c=sscanf(str, "d%d+%d", s, m)) == 0) ||
        ((c=sscanf(str, "d%d-%d", s, m)) == 0)))
    {
        showError();
    }
    
    if (c == 3) {                                 // 2d20+3
        printf("%s %dd%d%c%d.\n", ROLLING, *d, *s, sign, *m);
    } else if (c == 2 && *d == 0) {               // d20+3
        printf("%s d%d%c%d.\n", ROLLING, *s, sign, *m);
    } else if (c == 2 && *m == 0) {               // 2d20
        printf("%s %dd%d.\n", ROLLING, *d, *s);
    } else if (c == 1 && *d == 0 && *m == 0) {    // d20
        printf("%s d%d.\n", ROLLING, *s);
    } else {
        showError();
    }

}

int computeRollValue(char* str, int d, int s, int m)
{
    int i, currentRoll, totalValue=0;
    srand(time(NULL));
    
    if(d == 0) d++;   // set number of rolls to 1 if not set in parseRollString
    for(i = 1; i <= d; i++) {
        printf("Roll %d: %d\n", i, (currentRoll=rand()%s+1));
        totalValue+=currentRoll;
    }
    
    if (getRollSign(str) == '+') {
        totalValue += m;
    } else {
        totalValue -= m;
    }
    return totalValue;
}

int main(int argc, char* argv[])
{
    if(argc < 2) showError();
    
    int d = 0, s = 0, m = 0;
    parseRollString(argv[1], &d, &s, &m);
    printf("Total roll: %d\n", computeRollValue(argv[1], d, s, m));
    
    return EXIT_SUCCESS;
}
