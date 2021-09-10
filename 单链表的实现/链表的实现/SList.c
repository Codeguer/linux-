#include"SList.h"

// 动态申请一个节点
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
// 单链表打印
void SListPrint(SListNode* plist)
{
	while (plist)
	{
		printf("%d ", plist->data);
		plist = plist->next;
	}
	printf("\n");
}
// 单链表尾插
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
// 单链表的头插
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
// 单链表的尾删
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
// 单链表头删
void SListPopFront(SListNode** pplist)
{
	assert(pplist&&*pplist);
	SListNode*next = (*pplist)->next;
	free(*pplist);
	*pplist = next;
}
// 单链表查找
SListNode* SListFind(SListNode* plist, SLTDateType x)
{
	while (plist)
	{
		if (plist->data == x)
			return plist;
		plist = plist->next;
	}
}
// 单链表在pos位置之后插入x
// 分析思考为什么不在pos位置之前插入？
//因为要在pos位置之前插入需要找pos之前的节点，效率就变低了
void SListInsertAfter(SListNode* pos, SLTDateType x)
{
	assert(pos);
	SListNode *pnext = pos->next;
	pos->next = BuySListNode(x);
	pos->next->next = pnext;
}
// 单链表删除pos位置之后的值
// 分析思考为什么不删除pos位置？
//同理，要删除pos位置的值需要找pos之前的节点，这样效率就低了
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
//单链表的销毁
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