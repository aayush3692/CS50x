#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int computeScore(char []);
void main()
{
	char word1[100], word2[100];
	printf("Player 1: ");
	fflush(stdin);
	gets(word1);
	printf("Player 2: ");
	gets(word2);
	
	int score1 = computeScore(word1);
	int score2 = computeScore(word2);
	
	if(score1 > score2)
	{
		printf("Player 1 wins!! \n");
	}
	else if(score2 > score1)
	{
		printf("Player 2 wins!! \n");
	}
	else
	{
		printf("It's a tie! \n");
	}
	getch();
}
//  Compute and return score for string
int computeScore(char word[])
{
	int i, score = 0;
	for(i = 0; i < strlen(word); i++)
	{
		if(isupper(word[i]))
		{
			score = score + POINTS[word[i] - 65];
		}
		if(islower(word[i]))
		{
			score = score + POINTS[word[i] - 97];
		}
		
	}
	return score;
}
