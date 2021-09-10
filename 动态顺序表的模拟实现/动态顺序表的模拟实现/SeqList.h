#pragma once
#include<stdio.h>
#include<assert.h>
#include<stdbool.h>
#define _CRT_SECURE_NO_WARNINGS 1
#define N 10  //初始化空间的大小

typedef int SLDataType;
//顺序表结构
typedef struct SeqList
{
	SLDataType *_PList;//指向动态开辟的空间
	size_t _size;//存放有效数据的个数
	size_t _capacity;//记录动态开辟的空间大小
}SeqList;

// 基本增删查改接口
// 顺序表初始化
void SeqListInit(SeqList* psl);
// 顺序表销毁
void SeqListDestory(SeqList* psl);
// 顺序表打印
void SeqListPrint(const SeqList* psl);
// 检查空间，如果满了，进行增容
void CheckCapacity(SeqList* psl);
// 顺序表尾插
void SeqListPushBack(SeqList* psl, SLDataType x);
// 顺序表尾删
void SeqListPopBack(SeqList* psl);
// 顺序表头插
void SeqListPushFront(SeqList* psl, SLDataType x);
// 顺序表头删
void SeqListPopFront(SeqList* psl);
// 顺序表查找
int SeqListFind(SeqList* psl, SLDataType x);
// 顺序表在pos位置插入x
void SeqListInsert(SeqList* psl, size_t pos, SLDataType x);
// 顺序表删除pos位置的值
void SeqListErase(SeqList* psl, size_t pos);
