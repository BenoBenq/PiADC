#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#define BCM2708_PERI_BASE        0x3F000000
#define TIMER_BASE                (BCM2708_PERI_BASE + 0x3000)

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *tmr_mem, *tmr_map;
char *spi0_mem, *spi0_map;

// Timer access
volatile unsigned *tmr;
volatile unsigned *tmr_wait;

void setup_tmr()
{

   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }

   /* mmap GPIO */

   // Allocate MAP block
   if ((tmr_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   // Make sure pointer is on 4K boundary
   if ((unsigned long)tmr_mem % PAGE_SIZE)
     tmr_mem += PAGE_SIZE - ((unsigned long)tmr_mem % PAGE_SIZE);

   // Now map it
   tmr_map = (unsigned char *)mmap(
      (caddr_t)tmr_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      TIMER_BASE
   );

   if ((long)tmr_map < 0) {
      printf("mmap error %d\n", (int)tmr_map);
      exit (-1);
   }

   // Always use volatile pointer!
   tmr = (volatile unsigned *)tmr_map;
   tmr_wait = (volatile unsigned *)tmr_map;
}

void wait(int usec, volatile unsigned *tmrOri) {
    volatile unsigned * tmr;
    volatile unsigned * tmrpoint;
    volatile unsigned * CLO;
    volatile unsigned * C1;
    tmr = tmrOri;
    *tmr = *tmr | 15;           //set control to 0
    tmrpoint = tmr;
    tmrpoint++;
    CLO = tmrpoint;
    tmrpoint++; tmrpoint++; tmrpoint++;
    C1 = tmrpoint;
    if(usec > 0) {
        *tmrpoint = *CLO + usec;
    } else {
        *tmrpoint = *CLO + 1;
    }
    for(;;) { /*printf("%d %d %d\n", *tmr, *CLO, *C1); */if(*tmr & 0b10) {break;}}
}
