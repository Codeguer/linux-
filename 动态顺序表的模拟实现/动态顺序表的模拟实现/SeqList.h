#pragma once
#include<stdio.h>
#include<assert.h>
#include<stdbool.h>
#define _CRT_SECURE_NO_WARNINGS 1
#define N 10  //��ʼ���ռ�Ĵ�С

typedef int SLDataType;
//˳���ṹ
typedef struct SeqList
{
	SLDataType *_PList;//ָ��̬���ٵĿռ�
	size_t _size;//�����Ч���ݵĸ���
	size_t _capacity;//��¼��̬���ٵĿռ��С
}SeqList;

// ������ɾ��Ľӿ�
// ˳����ʼ��
void SeqListInit(SeqList* psl);
// ˳�������
void SeqListDestory(SeqList* psl);
// ˳����ӡ
void SeqListPrint(const SeqList* psl);
// ���ռ䣬������ˣ���������
void CheckCapacity(SeqList* psl);
// ˳���β��
void SeqListPushBack(SeqList* psl, SLDataType x);
// ˳���βɾ
void SeqListPopBack(SeqList* psl);
// ˳���ͷ��
void SeqListPushFront(SeqList* psl, SLDataType x);
// ˳���ͷɾ
void SeqListPopFront(SeqList* psl);
// ˳������
int SeqListFind(SeqList* psl, SLDataType x);
// ˳�����posλ�ò���x
void SeqListInsert(SeqList* psl, size_t pos, SLDataType x);
// ˳���ɾ��posλ�õ�ֵ
void SeqListErase(SeqList* psl, size_t pos);
