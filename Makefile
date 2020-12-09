all: pc pi dataAq timingTest

pc: pc.c
	gcc -o pc pc.c -lpthread

pi: pi.c
	gcc -o pi pi.c -lpthread

dataAq: dataAq.c
	gcc -o dataAq dataAq.c

timingTest: timingTest.c
	gcc -o timingTest timingTest.c -lm
