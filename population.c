#include <stdio.h>
int calculateyears(int, int);
int main(void)

{
	int start;
	do {
		printf("Start:");
		scanf("%d",&start);
	}while (start < 9);
	
	int end;
	do {
		printf("End:");
		scanf("%d",&end);
	} while(end < start);
	
	int result = calculateyears(start, end);
	
	printf("years:%d", result);
}

int calculateyears(int start, int end)
{
	int n = start;
	int years = 0;
	while (n < end)
	{
		n = n + n/3 -n/4;
		years++;
	}
	return years;
	
}
