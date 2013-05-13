#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>
int main(int argc, const char *argv[])
{
	setenv("MALLOC_TRACE", "test.log", 1); //增加的设置环境变量代码
  mtrace(); //检测起点
  int *a = (int *)malloc(10);
  muntrace(); //检测终点,检测到程序末尾时可以省略不写这个
  return 0;
}