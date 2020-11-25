#include <stdlib.h> 
#include <fcntl.h>
#include <sys/mman.h> 
#include <stdio.h>
 
#define BCM2708_PERI_BASE 0x3F000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000)
#define PAGE_SIZE (4*1024) 
#define BLOCK_SIZE (4*1024)

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3)) 
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3)) 
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio + 7) 
#define GPIO_CLR *(gpio + 10)

int  mem_fd; 
char *gpio_mem, *gpio_map; 
char *spi0_mem, *spi0_map; 
volatile unsigned *gpio;

void setup_io() 
{        
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {   
		printf("can't open /dev/mem \n");   
		exit (-1);    
	}    
	if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {   
		printf("allocation error \n");   
		exit (-1);    
	}    
	if ((unsigned long)gpio_mem % PAGE_SIZE)  gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);    
   
	gpio_map = (unsigned char *)mmap(   (caddr_t)gpio_mem,   BLOCK_SIZE,   PROT_READ|PROT_WRITE,   MAP_SHARED|MAP_FIXED,   mem_fd,   GPIO_BASE    );    
	if ((long)gpio_map < 0) {   
		printf("mmap error %d\n", (int)gpio_map);   
		exit (-1);    
	}    

gpio = (volatile unsigned *)gpio_map;
}
