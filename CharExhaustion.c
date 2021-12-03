#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FIELDNAME "\t+0\t+1\t+2\t+3\t+4\t+5\t+6\t+7\t+8\t+9\t+A\t+B\t+C\t+D\t+E\t+F"

//void ByteDiv(char* dest, void* src, int nBytes)
char* curMenCpy(, resetFlag)
{
	static void* cursor = 0;

}

void ExhaustProc(int byteNum, FILE* fp, char* pLFstr, int sizeLFstr)
{
	//int ceil = 1;
	int blkCharNum = (byteNum * 2 + (sizeof("\t") - 1 + byteNum) * 16 + sizeof("\n")) * 16;
	int sizeFieldRow = byteNum * 2 + sizeof(FIELDNAME)
	char* strFieldRow = (char*)calloc(sizeFieldRow, sizeof(char));
	for (int ctr = 0; ctr < byteNum; ctr++)
	{
		//ceil *= 0x100
		//*(byteArr + ctr) = 0x00;
		*(strFieldRow + ctr) = 0x20;
		*(strFieldRow + ctr + byteNum) = 0x20;
	}
	char* fieldName = FIELDNAME;
	memcpy(strFieldRow + byteNum * 2, fieldName, sizeof(FIELDNAME));
	/*
	for (int ctr = 0, char* src = FIELDNAME, char* dest = strFieldRow + byteNum * 2;
	ctr < sizeof(FIELDNAME);
	ctr++, src++, dest++)
		*dest = *src;
	*/
	char* blk = (char*)calloc(blkCharNum, sizeof(char));
	unsigned char* byteArr = (unsigned char*)calloc(byteNum - 1, sizeof(unsigned char));
	for (long int counter = 0; counter < 0x1 << (4 * (byteNum - 1)); counter++)
	{
		int cpySize = 0;
		char* cursor = blk;
		//unsigned char* anchor = &counter;
		memcpy(byteArr, &counter, byteNum * sizeof(unsigned char));
		//Integer Counter => Byte

		//Row Iteration
		//Write Field Row to File
		cpySize = sizeof(char) * (sizeFieldRow - 1);
		memcpy(cursor, strFieldRow, cpySize);
		cursor += cpySize;
		for (int row = 0x0, unsigned char endian = 0x0; row < 0x10; row++)
		{
			for (int col = 0x0; col < 0x10; col++, endian++)
			{
				
			}
		}
		//Write Line Feed Symbol to File
		//fwrite(pLFstr, sizeLFstr, 1, fp)
		cpySize = sizeof(char) * 
	}
	
}

int main(int argc, char** argv)
{
	int byteNum;
	if (argc == 1)
	{
		printf("Input Num of Char for Exhausting: ");
		scanf_s("%d", &byteNum, 2);
	}
	else
	{
		byteNum = atoi(argv[1]);
	}
	printf("\n%d Bytes Chars Exhausting...", byteNum);
	FILE* fp;
	fp = fopen("CharExhaustion.txt", "wb");
	if (!fp)
	{
		printf("\nCreating File Failed!!!");
		return 0;
	}
	for (int ctr = 0; ctr < byteNum; ctr++)
	{
		fprintf(fp, "CharExhaustion of %d Byte:\n\n", ctr);
		ExhaustProc(ctr + 1, fp)
		fprintf(fp, "\n\n\n");
	}
	fclose(fp);
	printf("Complete")
	return 0;
}