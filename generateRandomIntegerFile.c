/*
Program		: generateRandomIntegerFile.c
Programmer	: Sparsh Jain
Roll No		: 111601026
Description	: generates a huge binary file containing random integers
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main()
{
	FILE *p, *q;
	//char name[100];
	int i, num;
	
	//printf("\n Enter the name of the binary file (eg: abc.bin) you wish to generate: ");
	//scanf(" %s", name);
	
	p = fopen("input.bin", "wb");
	q = fopen("input.txt", "w");
	
	srand(time(0));
	
	for(i = 0; i < 10000; i++)
	{
		num = rand();
		//printf("\n %d", num);
		fwrite(&num, sizeof(int), 1, p);
		fprintf(q, "%d ", num);
	}
	
	printf("\n input.bin & input.txt created!");
	fclose(p);
	fclose(q);
	printf("\n\n");
	return 0;
}
