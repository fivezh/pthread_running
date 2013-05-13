#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
	pid_t pid;

	printf("hello\n");
	pid=fork();
	if (pid<0)
	{
		printf("fork error\n");
	}
	else if (pid == 0)
	{
		printf("here is sub process\n");
		return 0;
	}
	else
		printf("here is main process\n");
	printf("end\n");
	return 0;
}