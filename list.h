typedef struct List
{
	struct List *next;
	struct List *before;
	struct Node *data;
} List;

void initList (List *list, Node *data)
{
	list->data = data;
	list->next = NULL;
	list->before = NULL;
}

void removeList (List *list)
{
	List *b = list->before;
	List *n = list->next;
	b->next = n;
	n->before = b;
	list->next = NULL;
	list->before = NULL;
}

void addList (List *list, List *head)
{
	List *ptr = head->next;

	for (; ptr->data != NULL; ptr = ptr->next)
	{
		if (list->data->freq < ptr->data->freq)
			break;
	}

	list->before = ptr->before; //before <-- list -- ptr
	list->next = ptr;			//before <-- list --> ptr
	ptr->before->next = list;   //before <--> list --> ptr
	ptr->before = list;			//before <--> list <--> ptr
}