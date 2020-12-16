#include <stdio.h>

//Returns the last #number_of_lines from file stream fp and save them in out[][]
//Maximum LINE_LENGTH = 1000
void tail(FILE *fp, int number_of_lines, char out[][1000]) {
    int length_of_file = 0;
    int i = 0;
    int k = 0;
    char buff[number_of_lines][1000];
    while(fgets(*(out + k), 1000, fp)) {
        length_of_file++;
    }
    rewind(fp);
    for(i = 0; i < length_of_file; i++) {
        if(i < length_of_file-number_of_lines) {
            fgets(*(out + k), 1000, fp);
        }
        else {
            fgets(*(out + k), 1000, fp);
            k++;
        }
    }
}
