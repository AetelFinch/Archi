#include "tree.h"
#include "list.h"

int freqBytes[256]; //частота появления байтов в файле
List santinel;
unsigned char notZero; //кол-во уникальных байтов
int totalBytes; //кол-во байт во входном файле

//счет частоты появления каждого байта в файле
void byteFrequency(FILE *fin)
{
	for (int i = 0; i < 256; freqBytes[i++] = 0);
	unsigned char byte = 0;

	totalBytes = 1;

	fread(&byte, sizeof(char), 1, fin);
	while (!feof(fin))
	{	
		++totalBytes;
		++freqBytes[byte]; 
		fread(&byte, sizeof(char), 1, fin);
	}
}

void treeTemplate()
{
	santinel.next = &santinel;
	santinel.before = &santinel;
	santinel.data = NULL;

	notZero = 0;
	for (int i = 0; i < 256; ++i)
	{
		if (freqBytes[i] == 0)
			continue;
		++notZero;

		Node *node;
		node = (Node*)malloc(sizeof(Node));
		initNode(node, freqBytes[i], i);
		List *list;
		list = (List*)malloc(sizeof(List));
		initList(list, node);
		addList(list, &santinel);
	}
}

void buildTree()
{
	while (santinel.next != santinel.before)
	{
		List *first = santinel.next;
		List *second = santinel.next->next;

		Node *newNode;
		newNode = (Node*)malloc(sizeof(Node));
		initNode(newNode, 0, 0);
		addNode(newNode, first->data, second->data);

		removeList(first);
		removeList(second);

		List *newList;
		newList = (List*)malloc(sizeof(List));
		initList(newList, newNode);
		addList(newList, &santinel);
	}
}

//присваивание каждому существующему байту кодировки
void setCode()
{
	dataValues = (HC*)malloc(sizeof(HC) * notZero);
	numByte = 0;

	HC patt;
	patt.countBits = 0;
	for (int i = 0; i < 4; patt.bitString[i++] = 0);
	setBitValue(santinel.next->data->leftChild, patt);
	setBitValue(santinel.next->data->rightChild, patt);
}

int isTooSmallSize()
{
	int sizeOfFile = notZero * 3 + 4;
	for (int i = 0; i < notZero; ++i)
	{
		sizeOfFile += freqBytes[dataValues[i].byte] * dataValues[i].countBits;
	}
	if (sizeOfFile < totalBytes)
	{
		return 404;
	}
	return 0;
}

//информация для декодирования
void writeDecInfo(FILE *fout, char *format)
{
	fprintf(fout, "%s", format);
	fprintf(fout, "%c", notZero);
	for (int i = 0; i < notZero; ++i)
	{
		fprintf(fout, "%c%c", dataValues[i].byte, dataValues[i].countBits);

		int num = (dataValues[i].countBits - 1) / 8;
		for (int j = 0; j <= num; ++j)
			fprintf(fout, "%c", dataValues[i].bitString[j]);
	}
}

//сжатые данные из файла
void writeCompressData(FILE *fin, FILE *fout)
{
	HC ptrArr[256];
	for (int i = 0; i < notZero; ++i)
	{
		int idx = dataValues[i].byte;

		ptrArr[idx].countBits = dataValues[i].countBits;
		ptrArr[idx].byte = dataValues[i].byte;
		strncpy(ptrArr[idx].bitString, dataValues[i].bitString, 32);
	}

	rewind(fin);

	unsigned char patByte = 0;
	unsigned char curBit = 1, checkBit = 1;
	unsigned char byte;

	while (!feof(fin)) //8
	{	
		fread(&byte, sizeof(char), 1, fin);
		if (feof(fin))
			break;

		char isBit; 
		for (int i = 0; i < ptrArr[byte].countBits; ++i)
		{
			isBit = ptrArr[byte].bitString[i / 8] & (checkBit << (i % 8));
			if (isBit)
				patByte |= curBit;

			if (curBit == 128)
			{
				fprintf(fout, "%c", patByte);
				patByte = 0;
				curBit = 1;
			}
			else
				curBit <<= 1;
		}
	}
	fprintf(fout, "%c%c", patByte, curBit / 2);
}

void fcompress(FILE *fin, char *inFile)
{
	byteFrequency(fin);

	treeTemplate();

	buildTree();

	setCode();

	if (isTooSmallSize())
	{
		printf("The file is too small to compress (%d bytes)\n", totalBytes);
		exit(0);
	}

	char *format;
	format = (char*)calloc(sizeof(char), 4);
	strcpy(format, &inFile[strlen(inFile) - 3]);

	FILE *fout; //6
	char *outFile;
	outFile = (char*)calloc(sizeof(char), strlen(inFile) + 5);
	strcpy(outFile, inFile);
	strcat(outFile, ".bin");
	fout = fopen(outFile, "wb");

	writeDecInfo(fout, format);
	
	writeCompressData(fin, fout);
}

/*
Структура сжатого файла:

информация для декодировки:
	- 3 байта: формат сжатого файла
	- 4 байта: количество байт в файле (n)
	- n 3-х байтных структур, где 1 байт: закодированный байт, 2 байт: количество бит в кодировке, 3 байт: сама кодировка
сжатые данные:
	- коды символов (k - 2, где k количество записанных байт)
	- k - 1 байт остаточный
	- k байт - количество бит, которые остались в остаточном байте
*/