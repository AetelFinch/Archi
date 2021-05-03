#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compressor.h"
#include "decompressor.h"

int isCompress(char *file)
{
	int len = strlen(file);
	if (strcmp(&file[len-4], ".bin"))
		return 1;
	return 0;
}

int main(int argc, char **argv)
{
	FILE *fin;
	fin = fopen(argv[1], "rb");
	if (fin == NULL)
	{
		printf("File opening error. Please repeat opening\n");
		return 0;
	}

	if (isCompress(argv[1]))
		fcompress(fin, argv[1]);
	else
		fdecompress(fin, argv[1]);

	return 0;
}