#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lees_bestand.h"

int main(int argc, char *argv[]) {

    char* bestand;
    bestand = argv[1];
    
    char* str = malloc(MAX_GROOTTE);

    // if (!str) {
    //     perror("malloc"); 
    //     return 1; 
    // }

    lees_bestand(bestand, str);
    int rijen = 0, koloms = 0;
    int kolommen_in_rij = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%c", str[i]);
        if (str[i] == '\n') {
            rijen++;
            if (koloms == 0) {
                koloms = i;
            }
            if (kolommen_in_rij != koloms) {
                printf("Ongelijk aantal kolommen in rijen\n");
                return 1;
            }
            kolommen_in_rij = -1;
        }
        kolommen_in_rij++;
    }
    printf("Aantal rijen: %d\n", rijen);
    printf("Aantal kolommen: %d\n", koloms);

    free(str);
    
    return 0;
}

// // char a;
    // // a = 'a';
    // // printf("%c\n", a);

    // // char b;
    // // b = 'b';
    // // printf("%c\n", b);

    // char* ab;
    // ab = "ab";

    // char* bc;
    // bc = "bc";

    // // printf("%s\n", ab);
    // // printf("%s\n", bc);

    // char* array_one[2];
    // array_one[0] = ab;
    // array_one[1] = bc;

    // printf("%c\n", array_one[0][0]);
    // printf("%c\n", array_one[1][0]);

    // for (int i = 0; i < 2; i++) {
    //     printf("%s\n", array_one[i]);
    // }
    // // printf("%s\n", array[0]);
    // // printf("%c\n", ab[0]);

    // char* array_two[2];
    // array_two[0] = "cd";
    // array_two[1] = "ef";