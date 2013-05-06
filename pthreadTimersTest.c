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
      usleep(1000); //sleep(10);
      gettimeofday(&tEnd,NULL);
      timeuse = 1000000*(tEnd.tv_sec - tStart.tv_sec)   + (tEnd.tv_usec - tStart.tv_usec);
      printf("time_us:%d\n",timeuse);
      //timeuse = 1000*(tEnd.tv_sec - tStart.tv_sec)   + (tEnd.tv_usec - tStart.tv_usec)/1000;
      //printf("time_ms:%d\n",timeuse);
   }
}
int main(int argc, char const *argv[])
{
   pthread_t ptid;
   int err;
   err = pthread_create(&ptid,NULL,thread_child,NULL);
   if (err != 0)
   {
      printf("pthread_create error\n");
   }
   return 0;
}