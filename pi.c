#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#include "./header/GPIO.h"
#include "./header/semaphores.h"



int client_sockfd;

void *thread_TurnOnLED(void *arg);
void *thread_read_shm(void *arg);

struct shm_structure {
	unsigned int data[10];
	unsigned int time_stamp[10];
};


int main() {
	//#ServerSocketB
	//Socket setup
	int server_sockfd;
	int server_len, client_len;

	char buffo[100], buff[100];
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(9734);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	//listen to port and wait
	listen(server_sockfd, 5);

	//client connected
	client_len = sizeof(client_address);  // can this be wrong ??
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
	printf("Connected\n");
	//#ServerSocketE

	//#ServerStartThreadB
	//Start threads
	pthread_t readSHMthread;
	pthread_t LEDthread;
	pthread_create(&readSHMthread, NULL, thread_read_shm, NULL);
	pthread_create(&LEDthread, NULL, thread_TurnOnLED, NULL);


	for(;;) {
	}

	close(client_sockfd);
	//#ServerStartThreadE
}
//#ServerSendingThreadB
//THREAD// read the sheared memory and send to pc.c
void *thread_read_shm(void *arg) {
	//shared memory initialization
	void *shared_memory_point1 = (void *) 0;
	void *shared_memory_point2 = (void *) 0;
	struct shm_structure *shmB1, *shmB2;
	int shmid1, shmid2;

	//initialize shared memory 1
	shmid1 = shmget((key_t)1234, sizeof(struct shm_structure), 0666 | IPC_CREAT);
	shared_memory_point1 = shmat(shmid1, (void *)0, 0);
	shmB1 = (struct shm_structure *)shared_memory_point1;

	//initialize shared memory 2
	shmid2 = shmget((key_t)1243, sizeof(struct shm_structure), 0666 | IPC_CREAT);
	shared_memory_point2 = shmat(shmid2, (void *)0, 0);
	shmB2 = (struct shm_structure *)shared_memory_point2;

	//initialize semaphore 1
	int sem_id1 = semget(   (key_t) 1133 ,  1  ,  0666 | IPC_CREAT );

	//initialize semaphore 2
	int sem_id2 = semget(   (key_t) 3311 ,  1  ,  0666 | IPC_CREAT );

	unsigned int data[20];
	for(;;) {
		unsigned int *DataPtr = data;
		(void) semaphore_p(sem_id2);
		unsigned int *shmPtr2 = shmB2->data;
		unsigned int *timePtr2 = shmB2->time_stamp;
		//get data from shared memory, then send to client
		while(shmPtr2 < &shmB2->data[10]) {
			*DataPtr = *timePtr2;
			DataPtr++;
			*DataPtr = *shmPtr2;
			DataPtr++;
			timePtr2++;
			shmPtr2++;
		}
		(void) semaphore_v(sem_id2);
		write(client_sockfd, data, sizeof(data));

		DataPtr = data;
		(void) semaphore_p(sem_id1);
		unsigned int *shmPtr1 = shmB1->data;
		unsigned int *timePtr1 = shmB1->time_stamp;
		//get data from shared memory, then send to client
		while(shmPtr1 < &shmB1->data[10]) {
			*DataPtr = *timePtr1;
			DataPtr++;
			*DataPtr = *shmPtr1;
			DataPtr++;
			timePtr1++;
			shmPtr1++;
		}
		(void) semaphore_v(sem_id1);
		write(client_sockfd, data, sizeof(data));

	}
	//#ServerSendingThreadE
}

//THREAD// turn LED on when recieving correct signal
//#ServerRecievingThreadB
void *thread_TurnOnLED(void *arg) {
	int gpio_pin = 5;
	char recieve[10];
	for(;;) {
		recieve[0] = 'a';
		read(client_sockfd, recieve, 10);
		if(strcmp(recieve, "s") == 0) {
			//GPIO blink LED for 1 sec
			printf("Turning LED on and off!\n");
			setup_io();
			INP_GPIO(gpio_pin);
			OUT_GPIO(gpio_pin);

			GPIO_SET = 1 << gpio_pin;
			sleep(1);
			GPIO_CLR = 1 << gpio_pin;
		}
	}

}
//#ServerRecievingThreadE
