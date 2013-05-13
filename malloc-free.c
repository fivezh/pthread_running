/*****************************************************************************
**    Name:malloc-free.c
**    malloc/free在多线程下的测试：主线程malloc空间，在子线程中free是否成功?
**		测试结论：主线程和子线程可以分别malloc和free彼此申请的空间
**		20130509：增加mtrance()，编译执行程序后，再使用mtrance output，可查看分析内存
**		malloc/free的情况
**		mtrance output预期的输出为：No memory leaks.
**		存在内存未释放时,mtrance output的输出：
**		Memory not freed:
**		-----------------
**		   Address     Size     Caller
**		0xb14b3008 0x6400000  at 0x80485dd
**		-----------------
**    Author:fivezh@gmail.com
**    Date:2013059
**    Copyright (c) 2013,All Rights Reserved!
*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mcheck.h>

void *fun(void *addp)
{
	int *add;
	add = (int *)addp;

	printf("thread:%d\n", *add);
	*add = 2;
	printf("thread:%d\n", *add);
	free(add); //释放了
	// *add = 5; //虽然释放，但依然可以对指针操作，也就是野指针
}

int main(int argc, char const *argv[])
{
	setenv("MALLOC_TRACE", "output", 1);
	mtrace();
	int *add;
	add = (int *)malloc(1024*1024*100); //100M
	*add =1;
	pthread_t ptid;
	printf("main :%d\n", *add);
	
	if (pthread_create(&ptid,NULL,fun,(void *)add)!=0)
	{
		printf("pthread_create error\n");
	}
	printf("main :%d\n", *add);
	pthread_join(ptid,NULL);
	printf("main :%d\n", *add);

	return 0;
}
