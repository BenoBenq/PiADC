#include <sys/shm.h>
#include <signal.h>


#include "./header/mcp3008.h"
#include "./header/semaphores.h"



struct spi_setting spi_set;
int fd;
void *shared_memory_point = (void *) 0;
struct shm_structure *shmB;
int shmid;

struct shm_structure {
	int data[10];
};

void endfunc(int signo);

int main() {
    signal(SIGINT, endfunc);


    sem_id = semget(   (key_t) 1133 ,  1  ,  0666 | IPC_CREAT );
    set_semvalue();



    spi_set.device = "/dev/spidev0.0";
	spi_set.mode = 0;
	spi_set.bitsPerWord = 8;
	spi_set.speed = 1000000;
	spi_set.delay = 0;

    unsigned char data[3];
	fd = spiOpen(spi_set);


    shmid = shmget((key_t)1243, sizeof(struct shm_structure), 0666 | IPC_CREAT);
    shared_memory_point = shmat(shmid, (void *)0, 0);
    shmB = (struct shm_structure *)shared_memory_point;

	while(1) {
		int *shmPtr = shmB->data;
		(void) semaphore_p();
		while(shmPtr < &shmB->data[10]) {
			data[0] = 1;
			data[1] = 0b10000000;
			data[2] = 0;
			spiWriteRead(fd, data, sizeof(data), spi_set);
			int a2dVal = 0;
			a2dVal = data[1] << 8;
			a2dVal |=  data[2];
			printf("%d\n", a2dVal);

	    	*shmPtr = a2dVal;
			shmPtr++;
		}
		(void) semaphore_v();
	}
}

void endfunc(int signo) {
    del_semvalue();
    shmdt(shared_memory_point);
    shmctl(shmid, IPC_RMID, 0);
    exit(EXIT_SUCCESS);
}
