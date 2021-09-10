#include"SeqList.h"

// 基本增删查改接口
// 顺序表初始化
void SeqListInit(SeqList* psl)
{
	assert(psl);
	psl->_PList = (SLDataType*)malloc(sizeof(SLDataType)*N);
	psl->_size = 0;
	psl->_capacity = N;
}
// 顺序表销毁
void SeqListDestory(SeqList* psl)
{
	assert(psl);
	free(psl->_PList);
	psl->_capacity = 0;
	psl->_size = 0;
	psl->_PList = NULL;
}
// 顺序表打印
void SeqListPrint(const SeqList* psl)
{
	assert(psl);
	for (int i = 0; i < psl->_size; i++)
	{
		printf("%d ", psl->_PList[i]);
	}
	printf("\n");
}
// 检查空间，如果满了，进行增容
void CheckCapacity(SeqList* psl)
{
	assert(psl);
	if (psl->_capacity == psl->_size)
	{
		SLDataType *tmp = (SLDataType*)realloc(psl->_PList, sizeof(SLDataType)*psl->_capacity * 2);
		if (tmp == NULL)
		{
			perror("CheckCapacity:realloc");
			exit(-1);
		}
		psl->_PList = tmp;
		psl->_capacity *= 2;
	}
}
// 顺序表尾插
void SeqListPushBack(SeqList* psl, SLDataType x)
{
	CheckCapacity(psl);
	psl->_PList[psl->_size] = x;
	psl->_size++;
}
// 顺序表尾删
void SeqListPopBack(SeqList* psl)
{
	assert(psl);
	if (psl->_size != 0)
	{
		psl->_size--;
	}
}
// 顺序表头插
void SeqListPushFront(SeqList* psl, SLDataType x)
{
	assert(psl);
	CheckCapacity(psl);
	for (size_t i = psl->_size; i > 0; i--)
	{
		psl->_PList[i] = psl->_PList[i - 1];
	}
	psl->_PList[0] = x;
	psl->_size++;
}
// 顺序表头删
void SeqListPopFront(SeqList* psl)
{
	assert(psl);
	if (psl->_size)
	{
		for (size_t i = 0; i < psl->_size; i++)
		{
			psl->_PList[i] = psl->_PList[i + 1];
		}
		psl->_size--;
	}
}
// 顺序表查找
int SeqListFind(SeqList* psl, SLDataType x)
{
	assert(psl);
	for (size_t i = 0; i < psl->_size; i++)
	{
		if (psl->_PList[i] == x)
			return i;
	}
	return -1;
}
// 顺序表在pos位置插入x
void SeqListInsert(SeqList* psl, size_t pos, SLDataType x)
{
	CheckCapacity(psl);
	if (pos < psl->_size)
	{
		for (size_t i = psl->_size; i > pos; i++)
		{
			psl->_PList[i] = psl->_PList[i - 1];
		}
		psl->_PList[pos] = x;
		psl->_size++;
	}
}
// 顺序表删除pos位置的值
void SeqListErase(SeqList* psl, size_t pos)
{
	assert(psl);
	if (pos < psl->_size)
	{
		for (size_t i = pos; i < psl->_size-1; i++)
		{
			psl->_PList[i] = psl->_PList[i + 1];
		}
		psl->_size--;
	}
}