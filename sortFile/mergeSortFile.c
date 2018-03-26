/*
Program		: mergeSortFile.c
Programmer	: Sparsh Jain
Roll No		: 111601026
Description	: Sorts a input.bin containing integers in ascending order using mergeSort into output.txt
Date			: January 9, 2018
*/

#include<stdio.h>

int main()
{
    //file pointers left and right to point to the file being read, sorted to the file being written, and output to the human readable output file
    FILE *left, *right, *sorted, *output;
    //integer cycle to store the number of times 'merging' cycled
    //integer offset is the offset between right and left, the size of the sub array getting merged
    //integers a & b are the two integers being read and compared at a time
    //integers i & j are the two integers are used as counters
    //integer isSorted is used to check whether the file is sorted or not
    int cycle = 0, offset = 0, num, a, b, i, j, isSorted = 0;
    //fileSize stores the sizeof the given binary file as number of bytes
    unsigned long long int fileSize;

    //Do when the list is not sorted
    while(isSorted == 0)
    {
        //if even cycle, read from 'input.bin' and write to 'temp.bin'
        if(cycle%2 == 0)
        {
            //open the files
            left = fopen("input.bin", "rb+");
            right = fopen("input.bin", "rb+");
            if(left == NULL || right == NULL)
            {
                printf("\n Input file error! Try Again!");
                return 1;
            }
            sorted = fopen("temp.bin", "wb+");

            //store the fileSize
            fseek(right, 0, SEEK_END);
            fileSize = ftell(right);
            fseek(right, 0, SEEK_SET);

            //update the offset
            if(offset == 0) offset = 1;
            else offset = offset*2;

            //if offset exceeds the fileSize, the file is sorted
            if(fileSize <= offset*sizeof(int))
            {
                isSorted = 1;
                sorted = left;
                break;
            }   //else update the right pointer
            else fseek(right, offset*sizeof(int), SEEK_CUR);

            //compare till the whole file is read
            while(!feof(right) && !feof(left))
            {
                j = i = offset;

                if(fread(&a, sizeof(int), 1, left) == 1 && fread(&b, sizeof(int), 1, right) == 1)
                {
                    while(j > 0 && i > 0)
                    {
                        if(a < b)
                        {
                            fwrite(&a, sizeof(int), 1, sorted);
                            i--;
                            if(i == 0 || feof(left)) break;
                            if(fread(&a, sizeof(int), 1, left) != 1)    break;
                        }
                        else
                        {
                            fwrite(&b, sizeof(int), 1, sorted);
                            j--;
                            if(j == 0 || feof(right)) break;
                            if(fread(&b, sizeof(int), 1, right) != 1)   break;
                        }
                    }
                }
                if(feof(left) && feof(right)) break;

                if(j == 0 && i != 0)
                {
                    while(i > 0) 
                    {
                        fwrite(&a, sizeof(int), 1, sorted);
                        i--;
                        if(i == 0)  break;
                        else if(fread(&a, sizeof(int), 1, left) != 1)   break;
                    }
                }
                else if(i == 0)
                {
                    while(j > 0)
                    {
                        fwrite(&b, sizeof(int), 1, sorted);
                        j--;
                        if(j == 0)  break;
                        else if(fread(&b, sizeof(int), 1, right) != 1)  break;
                    }
                }
                else if(feof(left)) break;
                else if(feof(right))
                {
                    while(i > 0)
                    {
                        fwrite(&a, sizeof(int), 1, sorted);
                        i--;
                        if(i == 0)  break;
                        else if(fread(&a, sizeof(int), 1, left) != 1) break;
                    }
                }

                if(feof(left) && feof(right)) break;

                fseek(left, offset*sizeof(int), SEEK_CUR);
                fseek(right, offset*sizeof(int), SEEK_CUR);
                if(fileSize <= ftell(left)) break;
            }

            //increment cycle and close the files
            cycle++;
            fclose(left);
            fclose(right);
            fclose(sorted);
        }
        else    //else read from temp.bin and write to input.bin
        {
            left = fopen("temp.bin", "rb+");
            right = fopen("temp.bin", "rb+");
            if(left == NULL || right == NULL)
            {
                printf("\n Input file error! Try Again!");
                return 1;
            }
            sorted = fopen("input.bin", "wb+");

            fseek(right, 0, SEEK_END);
            fileSize = ftell(right);
            fseek(right, 0, SEEK_SET);

            if(offset == 0) offset = 1;
            else offset = offset*2;

            if(fileSize <= offset*sizeof(int))
            {
                isSorted = 1;
                sorted = left;
                break;
            }
            else fseek(right, offset*sizeof(int), SEEK_CUR);

            while(!feof(right) && !feof(left))
            {
                j = i = offset;

                if(fread(&a, sizeof(int), 1, left) == 1 && fread(&b, sizeof(int), 1, right) == 1)
                {
                    while(j > 0 && i > 0)
                    {
                        if(a < b)
                        {
                            fwrite(&a, sizeof(int), 1, sorted);
                            i--;
                            if(i == 0 || feof(left))  break;
                            if(fread(&a, sizeof(int), 1, left) != 1)    break;
                        }
                        else
                        {
                            fwrite(&b, sizeof(int), 1, sorted);
                            j--;
                            if(j == 0 || feof(right)) break;
                            if(fread(&b, sizeof(int), 1, right) != 1)   break;
                        }
                    }
                }

                if(feof(left) && feof(right)) break;

                if(j == 0 && i != 0)
                {
                    while(i > 0) 
                    {
                        fwrite(&a, sizeof(int), 1, sorted);
                        i--;
                        if(i == 0)  break;
                        else if(fread(&a, sizeof(int), 1, left) != 1)   break;
                    }
                }
                else if(i == 0)
                {
                    while(j > 0)
                    {
                        fwrite(&b, sizeof(int), 1, sorted);
                        j--;
                        if(j == 0)  break;
                        else if(fread(&b, sizeof(int), 1, right) != 1)  break;
                    }
                }
                else if(feof(left)) break;
                else if(feof(right))
                {
                    while(i > 0)
                    {
                        fwrite(&a, sizeof(int), 1, sorted);
                        i--;
                        if(i == 0)  break;
                        else if(fread(&a, sizeof(int), 1, left) != 1)  break;
                    }
                }

                if(feof(left) && feof(right)) break;

                fseek(left, offset*sizeof(int), SEEK_CUR);
                fseek(right, offset*sizeof(int), SEEK_CUR);
                if(fileSize <= ftell(left)) break;
            }
            cycle++;
            fclose(left);
            fclose(right);
            fclose(sorted);
        }
    }

    //Loop ends when the file is sorted
    //Created a human readable file
    output = fopen("output.txt", "w");
    while(fread(&num, sizeof(int), 1, sorted)==1)
        fprintf(output, "%d \n", num);
    
    //Closing all the opened files and removing the temporary file created
    fclose(right);
    fclose(left);
    //fclose(sorted);
    fclose(output);
    remove("temp.bin");

    //Notify the user and end program
    printf("\n Created output.txt");
    printf("\n\n");
    return 0;
}