typedef struct HaffCode
{
	unsigned char bitString[32];
	unsigned char countBits;
	unsigned char byte;
} HC;

typedef struct Node
{
	struct Node *leftChild;
	struct Node *rightChild;
	struct Node *parent;
	int freq;
	unsigned char byte;
	char bit;
} Node;

HC *dataValues;
int numByte;

void initNode (Node *data, int freq, unsigned char byte)
{
	data->leftChild = NULL;
	data->rightChild = NULL;
	data->parent = NULL;
	data->freq = freq;
	data->byte = byte;
}

void addNode (Node *newNode, Node *left, Node *right)
{
	newNode->leftChild = left;
	newNode->rightChild = right;
	left->parent = newNode;
	right->parent = newNode;
	newNode->freq = left->freq + right->freq;
	left->bit = 0;
	right->bit = 1;
}

void bitsetSet (HC *arr, int newval) //проверить на корректность работы
{
	if (newval == 1)
	{
		int idx = arr->countBits;
	    int q = idx / 8; //индекс числа, где содержится нужный бит

	    unsigned long long z = (1 << (idx - 8 * q));

	    arr->bitString[q] = arr->bitString[q] | z;
	}		
    arr->countBits++;
}

void setBitValue(Node *node, HC pat)
{
	if (node == NULL)
		return;

	bitsetSet(&pat, node->bit);

	if (node->leftChild == NULL && node->rightChild == NULL)
	{
		for (int i = 0; i < 32; dataValues[numByte].bitString[i] = pat.bitString[i], i++);
		dataValues[numByte].countBits = pat.countBits;
		dataValues[numByte].byte = node->byte;
		++numByte;
	}
	setBitValue(node->leftChild, pat); //4
	setBitValue(node->rightChild, pat);
}

void printBitString (unsigned char b)
{
	for (int i = 0; i < 8; ++i)
	{
		printf("%c", (b & 128) ? '1' : '0');
		b <<= 1;
	}
	printf("\n");
}

void printb (HC *cell) //6
{
	printf("%x: ", cell->byte);
	int ost = cell->countBits;
	for (int j = 0; j < 2; ++j)
	{
		unsigned char b = cell->bitString[j];

		unsigned char bit = 1;
		int prBit;
		if (ost >= 8)
			prBit = 8;
		else
			prBit = ost;

		for (int i = 0; i < prBit; ++i)
		{
			printf("%c", (b & 0x1) ? '1' : '0');
			b >>= 1;
		}
		ost -= 8;
	}
	
	printf("\n");
}