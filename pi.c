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
	int data;
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
	void *shared_memory_point = (void *) 0;
	struct shm_structure *shmB;
	int shmid;

	shmid = shmget((key_t)1243, sizeof(struct shm_structure), 0666 | IPC_CREAT);
	shared_memory_point = shmat(shmid, (void *)0, 0);
	shmB = (struct shm_structure *)shared_memory_point;

	//initialize semaphore
	sem_id = semget(   (key_t) 1133 ,  1  ,  0666 | IPC_CREAT );

	for(;;) {
		//get data from shared memory, then send to client
		(void) semaphore_p();
		write(client_sockfd, &shmB->data, sizeof(data));
		usleep(100);
		(void) semaphore_v();
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
