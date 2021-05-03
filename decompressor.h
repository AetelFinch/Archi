HC *getDecInfo(FILE *fin, unsigned char *bytes, HC *decInfo, char *format)
{
	fread(format, sizeof(char), 3, fin);
	fread(bytes, sizeof(char), 1, fin);

	decInfo = (HC*)malloc(sizeof(HC) * (*bytes));

	for (int i = 0; i < *bytes; ++i)
	{
		fread(&decInfo[i].byte, sizeof(char), 1, fin);
		fread(&decInfo[i].countBits, sizeof(char), 1, fin);

		for (int j = 0; j < (decInfo[i].countBits - 1) / 8 + 1; ++j)
			fread(&decInfo[i].bitString[j], sizeof(char), 1, fin);
	}
	return decInfo;
}



void fdecompress(FILE *fin, char *inFile)
{
	char *format;
	format = (char*)calloc(sizeof(char), 3);
	unsigned char bytes = 0;
	HC *decInfo = NULL;

	decInfo = getDecInfo(fin, &bytes, decInfo, format);

	for (int i = 0; i < bytes; ++i)
	{
		printb(&decInfo[i]);
	}
}

/*

Structure for decompressing

n = max(countBits)

1 bits: 
2 bits: [00]

...

n bits: [10..0][11..0]

логика работы алгоритма:

считывает байт 

for i = 1 to n
		смотрит, есть ли первые i бит в труктуре

		если есть, то печатает байт и продолжает работу с оставшимися битами



*/