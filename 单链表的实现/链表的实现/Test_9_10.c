#include"SList.h"

int main()
{
	SListNode* sl=NULL;
	for (int i = 0; i < 10; i++)
	{
		SListPushBack(&sl, i*(i + 1));
	}
	SListPrint(sl);
	SListPushFront(&sl,100);
	SListPrint(sl);
	SListPopBack(&sl);
	SListPrint(sl);
	SListPopFront(&sl);
	SListPrint(sl);
	SListDestory(&sl);
	return 0;
}