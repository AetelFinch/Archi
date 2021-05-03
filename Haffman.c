#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coder.h"
#include "decoder.h"

// на вход подается файл, который нужно декодировать или закодировать
int main(int argc, char **argv) //1
{
	FILE *f;
	f = fopen("project description.txt", "rb");
	if (f == NULL)
	{
		printf("File opening error. Please repeat opening\n");
		return 0;
	}

	/////////////////////////////////////////

	int freqBytes[256];
	for (int i = 0; i < 256; freqBytes[i++] = 0); //обнуление массива частот байтов
	unsigned char byte = 0;

	int totalBytes = 1;

	fread(&byte, sizeof(char), 1, f);
	while (!feof(f))
	{	
		++totalBytes;
		++freqBytes[byte]; 
		fread(&byte, sizeof(char), 1, f);
	}

	//////////////////////////////////////
	
	List santinel; //2
	santinel.next = &santinel;
	santinel.before = &santinel;
	santinel.data = NULL;

	int notZero = 0;
	for (int i = 0; i < 256; ++i) //создание списка узлов дерева
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

	////////////////////////////////////////

	while (santinel.next != santinel.before) //3 Построение дерева
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
	/////////////////////////////////////

	dataValues = (HC*)malloc(sizeof(HC) * notZero);
	numByte = 0;

	HC patt;
	patt.countBits = 0;
	for (int i = 0; i < 4; patt.bitString[i++] = 0);
	setBitValue(santinel.next->data->leftChild, patt); //4
	setBitValue(santinel.next->data->rightChild, patt);

	////////////////////////////////////

	int sizeOfFile = notZero * 3 + 4; //5 количество байт, необходимых для кодирования
	for (int i = 0; i < notZero; ++i)
	{
		sizeOfFile += freqBytes[dataValues[i].byte] * dataValues[i].countBits;
	}
	if (sizeOfFile < totalBytes)
	{
		printf("The file is too small to compress (%d bytes)\n", totalBytes);
		return 0;
	}
	/////////////////////////////////

	FILE *fout; //6
	fout = fopen("output.bin", "wb");

	fprintf(fout, "%c", notZero); //запись информации для декодирования 
	for (int i = 0; i < notZero; ++i)
	{
		fprintf(fout, "%c%c", dataValues[i].byte, dataValues[i].countBits); //7

		int num = (dataValues[i].countBits - 1) / 8;
		for (int j = 0; j <= num; ++j)
			fprintf(fout, "%c", dataValues[i].bitString[j]);
	}

	////////////////////////////////запись сжатых данных в файл

	HC ptrArr[256];
	for (int i = 0; i < notZero; ++i)
	{
		int idx = dataValues[i].byte;

		ptrArr[idx].countBits = dataValues[i].countBits;
		ptrArr[idx].byte = dataValues[i].byte;
		strncpy(ptrArr[idx].bitString, dataValues[i].bitString, 32);
	}

	rewind(f);

	unsigned char patByte = 0;
	unsigned char curBit = 1, checkBit = 1;

	while (!feof(f)) //8
	{	
		fread(&byte, sizeof(char), 1, f);
		if (feof(f))
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

	return 0;
}



/*
Улучшения:

- при кодировании записывать изначальный формат файла, чтобы при декодировании
  архиватор автоматически менял фаормат выходного файла

- 
*/