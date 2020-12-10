#include <sys/sem.h>


/*     This works on all unixes - included Mac (Darwin) which I have tested */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
   /*   semun already done */
#else
/* according to X/OPEN we have to define it ourselves */
    union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
    };
#endif



static int set_semvalue(int sem_id);
static void del_semvalue(int sem_id);
static int semaphore_p(int sem_id);
static int semaphore_v(int sem_id);
//int sem_id;

static int set_semvalue(int sem_id)
{
    union semun sem_union;
    sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
    return(1);
}

static void del_semvalue(int sem_id)
{
    union semun sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        printf("Failed to release semaphore with ID %d\n", sem_id);
    } else {
		printf("Semaphore with ID %d released\n",sem_id);
	}
}

static int semaphore_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;
	int answer;

	semop(sem_id, &sem_b, 1);
}

static int semaphore_v(int sem_id)
{
    struct sembuf sem_b;
    int answer;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;

    semop(sem_id, &sem_b, 1) ;

    return(1);
}
