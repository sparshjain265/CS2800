/*
Program		: mergeSort.c
Programmer	: Sparsh Jain
Roll No		: 111601026
Description	: Sorts a given file containing integers in ascending order using mergeSort
Date			: January 9, 2018
*/

#include<stdio.h>
#include<math.h>
#include<limits.h>

FILE * sort(FILE *input);
void merge(FILE *file1, FILE *file2, int offset);

int main()
{
   FILE *input, *sorted, *output;
   int num;

   printf("\n Opening input.bin");

   input = fopen("input.bin", "rb+");

   if(input == NULL)
   {
      printf("\n input.bin not found! Exiting!");
      return 1;
   }

   sorted = sort(input);

   output = fopen("output.txt", "w");

   fseek(sorted, 0, SEEK_SET);
   while(!feof(sorted))
   {
      fread(&num, sizeof(int), 1, sorted);
      fprintf(output, "%d ", num);
   }

   printf("\n \n");
   fclose(input);
   fclose(sorted);
   fclose(output);
   return 0;
}

FILE * sort(FILE *input)
{
   FILE *help, *sorted;
   int offset = 1, i = 0;
   help = fopen("help.bin", "wb+");

   sorted = input;
   for(offset = 1; offset <= INT_MAX; offset *= 2)
   {
      if(i%2 == 0)
      {
         merge(input, help, offset);
         fseek(input, 0, SEEK_SET);
         fseek(help, 0, SEEK_SET);
         sorted = help;
      }
      else
      {
         merge(help, input, offset);
         fseek(input, 0, SEEK_SET);
         fseek(help, 0, SEEK_SET);
         sorted = input;
      }
      i++;
   }

   fclose(help);
   return sorted;
}

void merge(FILE *file1, FILE *file2, int offset)
{
   FILE *help = file1;
   int a, b, i, j;

   fseek(help, offset*sizeof(int), SEEK_SET);
   while(!feof(help))
   {
      fread(&a, sizeof(int), 1, file1);
      fread(&b, sizeof(int), 1, help);

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
            fread(&b, sizeof(int), 1, help);
            j++;
         }

         if(i == offset)
         {
            while(j < offset)
            {
               fwrite(&b, sizeof(int), 1, file2);
               fread(&b, sizeof(int), 1, help);
               j++;
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
      fseek(help, offset*sizeof(int), SEEK_CUR);
   }
}
