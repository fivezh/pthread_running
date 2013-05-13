/*****************************************************************************
**    Name:pthreadTimerTest.c
**    测试多线程下的sleep/usleep相互干扰效果，验证为线程安全的(10ms级别)
**    Author:fivezh
**    Date:20130506
**    Copyright (c) 2013,All Rights Reserved!
*****************************************************************************/
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
void *thread_child (void *data)
{
   int count = 1;
   struct timeval tStart, tEnd;
   long timeuse;
   while(1)
   {
      count++;
      gettimeofday(&tStart,NULL);//取得开始的时间
      usleep(4000000); // us=microsecond
      //sleep(4);  // s=second
      gettimeofday(&tEnd,NULL);
      //timeuse = 1000000*(tEnd.tv_sec - tStart.tv_sec)   + (tEnd.tv_usec - tStart.tv_usec);
      //printf("time_us:%d\n",timeuse);
      timeuse = 1000*(tEnd.tv_sec - tStart.tv_sec)   + (tEnd.tv_usec - tStart.tv_usec)/1000;
      printf("i'm %u,time_ms:%ld\n",(unsigned int)pthread_self(),timeuse);
   }
}

void *thread_child2 (void *data)
{
   int count = 1;
   struct timeval tStart, tEnd;
   long timeuse;
   while(1)
   {
      count++;
      gettimeofday(&tStart,NULL);//取得开始的时间
      //usleep(1000); 
      sleep(3);
      gettimeofday(&tEnd,NULL);
      //timeuse = 1000000*(tEnd.tv_sec - tStart.tv_sec)   + (tEnd.tv_usec - tStart.tv_usec);
      //printf("time_us:%d\n",timeuse);
      timeuse = 1000*(tEnd.tv_sec - tStart.tv_sec)   + (tEnd.tv_usec - tStart.tv_usec)/1000;
      printf("i'm %u,time_ms:%ld\n",(unsigned int)pthread_self(),timeuse);
   }
}

void *thread_child3 (void *data)
{
   int count = 1;
   struct timeval tStart, tEnd;
   long timeuse;
   while(1)
   {
      count++;
      gettimeofday(&tStart,NULL);//取得开始的时间
      //usleep(1000); 
      sleep(5);
      gettimeofday(&tEnd,NULL);
      //timeuse = 1000000*(tEnd.tv_sec - tStart.tv_sec)   + (tEnd.tv_usec - tStart.tv_usec);
      //printf("time_us:%d\n",timeuse);
      timeuse = 1000*(tEnd.tv_sec - tStart.tv_sec)   + (tEnd.tv_usec - tStart.tv_usec)/1000;
      printf("i'm %u,time_ms:%ld\n",(unsigned int)pthread_self(),timeuse);
   }
}

int main(int argc, char const *argv[])
{
   pthread_t ptid, ptid2, ptid3;
   int err;
   err = pthread_create(&ptid,NULL,thread_child,NULL);
   if (err != 0)
   {
      printf("pthread_create error\n");
   }

   err = pthread_create(&ptid2,NULL,thread_child2,NULL);
   if (err != 0)
   {
      printf("pthread_create error\n");
   }

   err = pthread_create(&ptid3,NULL,thread_child3,NULL);
   if (err != 0)
   {
      printf("pthread_create error\n");
   }
   sleep(21);
   return 0;
}