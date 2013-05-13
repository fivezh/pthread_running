/*****************************************************************************
**    Name:pthreadMultiTimers.c
**    多线程与计时器联合方案，解决Linux下默认的alarm()仅能一个执行。
**    此程序实现开启一个线程计时，单独开启线程处理数据。多个计时器线程，实现同类数据的并行处理。
**    应用场景：服务器server中同时处理多副图片，且计时器触发后进行检查和重传操作。
**    Author:fivezh@gmail.com
**    Date:20130505
**		标准输入格式：[0 1 5][1 1 1][1 1 2][1 1 4] [0 2 3][1 2 1][1 2 2]
**		标准输出：1号节点:01204 2号节点：012
**    Copyright (c) 2013,All Rights Reserved!
*****************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_PIC_NUM 100

#ifndef TRUE
#define TRUE 1
	#ifndef FALSE
	#define FALSE 0
	#endif
#endif

typedef struct onePktData
{
	int pktId;
	int received;
	int dataMeta;
}onePktData,*pOnePktData;

typedef struct picStruct
{
	int using;
	int count;//total count of all pkt
	int nodeid;
	onePktData *data;
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
    printf("pthread_detach error!\n");
  }
  picStruct *pdata;
	pdata = (picStruct *)data;
	int i;
	int count = pdata->count;

	pthread_t ptid_timer;//计时器线程
	int err = pthread_create(&ptid_timer,NULL,fun_picDataTimer,(void *)&count);
	if (err != 0)
	{
		printf("ptid_timer thread create error\n");
	}
	int *joinRtn;
	if(0 != pthread_join(ptid_timer,&joinRtn)) //ptid_timer is joinable
	{
		printf("ptid_timer thread join error\n");
	}
	if ((int)joinRtn == 3) //计时器线程完成，开始检查数组和重传报文操作，后续开启重传计时器
	{
		printf("sleep finished of thread:%u\n",(unsigned int)pthread_self());
		//检查报文数组并开始重传，开启重传计时器
		//输出所有报文数据即可
		pOnePktData firstPkt;
		firstPkt = pdata->data;
		for (i = 0; i < count; ++i)
		{
			printf("查验数组：%d\n", (firstPkt+i)->dataMeta);
		}

		//重传完成
		//写入文件

		//释放数组内存
		pdata->using = FALSE;
		free(pdata->data);
	}
}

void *fun_picData(void *data)
{
	int ret_val;
	ret_val= pthread_detach(pthread_self());//detach style thread
  if (ret_val != 0) {
    printf("pthread_join error!\n");
  }
	picStruct *mydata;
	mydata = (picStruct *)data;
	int i = mydata->count; //真实数据
	pOnePktData pktPos;
	pktPos = mydata->data;
	(pktPos+i)->dataMeta = i;
	// printf("dddddddddd:%x\n", (unsigned int)hello);
	// *(hello+1)=*mydata;
	// printf("\n pic data is:%d\n", *mydata);

}


int isPicHandling(picStruct picDataArray[], int node)
{
	int i;
	for (i = 0; i < MAX_PIC_NUM; ++i)
	{
		if (picDataArray[i].nodeid == node && picDataArray[i].using == TRUE )
		{
			return TRUE;
		}
	}
	return FALSE;
}
void *findPicDataArrayUseful(picStruct picDataArray[])
{
	int i;
	for (i = 0; i < MAX_PIC_NUM; ++i)
	{
		if (picDataArray[i].using == FALSE )
		{
			return (void *)&picDataArray[i];
		}
	}
	return FALSE;
}

void *findPicDataArrayPos(picStruct picDataArray[], int node)
{
	int i;
	for (i = 0; i < MAX_PIC_NUM; ++i)
	{
		if (picDataArray[i].nodeid == node && picDataArray[i].using == TRUE )
		{
			return (void *)&picDataArray[i];
		}
	}
	return FALSE;
}
/**
* 主函数，读入数据分配至不同线程处理
**/
int main(int argc, char const *argv[])
{
	// hello = (int *)malloc(10*sizeof(int));
	// *hello = 1;
	// printf("aaaaaaaa:%x\n", (unsigned int)hello);
	pPicStruct startPic;
	picStruct picDataArray[MAX_PIC_NUM];//可同时处理MAX_PIC_NUM个图像，上限
	memset(picDataArray,'\0',sizeof(picDataArray));
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
			if (isPicHandling(picDataArray,pktNodeid) == FALSE) 
			//未在处理此nodeid的picDataInfor
			{
				// startPic = (picStruct *)malloc(sizeof(picStruct));
				startPic = (picStruct *)findPicDataArrayUseful(picDataArray);
				if (startPic == FALSE)
				{
					printf("error:正在传输图片数量超过阈值，分配空间失败@fivezh\n");
					continue;
				}
				// memset(startPic,'\0',sizeof(picStruct));
				// onePktData *ptest = (onePktData *)malloc(sizeof(onePktData)*pktData);
				startPic->data = (onePktData *)malloc(sizeof(onePktData)*pktData);
				//此步骤放在fun_picInfor线程内处理，也可
				startPic->nodeid = pktNodeid;//
				startPic->count = pktData;
				startPic->using = TRUE;
				// printf("%x\n", startPic);
				pthread_t ptid_infor;
				err = pthread_create(&ptid_infor,NULL,fun_picInfor,(void *)startPic);
				if (err != 0)
				{
					printf("ptid_infor pthread create error\n");
					return 1;
				}
			}
			else
			{
				//当前id的picDataInfor已经处理，待处理完成后释放
				printf("当前nodeid的picDataInfor已经在处理，故此报文不做处理\n");
			}
			

		}
		else if (pktType == 1)
		{
			pthread_t ptid_data;
			pOnePktData pApktData;
			pApktData = (onePktData *)malloc(sizeof(onePktData));
			memset(pApktData,'\0',sizeof(onePktData));
			// 找到对应的存储位置，填充数据找给线程处理			
			startPic = (picStruct *)findPicDataArrayPos(picDataArray,pktNodeid);
			if ( startPic == FALSE)
			{
				printf("error:此节点缺乏picDataInfor报文，不做处理@fivezh\n");
				continue;
			}

			//temp
			startPic->count = pktData;

			err = pthread_create(&ptid_data,NULL,fun_picData,(void *)startPic);
			if (err != 0)
			{
				printf("ptid_data pthread create error\n");
				return 1;
			}

		}
		else if (pktType < 0)
		{
			break;
		}
		else
		{
			printf("输入格式错误，请按规定格式输入\n");
			continue;
		}
		
	}

	sleep(1);
	return 0;
}