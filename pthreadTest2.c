/*****************************************************************************
**    Name:pthreadTest.c
**    testing for multithread in linux
**		
**    Author:fivezh@gmail.com
**    Date:2013-4-15
**    Copyright (c) 2013,All Rights Reserved!
*****************************************************************************/
#include <pthread.h>  //pthread_
#include <stdio.h>  //printf
#include <unistd.h>  //sleep
struct data
{
	int dataID;
	int realData;
};
int dataType;
pthread_t pThreadId[10];

void *funcPThread(void *var)
{
	int *pDataType;
	pDataType=(int *)var;
	pthread_t pid;
	pid = pthread_self();

	if (pid == pThreadId[0])
	{
		int selfData=0;
		printf("\nmy pid = %u, data is %d\n", (unsigned int)pid, *pDataType);	
		printf("*****pthread0 data change begin, selfData = %d\n", selfData);
		selfData=4;
		printf("*****pthread0 data change end, selfData = %d\n", selfData);
	}
	else if (pid == pThreadId[1])
	{
		int selfData=0;
		printf("\nmy pid = %u, data is %d\n", (unsigned int)pid, *pDataType);	
		printf("*****pthread1 data change begin, selfData = %d\n", selfData);
		selfData=5;
		printf("*****pthread1 data change end, selfData = %d\n", selfData);
	}

	//printf("Common scope selfData = %d\n", selfData);
	
	return (void *)0;
}
void *funcPThread2()
{
	printf("this is another thread function\n");
}
int main(int argc, char **args)
{

	dataType=3;
	int error=pthread_create(&pThreadId[0], NULL, funcPThread, (void *)&dataType);
	if (error)
	{
		printf("pthread_create error\n");
	}

	//sleep(1);
	dataType=5;
	error=pthread_create(&pThreadId[1], NULL, funcPThread, (void *)&dataType);
	if (error)
	{
		printf("pthread_create error\n");
	}
	sleep(2);


	
}