#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "./header/timer.h"

#define sampleSize 1000
#define multiplier 10000
/*
void itoa(int In, char *outp) {
    int k; int Nb, cmp;
    cmp=1;
    Nb=1;
    while(1) {
        //printf("%d,%d\n",In,Nb);
        if(!(In >> Nb)) {
            //printf("break\n");
            break;
        }
        Nb++;
    } // find leftmost nonzero bit
    for(k=Nb-1;k>-1;k--) {
        if (In & cmp<<k) {
            outp[Nb-1-k]='1';
        } else {
            outp[Nb-1-k]='0';
        }
    }
    outp[Nb]=(char)0;
}*/

void main() {
    char buff[100];
    //itoa(8, &buff);
    //printf("%s\n", buff);
    setup_tmr();
    /*
    volatile unsigned *CS, *CLO, *C0;
    CS = tmr;
    *CS = *CS | 15;
    tmr++;
    CLO = tmr;
    tmr++;
    tmr++;
    C0 = tmr;*/
    unsigned int CS, CLO, C0;
    volatile unsigned * tmrpoint;
    //tmrpoint=tmr;      //Read timer registers
    //CS=*tmrpoint;  tmrpoint++;
    //CLO=*tmrpoint; tmrpoint++;
    //tmrpoint++;
    //*tmrpoint = CLO + 1234;
    //C0 = *tmrpoint;


    //printf("%d\n", *C0);
    //printf("%#010x  %#010x  %#010x\n", CS, CLO, C0);
    char buffer[100];
    //itoa(*CS, &buffer);
    //printf("CS: %#010x     CLO: %d     C0: %d\n", CS, CLO, C0);
    //printf("%d\n", *C0);
    unsigned int val;
    //C0 = CLO + 1000;
    //printf("%d\n", *C0);
    int start = 1;
    while(1) {

        tmrpoint=tmr;      //Read timer registers
        printf("%#010x ", tmrpoint);
        CS=*tmrpoint;
        tmrpoint++;
        printf("%#010x ", tmrpoint);
        CLO=*tmrpoint;
        tmrpoint++;
        tmrpoint++;
        printf("%#010x\n", tmrpoint);
        if(start) { *tmrpoint = CLO + 1000; start=0; }
        C0=*tmrpoint;
        printf("CS: %#010x     CLO: %d     C0: %d\n", CS, CLO, C0);
        sleep(1);
    }
    //printf("%d\n", *C0);
    /*
    sleep(4);
    //printf("%d\n", *C0);
    printf("CS: %#010x     CLO: %d     C0: %d\n", *CS, *CLO, *C0);
    //printf("%d\n", *C0);
    */
    C0 = 0;
    //printf("%d\n", *C0);
    /*
    while(1) {
        if(*tmr != 0) {
            printf("Ya\n");
        }
    }*/



    /*
    tmr++;
    struct timeval tv_before, tv_after;

    int i, j;
    FILE *fp = fopen("timingResult.dat", "a");
    fprintf(fp, "Waited _ usec, mean of %d: _ +- _ (tics from system timer from bcm2835)\nWaited _ usec, mean of %d: _ +- _ (useconds measured by linux time)\n\n", sampleSize, sampleSize);
    for(j = 0; j < 10; j = j+1) {
        int times0[sampleSize] = {0};
        int times1[sampleSize] = {0};
        int *ptr0 = times0;
        int *ptr1 = times1;
        for(i = 0; i < sampleSize; i++) {
            int tm;
            tm = *tmr;
            usleep(j*multiplier);
            tm -= *tmr;
            tm = -tm;
            times0[i] = tm;

            gettimeofday(&tv_before, NULL);
            usleep(j*multiplier);
            gettimeofday(&tv_after, NULL);
            times1[i] = (1000000 * tv_after.tv_sec + tv_after.tv_usec) - (1000000 * tv_before.tv_sec + tv_before.tv_usec);
        }
        int sum0 = 0;
        while(ptr0 < &times0[sampleSize]) {
            sum0 += *ptr0;
            ptr0++;
        }
        sum0 = sum0/sampleSize;
        int sum1 = 0;
        while(ptr1 < &times1[sampleSize]) {
            sum1 += *ptr1;
            ptr1++;
        }
        sum1 = sum1/sampleSize;

        float sd0, sd1;
        ptr0 = times0;
        while(ptr0 < &times0[sampleSize]) {
            sd0 += (*ptr0 - sum0)*(*ptr0 - sum0);
            ptr0++;
        }
        ptr1 = times1;
        while(ptr1 < &times1[sampleSize]) {
            sd1 += (*ptr1 - sum1)*(*ptr1 - sum1);
            ptr1++;
        }
        //printf("%d\n", sd);
        sd0 = sqrt(sd0/(sampleSize-1));
        sd1 = sqrt(sd1/(sampleSize-1));


        printf("Waited %d usec, mean of %d: %d +- %f (tics from system timer from bcm2835)\n", j*multiplier, sampleSize, sum0, sd0);
        printf("Waited %d usec, mean of %d: %d +- %f (useconds measured by linux time)\n\n", j*multiplier, sampleSize, sum1, sd1);
        fprintf(fp, "%d %d +- %f\n%d %d +- %f\n\n", j*multiplier, sum0, sd0, j*multiplier, sum1, sd1);
        //printf("The system says %d +- %d usec\n", sum1, sd1);
    }
    fprintf(fp, "\n\n");
    */



    //fclose(fp);

}
