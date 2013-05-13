#include <stdio.h>

void bubbleSort(int *arr, int count)
{
	int tem;
	int i, j;

	/*
	//Type 1: i means which is the current step
	for (i=0; i<count-1; i++)
	{
		for (j=0; j<count-1-i; j++)
		{
			if (arr[j] > arr[j+1])
			{
			tem = arr[j+1];
			arr[j+1] = arr[j];
			arr[j] = tem;
			}
		}
	}
	//end Type1
	*/

	//Type2: i means the last number index
	i = count-1;
	while (i)
	{
		int k=100;
		int *p;

		for (j=0; j<i; j++)
		{
			if (arr[j] > arr[j+1])
			{
				tem = arr[j+1];
				arr[j+1] = arr[j];
				arr[j] = tem;
			}
		}
		i--;
	}
	//end Type2
}

int main(int argc, int **argv)
{
	// 测试数据
	int testArray[] = {5, 4, 1, 3, 6, 10, 22, 12, 33, 11, 7, 8, 9};

	int count = sizeof(testArray)/sizeof(int);
	int i;

	for(i=0; i< count; i++)
	{
		printf("%d ",testArray[i]);
	}
	printf("\n");
	
	
	//开始计时

	//冒泡排序
	bubbleSort(testArray, count);

	//结束计时

	for(i=0; i< count; i++)
	{
		printf("%d ",testArray[i]);
	}

	getchar();
	return 1;
}