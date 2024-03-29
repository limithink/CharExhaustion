#include <stdlib.h>
#include <stdio.h>

#define FIELDNAME "\t+0\t+1\t+2\t+3\t+4\t+5\t+6\t+7\t+8\t+9\t+A\t+B\t+C\t+D\t+E\t+F"
#define TABSTR "\t"

const char* g_fieldName = FIELDNAME;
const int g_lenFieldName = sizeof(FIELDNAME) - 1;
const int g_lenStrTab = sizeof(TABSTR) - 1;

char* Byte2Hex(unsigned char* pByte, int isLE, int isLower)
//isLE controls Hex String of return to be Stored as Big-Endian or Little-Endian
//isLower controls Hex String of return to be Uppercase(A-F) or Lowercase(a-f)
{
	static char Hex[2] = "";
	if (isLE) {//Little-Endian
		//High 4-bit
		Hex[1] = (*pByte >> 4) & 0x0F;
		//Low 4-bit
		Hex[0] = *pByte & 0x0F;
	}
	else {//Big-Endian
		//High 4-bit
		Hex[0] = (*pByte >> 4) & 0x0F;
		//Low 4-bit
		Hex[1] = *pByte & 0x0F;
	}
	for (int ctr = 0; ctr < 2; ctr++)
	{
		if (Hex[ctr] < 0xA)
			Hex[ctr] += 0x30;
		else {
			//A-F or a-f
			if (isLower)
				Hex[ctr] += 0x57;//Lowercase
			else
				Hex[ctr] += 0x37;//Uppercase
		}
	}
	return Hex;
}

void Int2ByteArr(unsigned char* byteArr, int byteNum, size_t intVal, int isLE)
//isLE controls Byte Array of return to be Stored as Big-Endian or Little-Endian
{
	int start, end, inc;
	if (isLE) {
		//Little-Endian
		start = 0;
		end = byteNum;
		inc = 1;
	}
	else {
		//Big-Endian
		start = byteNum - 1;
		end = -1;
		inc = -1;
	}
	for (int ctr = start; ctr != end; ctr += inc, intVal >>= 0x8)
		*(byteArr + ctr) = (unsigned char)(intVal & 0xFF);
	return;
}

void memDstCurCpy(void** pCursor, void* src, int cpySize)
{
	memcpy(*pCursor, src, cpySize);
	*((char**)pCursor) += cpySize;
}

void ExhaustProc(size_t byteNum, FILE* fp, char* pLFstr, int lenStrLF, int isLE)
//isLE controls Bytes Order of Single Charactor to be Little-Endian or Big-Endian
{
	//FieldRow of char Block Constructing
	size_t lenFieldRow = byteNum * 2 + g_lenFieldName;
	char* strFieldRow = (char*)calloc(lenFieldRow, sizeof(char));
	for (int ctr = 0; ctr < byteNum; ctr++)
	{
		*(strFieldRow + ctr) = 0x20;
		*(strFieldRow + ctr + byteNum) = 0x20;
	}
	memcpy(strFieldRow + byteNum * 2, g_fieldName, g_lenFieldName);
	//Allocating Buffer of char Block 
	size_t lenCharBlk = lenFieldRow + lenStrLF + \
		(byteNum * 2 + (g_lenStrTab + byteNum) * 16 + lenStrLF) * 16 + \
		lenStrLF;
	char* blk = (char*)calloc(lenCharBlk, sizeof(char));
	char* base = blk;
	//Write Field Row to File and LineFeed of End to char Block Buffer
	memDstCurCpy(&base, strFieldRow, lenFieldRow);
	memDstCurCpy(&base, pLFstr, lenStrLF);
	free(strFieldRow);//free Mem used for Constructing Field Row
	if (byteNum > 1)
	{//Exhausting Characters of 2 Bytes or more 
		//Allocating Buffer of Byte Array and Corresponding Hex Array
		size_t byteNum_ = byteNum - 1;
		size_t byteNum_D = byteNum_ * 2;
		unsigned char* byteArr = (unsigned char*)calloc(byteNum_, sizeof(unsigned char));
		char* hexArr = (char*)calloc(byteNum_D, sizeof(char));
		//Iteration of a char Block
		for (size_t counter = 0; counter < 0x1 << (byteNum_ * 8); counter++)
		{
			char* cursor = base;
			//Integer Counter => Byte Array
			Int2ByteArr(byteArr, byteNum_, counter, isLE);
			//Byte Array => Hex String
			if (isLE) {
				//Little-Endian Bytes Order of Single Charactor
				for (int ctr = byteNum_ - 1; ctr >= 0; ctr--)
					memcpy(hexArr + ctr * 2, Byte2Hex(byteArr + ctr, NULL, NULL), 2);
			}
			else {
				//Big-Endian Bytes Order of Single Charactor
				for (int ctr = 0; ctr < byteNum_; ctr++)
					memcpy(hexArr + ctr * 2, Byte2Hex(byteArr + ctr, NULL, NULL), 2);
			}
			//Row Iteration
			for (int row = 0x0; row < 0x100; row += 0x10)
			{
				//Write Hex String in each Row Head
				memDstCurCpy(&cursor, hexArr, byteNum_D);
				memDstCurCpy(&cursor, Byte2Hex(&row, NULL, NULL), 2);
				//Col Iteration
				for (int col = 0x0; col < 0x10; col++)
				{
					memDstCurCpy(&cursor, TABSTR, g_lenStrTab);
					unsigned char endian = (unsigned char)(row + col);
					if (isLE) {
						//Little-Endian Bytes Order of Single Charactor
						memDstCurCpy(&cursor, &endian, 1);
						memDstCurCpy(&cursor, byteArr, byteNum_);
					}
					else {
						//Big-Endian Bytes Order of Single Charactor
						memDstCurCpy(&cursor, byteArr, byteNum_);
						memDstCurCpy(&cursor, &endian, 1);
					}
				}
				//Write LineFeed to File of Each Row End
				memDstCurCpy(&cursor, pLFstr, lenStrLF);
			}
			//Write LineFeed to File of Each Char Block End
			memDstCurCpy(&cursor, pLFstr, lenStrLF);
			//Buffer => File
			fwrite(blk, sizeof(char), lenCharBlk, fp);
		}
		free(byteArr);
		free(hexArr);
	}
	else
	{//Exhausting Characters of Only a Byte
		//Row Iteration
		for (int row = 0x0; row < 0x100; row += 0x10)
		{
			//Write Hex String in each Row Head
			memDstCurCpy(&base, Byte2Hex(&row, NULL, NULL), 2);
			//Col Iteration
			for (int col = 0x0; col < 0x10; col++)
			{
				memDstCurCpy(&base, TABSTR, g_lenStrTab);
				unsigned char endian = (unsigned char)(row + col);
				memDstCurCpy(&base, &endian, 1);
			}
			//Write LineFeed to File of Each Row End
			memDstCurCpy(&base, pLFstr, lenStrLF);
		}
		//Write LineFeed to File of Each Char Block End
		memDstCurCpy(&base, pLFstr, lenStrLF);
		//Buffer => File
		fwrite(blk, sizeof(char), lenCharBlk, fp);
	}
	free(blk);
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
	for (int ctr = 1; ctr <= byteNum; ctr++)
	{
		fprintf(fp, "CharExhaustion of %d Byte:\r\n\r\n", ctr);
		ExhaustProc(ctr, fp, "\r\n", sizeof("\r\n") - 1, NULL);
		fprintf(fp, "\r\n");
	}
	fclose(fp);
	printf("Complete");
	return 0;
}
