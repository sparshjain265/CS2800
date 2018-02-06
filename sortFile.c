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
	FILE *input, *assist, *sorted, *output;
	long long unsigned int offset = 1, i = 0;
	int num;
	
	printf("\n Enter the name of the binary file (eg: abc.bin) you want to sort: ");
	scanf(" %s", fileName);
	
	input = fopen(fileName, "rb+");
	
	if(input == NULL)
	{
		printf("\n File does not exist!");
		return 1;
	}
	
	assist = fopen("assist.bin", "wb+");
	
	sorted = input;
	while(fseek(sorted, offset*sizeof(int), SEEK_SET) == 0)
	{
		if(i%2 == 0)
		{
			merge(input, assist, offset);
			fseek(input, 0, SEEK_SET);
			fseek(assist, 0, SEEK_SET);
			sorted = assist;
		}
		else	
		{
			merge(assist, input, offset);
			fseek(input, 0, SEEK_SET);
			fseek(output, 0, SEEK_SET);
			sorted = input;
		}

		i++;
		offset *= 2;
	}
	
	output = fopen("output.txt", "w");
	fseek(sorted, 0, SEEK_SET);
	while(fread(&num, sizeof(int), 1, sorted) == 1)
		fprintf(output, "%d ", num);

	
	printf("\n\n");
	fclose(input);
	fclose(assist);
	fclose(sorted);
	fclose(output);
	return 0;
}

void merge(FILE *file1, FILE *file2, long long unsigned int offset)
{
	FILE *help1 = file1;
	long long unsigned int i, j;
	int a, b;
	
	while(!feof(help1))
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
		fseek(help1, offset*sizeof(int), SEEK_CUR);
	}
}

