#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

void read_roll(char* str, int* d, int* s, int* m);
int roll(int d, int s, int m);

int main(int argc, char* argv[])
{
    if(argc < 2){
        printf("Malformed roll argument: must have form '[num_dice]d[num_sides]+[modifier]'\n");
    } else {
        int d=0, s=0, m=0;
        read_roll(argv[1],&d,&s,&m);
        printf("Total roll: %d\n",roll(d,s,m));
    }
    return EXIT_SUCCESS;
}


// note: NEED TO FIX THIS TO ACCOMODATE NEGATIVE MODIFIERS
void read_roll(char* str, int* d, int* s, int* m)
{
    // this part checks for the case in which arg is formed like 'd+3'.
    int i=0;
    while(*(str+i) != 'd') i++; // find the d
    if(!isdigit(*(str+i+1))){   // if not immediately followed by a digit, then die
        printf("Malformed roll argument: must have form '[num_dice]d[num_sides]+[modifier]'\n");
        exit(1);
    }
    
    /*  How sscanf can fail here :
        - arg is of form 2d+3
            d assigned, s not found so returns 1. never valid.
        - arg is of form d6+2, d+2, or d+.
            returns 0. need to rerun sscanf "d%d+%d". if 0, then invalid.
        - arg is of form 2d6
            returns 2. and should be a valid thing.
    */
    
    int c = sscanf(str,"%dd%d+%d",d,s,m);
    
    if(c<2){
        if(c==1){
            printf("Malformed roll argument: must have form '[num_dice]d[num_sides]+[modifier]'\n");
            exit(1);
        } else if(c==0){
            if((c=sscanf(str,"d%d+%d",s,m)) == 0){
                printf("Malformed roll argument: must have form '[num_dice]d[num_sides]+[modifier]'\n");
                exit(1);
            }
        }
    }
    
    if(c == 3){                                 // 2d20+3
        printf("Rolling %dd%d+%d.\n",*d,*s,*m);
    } else if(c == 2 && *d == 0){               // d20+3
        printf("Rolling d%d+%d.\n",*s,*m);
    } else if(c == 2 && *m == 0){               // 2d20
        printf("Rolling %dd%d.\n",*d,*s);
    } else if(c == 1 && *d == 0 && *m == 0){    // d20
        printf("Rolling d%d.\n",*s);
    } else {
        printf("Malformed roll argument: must have form '[num_dice]d[num_sides]+[modifier]'\n");  
        exit(1);
    }

}

int roll(int d, int s, int m)
{
    int i, new, result=0;
    srand(time(NULL));
    
    if(d == 0) d++;   // set number of rolls to 1 if not set in read_roll
    
    for(i=1;i<=d;i++){
        printf("Roll %d: %d\n",i,(new=rand()%s+1));
        result+=new;
    }
    
    if(m>0) result+=m;
    
    return result;
}
