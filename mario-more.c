#include <stdio.h>

int main(void)
{
	int n;
	printf("Enter height:");
	do
	{
		scanf ("%d", &n);
		printf("Enter height:\n");
	}
	while (n < 1 || n > 8);
	
	for (int i = 1; i <= n; i++)
	{
		for (int j = i; j <= n; j++)
		{
			printf(" ");
		}
		for (int k = 1; k <= i; k++)
		{
			printf("#");
		}
		printf("  ");
		for (int k = 1; k <= i; k++)
		{
			printf("#");
		}
		printf("\n");
		
	}
	
	
	
	
}
