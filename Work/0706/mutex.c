#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100

void *thead_inc(void *arg);
void *thread_des(void * arg);

long long num = 0;
pthread_mutex_t mutex;

int main(int argc, char *atgv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	pthread_mutex_init(&mutex, NULL);

	for(i=0;i<NUM_THREAD; i++)
		pthread_join(thread_id[i],NULL);

	printf("result: %lld \n",num);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void *thread_inc(void *arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i = 0; i<50000000; i++)
		num+=1;
	pthread_mutex_unlock(&mutex);
	return NULL;
}
void * thread_des(void *arg)
{
	int i;
	for(i = 0; i<50000000; i++)
	{
		pthread_mutex_lock(&mutex);
		num-=1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
