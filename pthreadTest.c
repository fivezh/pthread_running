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

	int selfData=0;
	if (pid == pThreadId[0])
	{
		printf("\n*****pthread0 data change begin, selfData = %d\n", selfData);
		selfData=4;
		printf("my pid = %u, data is %d\n", (unsigned int)pid, *pDataType);	
		printf("*****pthread0 data change end, selfData = %d\n", selfData);
	}
	else if (pid == pThreadId[1])
	{
		printf("\n*****pthread1 data change begin, selfData = %d\n", selfData);
		selfData=5;
		printf("my pid = %u, data is %d\n", (unsigned int)pid, *pDataType);	
		printf("*****pthread1 data change end, selfData = %d\n", selfData);
	}
	//线程对多线程共享的数据可能污读

	printf("Common scope selfData = %d\n", selfData);
	
	return (void *)0;
}
void *funcPThread2()
{
	printf("this is another thread function\n");
}
int main(int argc, char **args)
{
	int error;
	int count=2;
	int i;
	for (i = 0; i < count; i++)
	{
		dataType=i;
		error=pthread_create(&pThreadId[i], NULL, funcPThread, (void *)&dataType);
		if (error)
		{
			printf("pthread create error\n");
		}
		pthread_join(pThreadId[i], NULL);
		printf("hello end\n");
	}

	
}