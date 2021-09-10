#include"SeqList.h"

// ������ɾ��Ľӿ�
// ˳����ʼ��
void SeqListInit(SeqList* psl)
{
	assert(psl);
	psl->_PList = (SLDataType*)malloc(sizeof(SLDataType)*N);
	psl->_size = 0;
	psl->_capacity = N;
}
// ˳�������
void SeqListDestory(SeqList* psl)
{
	assert(psl);
	free(psl->_PList);
	psl->_capacity = 0;
	psl->_size = 0;
	psl->_PList = NULL;
}
// ˳����ӡ
void SeqListPrint(const SeqList* psl)
{
	assert(psl);
	for (int i = 0; i < psl->_size; i++)
	{
		printf("%d ", psl->_PList[i]);
	}
	printf("\n");
}
// ���ռ䣬������ˣ���������
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
// ˳���β��
void SeqListPushBack(SeqList* psl, SLDataType x)
{
	CheckCapacity(psl);
	psl->_PList[psl->_size] = x;
	psl->_size++;
}
// ˳���βɾ
void SeqListPopBack(SeqList* psl)
{
	assert(psl);
	if (psl->_size != 0)
	{
		psl->_size--;
	}
}
// ˳���ͷ��
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
// ˳���ͷɾ
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
// ˳������
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
// ˳�����posλ�ò���x
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
// ˳���ɾ��posλ�õ�ֵ
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