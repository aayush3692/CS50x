#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char argv[])
{
      // Make sure program was run with just one command-line argument
      if(argc != 2)
      {
      	printf("Usage: ./caesar key\n ");
      	return 1;
      }
    // Make sure every character in argv[1] is a digit
    int i;
    for(i = 0; i < strlen(argv[1]); i++)
    {
    	if(!isdigit(argv[1][i]))
    	{
    		printf("Usage: ./caesar key\n ");
      		return 1;
		}
	}
    // Convert argv[1] from a `string` to an `int`
	int k = atoi(argv[i]);
    // Prompt user for plaintext
    char plaintext[100], ciphertext[100];
	printf("Plaintext: ");
	fflush(stdin);
	gets(plaintext);
	printf("Ciphertext: ");
    // For each character in the plaintext:
	int j;
	for(j= 0; j < strlen(plaintext); j++)
	{
		// Rotate the character if it's a letter
		if(isupper(plaintext))
		{
			printf("%c", ((plaintext[j] - 65) + k) % 26 + 65);
		}
		else if(islower(plaintext))
		{
			printf("%c", ((plaintext[j] - 97) + k) % 26 + 97);
		}
		else
		{
			printf("%c", plaintext[j]);
		}
		
	}
        
}

