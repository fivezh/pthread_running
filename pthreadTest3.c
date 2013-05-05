/*****************************************************************************
**    Name:pthreadTest.c
**    testing for multithread in linux
**		
**    Author:fivezh@gmail.com
**    Date:2013-4-15
**    Copyright (c) 2013,All Rights Reserved!
*****************************************************************************/
//http://blog.sina.com.cn/s/blog_70004f4a0100p2e3.html
//http://linux.chinaunix.net/doc/program/2001-08-11/642.shtml
//http://hi.baidu.com/wg_wang/item/b2425f52f6f95c9609be17c8
//http://blog.csdn.net/ljq1203/article/details/7417960
//http://page.renren.com/601478926/note/902273469?ref=hotnewsfeed&sfet=2003&fin=48&fid=22189272901&ff_id=263890731&platform=0&expose_time=1366852240 
//大学毕业出路利弊分析一览表
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
		printf("\n*****pthread0 data change begin, selfData = %d\n", selfData);
		printf("my pid = %u, data is %d\n", (unsigned int)pid, *pDataType);	
		selfData=4;
		printf("*****pthread0 data change end, selfData = %d\n", selfData);
		// sleep(5);
	}
	else if (pid == pThreadId[1])
	{
		int selfData=0;
		printf("\n*****pthread1 data change begin, selfData2 = %d\n", selfData);
		printf("my pid = %u, data is %d\n", (unsigned int)pid, *pDataType);	
		selfData=5;
		printf("*****pthread1 data change end, selfData2 = %d\n", selfData);
		// sleep(2);
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
		printf("my pid = %u, i = %d\n", (unsigned int)pThreadId[i], i);	
	}

	for (i = 0; i < count; i++)
	{
		pthread_join(pThreadId[i], NULL);
		//阻塞主线程，直到等待线程结束，类似方案为pthread_detach()
		printf("join of the pthread: %u\n", (unsigned int)pThreadId[i]);
	}

	printf("end of the main thread\n");
}