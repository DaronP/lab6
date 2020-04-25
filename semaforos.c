#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int i;

pthread_mutex_t threadLock;

sem_t start;
sem_t noRes;

FILE * fil;

int randomResource;
int rsrc;

void *tMethod(void *argC)
{
	fprintf(fil, "Thread created\n");
	sem_wait(&start);
	
	int val = 12;
	int cont = 0;
	
	if(i == 12)
	{
		for(cont = 0; cont < val; cont++)
		{
			fprintf(fil, "Esperando...\n");
			sem_wait(&noRes);
		}
	}
	else
	{
		sem_post(&start);
	}
}

void *resourse(void *argC)
{
	int semCount = 0;

	fprintf(fil ,"Thread2 created\n");
	while(1)
	{
		rsrc++;
		if(rsrc < randomResource)
		{
			semCount += 1;
			fprintf(fil, "Created semafore %i", semCount);
			sleep(1);
		}
		rsrc = 0;
		fprintf(fil, "Liberando recursos...\n");
		sem_wait(&noRes);
		
		//Locking thread
		pthread_mutex_lock(&threadLock);
		fprintf(fil, "Working...\n");
		pthread_mutex_unlock(&threadLock);
		
	}
}


int main()
{
	int i;
	int count = 12;
	int count2 = 24;
	randomResource = rand()% 3;
	
	pthread_mutex_init(&threadLock, NULL);
	sem_init(&start, 0, 1);
	
	//Creating log file
	
	fil = fopen("./log.txt", "w");
	
	pthread_t * threadId = malloc(sizeof(pthread_t)*count2);
	
	for(i = 0; i < count; i++)
	{
		pthread_create(&threadId[i], NULL, tMethod, NULL);
		sleep(1);
	}
	
	for(i = count; i < count2; i++)
	{
		pthread_create(&threadId[i], NULL, resourse, NULL);
	}
	
	
	for(i = 0; i < count2; i++)
	{
		pthread_join(threadId[i], NULL);
	}
	
	pthread_mutex_destroy(&threadLock);
	sem_destroy(&start);
	sem_destroy(&noRes);
	
	fclose(fil);
	return 0;
	
}
























