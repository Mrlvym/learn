#include <stdio.h>
#include <string.h>
#include <math.h>


int main ()
{

//冒泡排序
#if 0
	printf("please enter 10 num:");

	int str[10] = {0};

	for(int i = 0; i < 10; i++)
	{
		scanf("%d", &str[i]);
	}	

	for(int i = 0; i < 10; i++)
	{
		printf("str[%d] = %d\n",i ,str[i]);
	}
	printf("\n\n\n");

	for(int i = 0; i < 10-1; i++)
	{
		for(int j = 0; j < 10-1-i; j++)
		{
			if(str[j] < str[j+1])
			{
				int t = 0;
				t = str[j];
				str[j] = str[j+1];
				str[j+1] = t;
			}
		}
	}
	
	for(int i = 0; i < 10; i++)
	{
		printf("str[%d] = %d\n",i ,str[i]);
	}
#endif 

//atoi函数
#if 0
	char str[5] = {0};
	int sum = 0;	

	printf("please enter a string(len <= 5):");

	scanf("%s", str);
	
	printf("%s\nn = %d\n", str, (int)strlen(str));

	for(int i = strlen(str)-1; i >= 0; i--)
	{
		sum += (str[i] - '0') * (int)pow(10 ,strlen(str)-i-1);
	}

	printf("sum = %d\n", sum);
#endif

//
#if 1



#endif



	return 0;
}


