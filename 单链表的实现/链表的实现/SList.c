#include"SList.h"

// ��̬����һ���ڵ�
SListNode* BuySListNode(SLTDateType x)
{
	SListNode *Node = (SListNode*)malloc(sizeof(SListNode));
	if (Node == NULL)
	{
		perror("BuySListNode:malloc");
		exit(-1);
	}
	Node->data = x;
	Node->next = NULL;
	return Node;
}
// �������ӡ
void SListPrint(SListNode* plist)
{
	while (plist)
	{
		printf("%d ", plist->data);
		plist = plist->next;
	}
	printf("\n");
}
// ������β��
void SListPushBack(SListNode** pplist, SLTDateType x)
{
	assert(pplist);
	SListNode*cur = *pplist;
	SListNode*prev = cur;
	while (cur)
	{
		prev = cur;
		cur = cur->next;
	}
	if (prev == NULL)
		*pplist = BuySListNode(x);
	else
		prev->next = BuySListNode(x);
}
// �������ͷ��
void SListPushFront(SListNode** pplist, SLTDateType x)
{
	assert(pplist);
	if (*pplist==NULL)
		*pplist = BuySListNode(x);
	else
	{
		SListNode*tmp = (*pplist);
		*pplist = BuySListNode(x);
		(*pplist)->next = tmp;
	}
}
// �������βɾ
void SListPopBack(SListNode** pplist)
{
	assert(pplist&&*pplist);
	SListNode*cur = *pplist;
	SListNode*prev = cur;
	while (cur->next)
	{
		prev = cur;
		cur = cur->next;
	}
	free(cur);
	prev->next = NULL;
}
// ������ͷɾ
void SListPopFront(SListNode** pplist)
{
	assert(pplist&&*pplist);
	SListNode*next = (*pplist)->next;
	free(*pplist);
	*pplist = next;
}
// ���������
SListNode* SListFind(SListNode* plist, SLTDateType x)
{
	while (plist)
	{
		if (plist->data == x)
			return plist;
		plist = plist->next;
	}
}
// ��������posλ��֮�����x
// ����˼��Ϊʲô����posλ��֮ǰ���룿
//��ΪҪ��posλ��֮ǰ������Ҫ��pos֮ǰ�Ľڵ㣬Ч�ʾͱ����
void SListInsertAfter(SListNode* pos, SLTDateType x)
{
	assert(pos);
	SListNode *pnext = pos->next;
	pos->next = BuySListNode(x);
	pos->next->next = pnext;
}
// ������ɾ��posλ��֮���ֵ
// ����˼��Ϊʲô��ɾ��posλ�ã�
//ͬ��Ҫɾ��posλ�õ�ֵ��Ҫ��pos֮ǰ�Ľڵ㣬����Ч�ʾ͵���
void SListEraseAfter(SListNode* pos)
{
	assert(pos);
	SListNode*cur = pos->next;
	SListNode*pnext = NULL;
	if (cur)
		pnext = cur->next;
	free(cur);
	pos->next = pnext;
}
//�����������
void SListDestory(SListNode**pplist)
{
	assert(pplist);
	SListNode*cur = *pplist;
	SListNode*pnext = NULL;
	while (cur)
	{
		pnext = cur->next;
		free(cur);
		cur = pnext;
	}
	*pplist = NULL;
}