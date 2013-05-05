/*****************************************************************************
**    Name:pthreadTimers.c
**    多线程与计时器联合方案，解决Linux下默认的alarm()仅能一个执行。
**    此程序实现开启一个线程计时，单独开启线程处理数据。多个计时器线程，实现同类数据的并行处理。
**    应用场景：服务器server中同时处理多副图片，且计时器触发后进行检查和重传操作。
**    Author:fivezh@gmail.com
**    Date:20130504
**    Copyright (c) 2013,All Rights Reserved!
*****************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct pktData
{
	int pktId;
	int received;
	int dataMeta;
}pktData,*pPktData;

typedef struct picStruct
{
	int using;
	int count;//total count of all pkt
	pktData *data;
}picStruct,*pPicStruct;

int *hello;


void *fun_picDataTimer(void *data)
{
	int *sleepTimes;
	sleepTimes = (int *)data;

	sleep(5*(*sleepTimes));
	return (void *)3;
}
/**
* 处理首个图像信息报文
*
* @param void *data 传递数据
*
**/
void *fun_picInfor(void *data)
{
	int ret_val;
	ret_val= pthread_detach(pthread_self());//detach style thread
  if (ret_val != 0) {
    printf("pthread_join error!\n");
  }
  int *pdata;
	pdata = (int *)data;
	int i;
	int count = *pdata;
	hello=(int *)malloc(count*sizeof(int));
	// printf("ppppppppp:%x\n", (unsigned int)hello);
	// printf("\npicDataInfor =%d\n", count);

	pthread_t pid_timer;//计时器线程
	int err = pthread_create(&pid_timer,NULL,fun_picDataTimer,(void *)&count);
	if (err != 0)
	{
		printf("pid_timer thread create error\n");
	}
	int *joinRtn;
	if(0 != pthread_join(pid_timer,&joinRtn))
	{
		printf("pid_timer thread join error\n");
	}
	if ((int)joinRtn == 3) //计时器线程完成，开始检查数组和重传报文操作，后续开启重传计时器
	{
		printf("sleep finished of thread:%u\n",(unsigned int)pthread_self());
		//检查报文数组并开始重传，开启重传计时器
	}


}
void *fun_picData(void *data)
{
	int ret_val;
	ret_val= pthread_detach(pthread_self());//detach style thread
  if (ret_val != 0) {
    printf("pthread_join error!\n");
  }
	int *mydata;
	mydata = (int *)data;
	int i;
	// printf("dddddddddd:%x\n", (unsigned int)hello);
	*(hello+1)=*mydata;
	printf("\n pic data is:%d\n", *mydata);
}


/**
* 主函数，读入数据分配至不同线程处理
**/
int main(int argc, char const *argv[])
{
	// hello = (int *)malloc(10*sizeof(int));
	// *hello = 1;
	// printf("aaaaaaaa:%x\n", (unsigned int)hello);
	while(1)
	{
		int i,count;
		int pktType,pktNodeid,pktData;
		int err;
		printf("请输入数据包:");

		//pktType nodeid count/pktData分别为三个字段的意义
		//pktType字段：0代表picDataInfor，1代表picData
		//nodeid字段：代表不同id的节点
		//count/pktData字段：若pktType==0，则此字段为count，表示总的报文数量；否则为具体的报文数据
		//实例数据如下：
		//0 1 5 //1号节点的infor报文，全部图像数据共分为了5个
		//1 1 3
		//1 1 4等形式
		scanf("%d%d%d",&pktType,&pktNodeid,&pktData);
		if (pktType == 0)
		{
			//thread picDataInfor
			pthread_t ptid;
			err = pthread_create(&ptid,NULL,fun_picInfor,(void *)&pktData);
			if (err != 0)
			{
				printf("pthread create error\n");
			}

		}
		else if (pktType == 1)
		{
			pthread_t ptid_data;

			err = pthread_create(&ptid_data,NULL,fun_picData,(void *)&pktData);
			if (err != 0)
			{
				printf("pthread create error\n");
			}

		}
		if (pktType < 0)
		{
			//end of test
			// printf("xxxxxx:%x\n", (unsigned int)hello);
			// printf("%d\n", *(hello+1)); //此为验证多线程中malloc申请空间后，在线程结束后是否被回收空间。
			break;
		}
	}

	sleep(2);
	return 0;
}