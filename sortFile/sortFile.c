/*
Program		: sortFile.c
Programmer	: Sparsh Jain
Roll No		: 111601026
Description	: Sorts a given file containing integers in ascending order using mergeSort
Date			: January 9, 2018
*/

#include<stdio.h>
#include<math.h>

void merge(FILE *file1, FILE *file2, long long unsigned int offset);

int main()
{	
	char fileName[100];
	FILE *file1, *file2, *f;
	long long unsigned int offset = 1, i;
	int a, b;
	
	printf("\n Enter the name of the binary file (eg: abc.bin) you want to sort: ");
	scanf(" %s", fileName);
	
	file1 = fopen(fileName, "rb+");
	
	if(file1 == NULL)
	{
		printf("\n File does not exist!");
		return 1;
	}
	
	file2 = fopen("assist.bin", "wb+");
	
	f = file1;
	while(fseek(f, offset*
	merge(file1, file2, offset);	
	
	printf("\n\n");
	return 0;
}

void merge(FILE *file1, FILE *file2, long long unsigned int offset)
{
	FILE *help1 = file1;
	long long unsigned int i, j;
	int a, b;
	
	while(fseek(help1, offset*sizeof(int), SEEK_CUR) == 0)
	{		
		fread(&a, sizeof(int), 1, file1);
		fread(&b, sizeof(int), 1, help1);
		
		i = j = 0;
		while(i < offset || j < offset)
		{
			if(a < b)
			{
				fwrite(&a, sizeof(int), 1, file2);
				fread(&a, sizeof(int), 1, file1);
				i++;
			}
			else
			{
				fwrite(&b, sizeof(int), 1, file2);
				if(fread(&b, sizeof(int), 1, help1) != 1)
					j = offset;
				else j++;
			}
		
			if(i == offset)
			{
				while(j < offset)
				{
					fwrite(&b, sizeof(int), 1, file2);
					if(fread(&b, sizeof(int), 1, help1) != 1)
						j = offset;
					else j++;
				}
			}
		
			if(j == offset)
			{
				while(i < offset)
				{
					fwrite(&a, sizeof(int), 1, file2);
					fread(&a, sizeof(int), 1, file1);
					i++;
				}
			}
		}
		fseek(file1, offset*sizeof(int), SEEK_CUR);
	}
}


