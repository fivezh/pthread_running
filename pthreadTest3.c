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
		printf("*****pthread1 data change begin, selfData2 = %d\n", selfData);
		selfData=5;
		printf("*****pthread1 data change end, selfData2 = %d\n", selfData);
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
		printf("\nmy pid = %u, i = %d\n", (unsigned int)pThreadId[i], i);	
		//pthread_join(pThreadId[i], NULL);
		//printf("hello end, %d\n", i);
	}

	for (i = 0; i < count; i++)
	{
		pthread_join(pThreadId[i], NULL);
	}
	// sleep(5);

	// int a=2;
	// if (a==1)
	// {
	// 	 int b=3;
	// 	 printf("%d\n", b);
	// }
	// else
	// {
	// 	int b=4;
	// 	printf("%d\n", b);
	// }


	
}