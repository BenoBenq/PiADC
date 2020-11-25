all: pc pi dataAq

pc: pc.c
	gcc -o pc pc.c -lpthread

pi: pi.c
	gcc -o pi pi.c -lpthread

dataAq: dataAq.c
	gcc -o dataAq dataAq.c
