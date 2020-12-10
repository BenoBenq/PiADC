#include <sys/shm.h>
#include <signal.h>

#include "./header/mcp3008.h"
#include "./header/semaphores.h"
#include "./header/timer.h"

//shared memory declaration
void *shared_memory_point1 = (void *) 0;
void *shared_memory_point2 = (void *) 0;
struct shm_structure *shmB1, *shmB2;
int shmid1, shmid2;
//#SharedMemoryStrucB
//shared memory structure
//TODO put into own header file
struct shm_structure {
	unsigned int data[10];
	unsigned int time_stamp[10];
};
//#SharedMemoryStrucE

//SPI strucure
struct spi_setting spi_set;
//SPI file discriptor
int fd, sem_id1, sem_id2;

void endfunc(int signo);

int main() {
	//initialize signal handler
	signal(SIGINT, endfunc);

	//initialize semaphore 1
	sem_id1 = semget(   (key_t) 1133 ,  1  ,  0666 | IPC_CREAT );
	set_semvalue(sem_id1);

	//initialize semaphore 2
	sem_id2 = semget(   (key_t) 3311 ,  1  ,  0666 | IPC_CREAT );
	set_semvalue(sem_id2);

	//#SPISettingB
	//set SPI device configs
	spi_set.device = "/dev/spidev0.0";
	spi_set.mode = 0;
	spi_set.bitsPerWord = 8;
	spi_set.speed = 1000000;
	spi_set.delay = 0;
	//#SPISettingE
	unsigned char data[3];
	//open SPI device on RPi
	fd = spiOpen(spi_set);

	//initialize shared memory 1
	shmid1 = shmget((key_t)1234, sizeof(struct shm_structure), 0666 | IPC_CREAT);
	shared_memory_point1 = shmat(shmid1, (void *)0, 0);
	shmB1 = (struct shm_structure *)shared_memory_point1;

	//initialize shared memory 2
	shmid2 = shmget((key_t)1243, sizeof(struct shm_structure), 0666 | IPC_CREAT);
	shared_memory_point2 = shmat(shmid2, (void *)0, 0);
	shmB2 = (struct shm_structure *)shared_memory_point2;

	//setup timer
	setup_tmr();
	tmr++;
	unsigned int current_time;
	unsigned int start_time = *tmr;
	for(;;) {
    //send and recieve bytes to/from ADC
		//#ReadSPIdeviceB
		unsigned int *shmPtr1 = shmB1->data;
		unsigned int *timePtr1 = shmB1->time_stamp;
		unsigned int *shmPtr2 = shmB2->data;
		unsigned int *timePtr2 = shmB2->time_stamp;
		printf("---1---\n");
		(void) semaphore_p(sem_id1);  //ask for access
		while(shmPtr1 < &shmB1->data[10]) {
			data[0] = 1;          //1
			data[1] = 0b10000000; //selects first input of ADC
			data[2] = 0;          //0
			spiWriteRead(fd, data, sizeof(data), spi_set);
			current_time = *tmr;		//take the current time from system timer
			current_time -= start_time;
			printf("%u ", current_time);
      		//merge the incomming data
			unsigned int a2dVal = 0;
			a2dVal = data[1] << 8;
			a2dVal |=  data[2];

			printf("%u\n", a2dVal);
      		//Save in secured shared memory
	    	*shmPtr1 = a2dVal;
			*timePtr1 = current_time;
			shmPtr1++;
			timePtr1++;
			usleep(100);
			//sleep(1);
		}
		(void) semaphore_v(sem_id1); //free access to shared memory


		printf("---2---\n");
		(void) semaphore_p(sem_id2);  //ask for access
		while(shmPtr2 < &shmB2->data[10]) {
			data[0] = 1;          //1
			data[1] = 0b10000000; //selects first input of ADC
			data[2] = 0;          //0
			spiWriteRead(fd, data, sizeof(data), spi_set);
			current_time = *tmr;		//take the current time from system timer
			current_time -= start_time;
			printf("%u ", current_time);
      		//merge the incomming data
			unsigned int a2dVal = 0;
			a2dVal = data[1] << 8;
			a2dVal |=  data[2];

			printf("%u\n", a2dVal);
      		//Save in secured shared memory
	    	*shmPtr2 = a2dVal;
			*timePtr2 = current_time;
			shmPtr2++;
			timePtr2++;
			usleep(100);
			//sleep(1);
		}
		(void) semaphore_v(sem_id2); //free access to shared memory
    //#ReadSPIdeviceE
	}
}

//catch signal function for ctr + c
//DELETE semaphore and shared memory
void endfunc(int signo) {
	del_semvalue(sem_id1);
	del_semvalue(sem_id2);
	shmdt(shared_memory_point1);
	shmdt(shared_memory_point2);
	shmctl(shmid1, IPC_RMID, 0);
	shmctl(shmid2, IPC_RMID, 0);
	exit(EXIT_SUCCESS);
}
