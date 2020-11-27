#include <unistd.h>
#include <math.h>
#include <time.h>

#include "./header/timer.h"

#define sampleSize 100
#define multiplier 10000

void main() {
    setup_tmr();
    tmr++;

    struct timeval tv_before, tv_after;

    int i, j;
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
        //printf("The system says %d +- %d usec\n", sum1, sd1);
    }
    /* Control measurement
    int tm;
    tm = *tmr;
    usleep(300);
    tm -= *tmr;
    tm = -tm;
    printf("sleept 300 usec: %d\n", tm);
    */
}
