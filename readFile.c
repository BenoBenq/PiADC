#include <stdio.h>


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
            //printf("Out: %s\n", *(out + k));
            //out++;
            k++;
        }
    }
}

void main() {
    int number_of_lines = 11;
    char data[number_of_lines][1000];
    FILE *fp = fopen("recieve.d", "r");
    tail(fp, number_of_lines, data);
    //char (* ptr) = data;
    //while(ptr < &data[number_of_lines]) {printf("%s\n", ptr); ptr++;}
    int time_value, value;
    for(int i = 0; i < number_of_lines; i++) {
        sscanf(*(data+i), "%d %d", &time_value, &value);
        printf("%d\n", value);
    }
}
