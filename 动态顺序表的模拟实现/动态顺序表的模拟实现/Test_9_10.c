#include"SeqList.h"

int main()
{
	SeqList sl;
	SeqListInit(&sl);
	for (int i = 0; i < 100; i++)
	{
		SeqListPushBack(&sl, i*(i + 2));
	}
	SeqListPrint(&sl);
	SeqListDestory(&sl);

	return 0;
}