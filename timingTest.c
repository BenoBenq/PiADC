#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "./header/timer.h"

//#define sampleSize 1000
//#define multiplier 10000
#define numberOfSamples 4
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
    int sampleSizeList[numberOfSamples] = {10, 100, 1000, 10000};
    int multiplierList[numberOfSamples] = {10000, 1000, 100, 10};
    char buff[100];

    setup_tmr();
    //volatile unsigned *waitPtr = tmr;
    tmr++;
    struct timeval tv_before, tv_after;

    int i, j, k, m;
    FILE *fp = fopen("timingResult.dat", "w");
    //fprintf(fp, "Waited _ usec, mean of %d: _ +- _ (tics from system timer from bcm2835)\nWaited _ usec, mean of %d: _ +- _ (useconds measured by linux time)\n\n", sampleSize, sampleSize);
    for(k = 0; k < numberOfSamples; k++) {
        int sampleSize = sampleSizeList[k];
        int multiplier = multiplierList[k];
        int times[4][sampleSize];
        memset(times, 0, 4*sampleSize*sizeof(int));
        for(j = 0; j < 10; j++) {
            for(i = 0; i < sampleSize; i++) {
                int tm;
                //take time of usleep with system counter
                tm = *tmr;
                usleep(j*multiplier);
                tm -= *tmr;
                tm = -tm;
                times[0][i] = tm;
                //take time of usleep with linux system time
                gettimeofday(&tv_before, NULL);
                usleep(j*multiplier);
                gettimeofday(&tv_after, NULL);
                times[1][i] = (1000000 * tv_after.tv_sec + tv_after.tv_usec) - (1000000 * tv_before.tv_sec + tv_before.tv_usec);

                //take time of custom wait with system counter
                tm = *tmr;
                wait(j*multiplier, tmr_wait);
                tm -= *tmr;
                tm = -tm;
                times[2][i] = tm;
                //take time of custom wait with linux system time
                gettimeofday(&tv_before, NULL);
                wait(j*multiplier, tmr_wait);
                gettimeofday(&tv_after, NULL);
                times[3][i] = (1000000 * tv_after.tv_sec + tv_after.tv_usec) - (1000000 * tv_before.tv_sec + tv_before.tv_usec);
            }

            //Calculating the average and standard deviation
            int sum[4] = {0};
            float sd[4] = {0};
            int *ptr[4] = {times[0], times[1], times[2], times[3]};
            for(m = 0; m < 4; m++){
                while(ptr[m] < &times[m][sampleSize]) {
                    sum[m] += *ptr[m];
                    ptr[m]++;
                }
                sum[m] = sum[m]/sampleSize;
                ptr[m] = times[m];
                while(ptr[m] < &times[m][sampleSize]) {
                    sd[m] += (*ptr[m] - sum[m])*(*ptr[m] - sum[m]);
                    ptr[m]++;
                }
                sd[m] = sqrt(sd[m]/(sampleSize-1));
            }
            int h;
            for(h = 0; h < 4; h++) {printf("%d +- %0.3f ",sum[h], sd[h]);} printf("\n");

            //printf("Waited %d usec, mean of %d: %d +- %f (tics from system timer from bcm2835)\n", j*multiplier, sampleSize, sum0, sd0);
            //printf("Waited %d usec, mean of %d: %d +- %f (useconds measured by linux time)\n\n", j*multiplier, sampleSize, sum1, sd1);
            //fprintf(fp, "%d %d +- %f\n%d %d +- %f\n\n", j*multiplier, sum0, sd0, j*multiplier, sum1, sd1);
            //printf("The system says %d +- %d usec\n", sum1, sd1);
        }
        printf("\n");
    }
    //fprintf(fp, "\n\n");




    fclose(fp);
}
