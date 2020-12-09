#include <sys/shm.h>
#include <signal.h>

#include "./header/mcp3008.h"
#include "./header/semaphores.h"

//shared memory declaration
void *shared_memory_point = (void *) 0;
struct shm_structure *shmB;
int shmid;
//#SharedMemoryStrucB
//shared memory structure
//TODO put into own header file
struct shm_structure {
	int data;
};
//#SharedMemoryStrucE

//SPI strucure
struct spi_setting spi_set;
//SPI file discriptor
int fd;

void endfunc(int signo);

int main() {
	//initialize signal handler
	signal(SIGINT, endfunc);

	//initialize semaphore
	sem_id = semget(   (key_t) 1133 ,  1  ,  0666 | IPC_CREAT );
	set_semvalue();

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

	//initialize shared memory
	shmid = shmget((key_t)1243, sizeof(struct shm_structure), 0666 | IPC_CREAT);
	shared_memory_point = shmat(shmid, (void *)0, 0);
	shmB = (struct shm_structure *)shared_memory_point;


	for(;;) {
		//send and recieve bytes to/from ADC
		//#ReadSPIdeviceB
		data[0] = 1; 			//1
		data[1] = 0b10000000;	//selects first input of ADC
		data[2] = 0;			//-
		spiWriteRead(fd, data, sizeof(data), spi_set);
		//merge the incomming data
		int a2dVal = 0;
		a2dVal = data[1] << 8;
		a2dVal |=  data[2];

		printf("%d\n", a2dVal);

		//Save in secured shared memory
		(void) semaphore_p();	//ask for access
		shmB->data = a2dVal;
		(void) semaphore_v();	//free access to shared memory
		//#ReadSPIdeviceE
	}
}

//catch signal function for ctr + c
//DELETE semaphore and shared memory
void endfunc(int signo) {
	del_semvalue();
	shmdt(shared_memory_point);
	shmctl(shmid, IPC_RMID, 0);
	exit(EXIT_SUCCESS);
}
