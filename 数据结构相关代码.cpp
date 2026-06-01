//在第i个位置前插入元素e
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define OK 1
#define OVERFLOW -2
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status InputList_Sq(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
void InsertList_Sq(SqList *L,int i,int e){//在第i个位置前插入元素e 
	int j;
	ElemType *newbase;
	if(L->length>=L->listsize){
		newbase=(ElemType*)realloc(L->elem,(L->listsize+LISTINCREMENT)*sizeof(ElemType));
		if(!newbase) exit(OVERFLOW);
		L->elem=newbase;
		L->listsize+=LISTINCREMENT;
	}
	for(j=L->length-1;j>=i-1;j--){
		L->elem[j+1]=L->elem[j];
	}
	L->elem[i-1]=e;
	L->length++;
}
void PrintList_Sq(SqList L){
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList L;
	int n,i,e;
	InitList_Sq(&L);
	scanf("%d",&n);
	InputList_Sq(&L,n);
	scanf("%d%d",&i,&e);
	InsertList_Sq(&L,i,e);
	PrintList_Sq(L);
	return 0;
}
//删除位于x1，x2之间的所有数
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define OK 1
#define OVERFLOW -2
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status InputList_Sq(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
void DeleteList_Sq(SqList *L,int x1,int x2){//删除X1与X2之间的所有元素（不包括） 
	int i=0,k=0;
	for(i=0;i<L->length;i++){
		if(L->elem[i]>x1&&L->elem[i]<x2){
			k++;
		}
		else
		L->elem[i-k]=L->elem[i];	
	}
	L->length-=k;
}
void PrintList_Sq(SqList L){
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList L;
	int n,x1,x2;
	InitList_Sq(&L);
	scanf("%d",&n);
	InputList_Sq(&L,n);
	scanf("%d%d",&x1,&x2);
	DeleteList_Sq(&L,x1,x2);
	PrintList_Sq(L);
	return 0;
}
//将所有元素逆置 
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define OK 1
#define OVERFLOW -2
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status InputList_Sq(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
void Reverse(SqList *L){//将所有元素逆置 
	int i=0,j=L->length-1,temp;
	while(i<j){
		temp=L->elem[i];
		L->elem[i]=L->elem[j];
		L->elem[j]=temp;
		i++;
		j--;
	}
}
void PrintList_Sq(SqList L){
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList L;
	int n;
	InitList_Sq(&L);
	scanf("%d",&n);
	InputList_Sq(&L,n);
	Reverse(&L); 
	PrintList_Sq(L);
	return 0;
}
//将比数k小的放左边，大的放右边 
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define OK 1
#define OVERFLOW -2
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status InputList_Sq(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
void fun(SqList *L,int k){
	int i=0,j=L->length-1,temp;
	while(i<j){
		while(i<j&&L->elem[i]<k) i++;
		while(i<j&&L->elem[j]>k) j--;
		if(i<j){
		temp=L->elem[i];
		L->elem[i]=L->elem[j];
		L->elem[j]=temp;	
		}
	}
}
void PrintList_Sq(SqList L){
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList L;
	int n,k;
	InitList_Sq(&L);
	scanf("%d",&n);
	InputList_Sq(&L,n);
	scanf("%d",&k);
	fun(&L,k); 
	PrintList_Sq(L);
	return 0;
}
//求最大值并删除 
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define OK 1
#define OVERFLOW -2
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status InputList_Sq(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
int findMaxList(SqList *L){
	int i,max,k=0;
	max=L->elem[0];
	for(i=0;i<L->length;i++){
		if(L->elem[i]>max){
		max=L->elem[i];	
		}
	}
	for(i=0;i<L->length;i++){
		if(L->elem[i]==max)
		k++;
		else
		L->elem[i-k]=L->elem[i];
	}
	L->length-=k;
	return max;
}
void PrintList_Sq(SqList L){
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList L;
	int n,max;
	InitList_Sq(&L);
	scanf("%d",&n);
	InputList_Sq(&L,n);
    max=findMaxList(&L);
    printf("%d\n",max);
	PrintList_Sq(L);
	return 0;
}
//合并两个有序顺序表La和Lb
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define OK 1
#define OVERFLOW -2
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status InputList_Sq(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
void MergeList(SqList *La,SqList *Lb,SqList *Lc){ 
	int i=0,j=0,k=0,x;
	while(i<La->length&&j<Lb->length){
		if(La->elem[i]<=Lb->elem[j]){
		Lc->elem[k++]=La->elem[i];
		i++;	
		}
		else{
			Lc->elem[k++]=Lb->elem[j];
			j++;
		}	
	}
	if(i==La->length){
		for(x=j;x<Lb->length;x++){
			Lc->elem[k++]=Lb->elem[x];
		}	
	}
	else{
		for(x=i;x<La->length;x++){
			Lc->elem[k++]=La->elem[x];
		}
	}
	Lc->length=k;
}
void PrintList_Sq(SqList L){
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList La,Lb,Lc;
	int m,n;
	InitList_Sq(&La);
	InitList_Sq(&Lb);
	InitList_Sq(&Lc);
	scanf("%d%d",&m,&n);
	InputList_Sq(&La,m);
	InputList_Sq(&Lb,n);
	MergeList(&La,&Lb,&Lc);
	PrintList_Sq(Lc);
	return 0;
}
//A并B 
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define OK 1
#define OVERFLOW -2
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status InputList_Sq(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
int  LocateList(SqList *L,int e){
	int i;
	for(i=0;i<L->length;i++){
		if(L->elem[i]==e){
			break;
		}
	}
	if(i==L->length)
	return 0;
	else
	return 1;
}
void fun(SqList *La,SqList *Lb){
	int j,i=0;
	for(j=0;j<Lb->length;j++){
		if(!LocateList(La,Lb->elem[j])){
			La->elem[La->length+i]=Lb->elem[j];
			i++;
		}
	}
	La->length+=i;
}
void PrintList_Sq(SqList L){
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList La,Lb;
	int m,n;
	InitList_Sq(&La);
	InitList_Sq(&Lb);
	scanf("%d%d",&m,&n);
	InputList_Sq(&La,m);
	InputList_Sq(&Lb,n);
	fun(&La,&Lb);
	PrintList_Sq(La);
	return 0;
}
//A交B
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define OK 1
#define OVERFLOW -2
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int length;
	int listsize;
}SqList;
Status InitList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(!L->elem) exit(OVERFLOW);
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status InputList_Sq(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
int  LocateList(SqList *L,int e){
	int i;
	for(i=0;i<L->length;i++){
		if(L->elem[i]==e){
			break;
		}
	}
	if(i==L->length)
	return 0;
	else
	return 1;
}
void fun(SqList *La,SqList *Lb){
	int j,k=0;
	for(j=0;j<Lb->length;j++){
		if(!LocateList(La,Lb->elem[j])){
			k++;
		}
		else{
			Lb->elem[j-k]=Lb->elem[j];
		}
	}
	Lb->length-=k;
}
void PrintList_Sq(SqList L){
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList La,Lb;
	int m,n;
	InitList_Sq(&La);
	InitList_Sq(&Lb);
	scanf("%d%d",&m,&n);
	InputList_Sq(&La,m);
	InputList_Sq(&Lb,n);
	fun(&La,&Lb);
	PrintList_Sq(Lb);
	return 0;
}
//(A-B)∪(B-A)
#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 20
#define LISTINCREMENT 20
#define OK 1;
typedef int ElemType;
typedef int Status;
typedef struct{
	ElemType *elem;
	int listsize;
	int length;
}SqList;
Status initList_Sq(SqList *L){
	L->elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	L->length=0;
	L->listsize=LIST_INIT_SIZE;
	return OK;
}
Status inputList(SqList *L,int n){
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&L->elem[i]);
		L->length++;
	}
	return OK;
}
int LocateList(SqList *L,int e){
	int i=0;
	for(i=0;i<L->length;i++){
		if(L->elem[i]==e){
			break;
		}
	}
	if(i==L->length)
	return 0;
	else
	return 1;
}
void fun(SqList *La,SqList *Lb,SqList *Lc){
	int i,j,k=0;
	for(i=0;i<La->length;i++){
		if(!LocateList(Lb,La->elem[i])){
			Lc->elem[k++]=La->elem[i];
		}
	}
	for(j=0;j<Lb->length;j++){//B-A
		if(!LocateList(La,Lb->elem[j])){
			Lc->elem[k++]=Lb->elem[j];
		}
	}
	Lc->length=k;
}
void PrintList(SqList L){
	if(L.length==0){
		printf("结果为空集！"); 
	}
	int i=0;
	for(i=0;i<L.length;i++){
		printf("%d ",L.elem[i]);
	}
}
int main(){
	SqList La,Lb,Lc;
	int m,n;
	initList_Sq(&La);
	initList_Sq(&Lb);
	initList_Sq(&Lc);
	scanf("%d%d",&m,&n);
	inputList(&La,m);
	inputList(&Lb,n);
	fun(&La,&Lb,&Lc);
	PrintList(Lc);
	return 0;
}
//链表
//用头插法和尾插法创建单链表 
#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode,*linkList;
void CreateList_H(linkList L,int n){//尾插法 
	linkList r,s;
	r=L;
	int i;
	int a[100];
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		r->next=s;
		r=s;
	}
	r->next=NULL;
}
void CreateList_H(linkList L,int n){//头插法 
	linkList r,s;
	int i;
	int a[100];
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		s->next=L->next;
		L->next=s;
	}
}
void PrintList(linkList L){
	linkList p;
	p=L->next;
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
}
int main(){
	linkList L=(linkList)malloc(sizeof(LNode));
	L->next=NULL;
	int n;
	scanf("%d",&n);
	CreateList_H(L,n);
	PrintList(L);
	return 0;
}
//删除链表最小值结点 
#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode,*linkList;
void CreateList_H(linkList L,int n){//尾插法 
	linkList r,s;
	r=L;
	int i;
	int a[100];
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		r->next=s;
		r=s;
	}
	r->next=NULL;
}
void DeleteList(linkList L){
	linkList min,minpre,p,pre;
	pre=L;
	p=pre->next;
	minpre=pre;
	min=minpre->next;
	while(p!=NULL){
		if(p->data<min->data){
			min=p;
			minpre=pre;
		}
		pre=p;
		p=p->next;
	}
	minpre->next=min->next;
	free(min); 
}
void PrintList(linkList L){
	linkList p;
	p=L->next;
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
}
int main(){
	linkList L=(linkList)malloc(sizeof(LNode));
	L->next=NULL;
	int n;
	scanf("%d",&n);
	CreateList_H(L,n);
	DeleteList(L);
	PrintList(L);
	return 0;
}
///在有序链表中插入一个元素 
#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode,*linkList;
void CreateList_H(linkList L,int n){//尾插法 
	linkList r,s;
	r=L;
	int i;
	int a[100];
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		r->next=s;
		r=s;
	}
	r->next=NULL;
}
void InsertList(linkList L,int e){
	linkList p,pre,s;
	pre=L;
	p=pre->next;
	while(p!=NULL){
		if(p->data>e){
			s=(linkList)malloc(sizeof(LNode));
			s->data=e;
			s->next=p;
			pre->next=s;
			return;//结束函数 
		}
		pre=p;
		p=p->next;
	}
	//处理末尾插入元素 
    s = (linkList)malloc(sizeof(LNode));
    s->data = e;
    s->next = NULL;
    pre->next = s;
}
void PrintList(linkList L){
	linkList p;
	p=L->next;
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
}
int main(){
	linkList L=(linkList)malloc(sizeof(LNode));
	L->next=NULL;
	int n,e;
	scanf("%d",&n);
	CreateList_H(L,n);
	scanf("%d",&e);
	InsertList(L,e);
	PrintList(L);
	return 0;
}
//删除链表中所有值为偶数的结点; 
#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode,*linkList;
void CreateList_H(linkList L,int n){//尾插法 
	linkList r,s;
	r=L;
	int i;
	int a[100];
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		r->next=s;
		r=s;
	}
	r->next=NULL;
}
void DeleteList(linkList L){
	linkList p,pre;
	pre=L;
	p=pre->next;
	while(p!=NULL){
		if(p->data%2==0){
			pre->next=p->next;
			free(p);
			p=pre->next;
		}
		else{
			pre=p;
			p=p->next;
		}
	}
}
void PrintList(linkList L){
	linkList p;
	p=L->next;
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
}
int main(){
	linkList L=(linkList)malloc(sizeof(LNode));
	L->next=NULL;
	int n,e;
	scanf("%d",&n);
	CreateList_H(L,n);
	DeleteList(L);
	PrintList(L);
	return 0;
}
//A=A并B，释放LB空间 
#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode,*linkList;
void CreateList_H(linkList L,int a[],int n){//尾插法 
	linkList r,s;
	r=L;
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		r->next=s;
		r=s;
	}
	r->next=NULL;
}
int isinList(linkList L,int e){
	linkList p;
	p=L->next;
	while(p!=NULL){
		if(p->data==e)
		return 1;
		p=p->next;
	}
	return 0;
}
void MergeList(linkList La,linkList Lb)
{
	linkList pb,pa,p,pre;
	pa=La->next;
	pre=Lb;
	pb=pre->next;
	while(pb!=NULL){
		if(!isinList(La,pb->data)){//头插法插入 
		p=pb;
		pre->next=pb->next;
		pb->next=pa;
		La->next=pb;
		pa=La->next;
		pb=pre->next;	
		}
		else{
		pre=pb;
		pb=pb->next;	
		}	
	}
}
void FreeList(linkList L){
	linkList p,q;
	p=L->next;
	while(p!=NULL){
		q=p;
		p=p->next;
		free(q);
	}
	free(L);
}
void PrintList(linkList L){
	linkList p;
	p=L->next;
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
}
int main(){
	int i,j;
	int a[100],b[100];
	linkList La=(linkList)malloc(sizeof(LNode));
	La->next=NULL;
	linkList Lb=(linkList)malloc(sizeof(LNode));
	Lb->next=NULL;
	int m,n,e;
	scanf("%d%d",&m,&n);
	CreateList_H(La,a,m);
	CreateList_H(Lb,b,n);
	MergeList(La,Lb);
	FreeList(Lb);
	PrintList(La);
	return 0;
}
//A交B 
#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode,*linkList;
void CreateList_T(linkList L,int a[],int n){//尾插法 
	linkList r,s;
	r=L;
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		r->next=s;
		r=s;
	}
	r->next=NULL;
}
int isinList(linkList L,int e){
	linkList p;
	p=L->next;
	while(p!=NULL){
		if(p->data==e)
		return 1;
		p=p->next;
	}
	return 0;
}
void MergeList(linkList La,linkList Lb)
{
	linkList pb,pa,p,pre;
	pa=La->next;
	pre=Lb;
	pb=pre->next;
	while(pb!=NULL){
		if(!isinList(La,pb->data)){//头插法插入 
		p=pb;
		pre->next=pb->next;
		pb->next=pa;
		La->next=pb;
		pa=La->next;
		pb=pre->next;	
		}
		else{
		pre=pb;
		pb=pb->next;	
		}	
	}
	pre->next=NULL;
}
void FreeList(linkList L){
	linkList p,q;
	p=L->next;
	while(p!=NULL){
		q=p;
		p=p->next;
		free(q);
	}
	free(L);
}
void PrintList(linkList L){
	linkList p;
	p=L->next;
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
}
int main(){
	int i,j;
	int a[100],b[100];
	linkList La=(linkList)malloc(sizeof(LNode));
	La->next=NULL;
	linkList Lb=(linkList)malloc(sizeof(LNode));
	Lb->next=NULL;
	int m,n,e;
	scanf("%d%d",&m,&n);
	CreateList_T(La,a,m);
	CreateList_T(Lb,b,n);
	MergeList(La,Lb);
	//FreeList(Lb);
	PrintList(Lb);
	return 0;
}
//A-B
#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode,*linkList;
void CreateList_T(linkList L,int a[],int n){//尾插法 
	linkList r,s;
	r=L;
	int i;
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		r->next=s;
		r=s;
	}
	r->next=NULL;
}
int isinList(linkList L,int e){
	linkList p;
	p=L->next;
	while(p!=NULL){
		if(p->data==e)
		return 1;
		p=p->next;
	}
	return 0;
}
void MergeList(linkList La,linkList Lb)
{
	linkList pb,pa,p,pre;
	pb=Lb->next;
	pre=La;
	pa=pre->next;
	while(pa!=NULL){
		if(isinList(Lb,pa->data)){
		pre->next=pa->next;
		pa=pre->next;	
		}
		else{
		pre=pa;	
		pa=pa->next;
		}
	}
}
void FreeList(linkList L){
	linkList p,q;
	p=L->next;
	while(p!=NULL){
		q=p;
		p=p->next;
		free(q);
	}
	free(L);
}
void PrintList(linkList L){
	linkList p;
	p=L->next;
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
}
int main(){
	int i,j;
	int a[100],b[100];
	linkList La=(linkList)malloc(sizeof(LNode));
	La->next=NULL;
	linkList Lb=(linkList)malloc(sizeof(LNode));
	Lb->next=NULL;
	int m,n,e;
	scanf("%d%d",&m,&n);
	CreateList_T(La,a,m);
	CreateList_T(Lb,b,n);
	MergeList(La,Lb);
	//FreeList(Lb);
	PrintList(La);
	return 0;
}
//用双链表判断是否回文
//先序，中序，后序遍历输出二叉树 
#include<stdlib.h>
#include<stdio.h>
typedef char ElemType;
typedef struct node{
	ElemType data;//结点值域 
	struct node *lchild,*rchild;
}BTNode,*BiTree;
void CreateBiTree(BiTree *T){//创建二叉树 
	char ch;
	scanf("%c",&ch);
	if(ch=='#') *T=NULL;
	else{
		*T=(BiTree)malloc(sizeof(BTNode));
		(*T)->data=ch;
		CreateBiTree(&(*T)->lchild);
		CreateBiTree(&(*T)->rchild);
	}
}
void visit(BiTree T){
	printf("%c ",T->data);
}
void PreOrder(BiTree T){
	if(T!=NULL){
		visit(T);
		PreOrder(T->lchild);
		PreOrder(T->rchild);
	}
}
void InOrder(BiTree T){
	if(T!=NULL){
		InOrder(T->lchild);
		visit(T);
		InOrder(T->rchild);
	} 
}
void PostOrder(BiTree T){
	if(T!=NULL){
		PostOrder(T->lchild);
		PostOrder(T->rchild);
		visit(T);
	}
}
int main(){
	BiTree T;
	CreateBiTree(&T);
	PreOrder(T);
	InOrder(T);
	PostOrder(T);
	return 0;
}

//求二叉树中节点个数
#include<stdio.h>
#include<stdlib.h>
typedef char ElemType;
typedef struct node{
	ElemType data;
	struct node *lchild,*rchild;
}BNode,*BiTree;
int nodecount;
void CreateBiTree(BiTree *T){
	char ch;
	scanf("%c",&ch);
	if(ch=='#') *T=NULL;
	else{
		*T=(BiTree)malloc(sizeof(BNode));
		(*T)->data=ch;
		CreateBiTree(&((*T)->lchild));
		CreateBiTree(&((*T)->rchild));
	}
} 
void visit(BiTree T){
	nodecount++;
	printf("%c ",T->data);
}
void PreOrder(BiTree T){
	if(T!=NULL){
		visit(T);
		PreOrder(T->lchild);
		PreOrder(T->rchild);
	}
}
int main(){
	BiTree T;
	nodecount=0;
	CreateBiTree(&T);
	PreOrder(T);
	printf("\n");
	printf("%d",nodecount);
}
//计算度数，以及度为0,2的节点数 
#include<stdio.h>
#include<stdlib.h>
typedef char ElemType;
typedef struct node{
	ElemType data;
	struct node *lchild,*rchild;
}BNode,*BiTree;
void CreateBiTree(BiTree *T){
	char ch;
	scanf("%c",&ch);
	if(ch=='#') *T=NULL;
	else{
		*T=(BiTree)malloc(sizeof(BNode));
		(*T)->data=ch;
		CreateBiTree(&((*T)->lchild));
		CreateBiTree(&((*T)->rchild));
	}
} 
int Nodecount(BiTree T){
	int L=0,R=0;
	if(T==NULL){
		return 0;
	}
	else{
		L=Nodecount(T->lchild);
		R=Nodecount(T->rchild);
		return 1+L+R;
	}
}
int Leafcount(BiTree T){
	int L=0,R=0;
	if(T==NULL){
		return 0;
	}
	else{
		if(T->lchild==NULL&&T->rchild==NULL)
		return 1;
		else{
			L=Leafcount(T->lchild);
			R=Leafcount(T->rchild);
			return L+R;
		}
	}
}
int Node2count(BiTree T){
	int L=0,R=0;
	if(T==NULL||(T->lchild==NULL&&T->rchild==NULL))
	return 0;
	else{	
		L=Node2count(T->lchild);
		R=Node2count(T->rchild);
		if(T->lchild==NULL||T->rchild==NULL)
			return L+R;
		else
			return 1+L+R;
	}
}
int main(){
	BiTree T;
	CreateBiTree(&T);
	int nodecount=0,leafcount=0,node2count=0;
	nodecount=Nodecount(T);
	leafcount=Leafcount(T);
	node2count=Node2count(T);
	printf("%d %d %d",nodecount,leafcount,node2count);
}
//计算二叉树的高度 
#include<stdio.h>
#include<stdlib.h>
typedef char ElemType;
typedef struct node{
	ElemType data;
	struct node *lchild,*rchild;
}BNode,*BiTree;
void CreateBiTree(BiTree *T){
	char ch;
	scanf("%c",&ch);
	if(ch=='#') *T=NULL;
	else{
		*T=(BiTree)malloc(sizeof(BNode));
		(*T)->data=ch;
		CreateBiTree(&((*T)->lchild));
		CreateBiTree(&((*T)->rchild));
	}
}
int Depth(BiTree T){
	int L=0,R=0,MAX=0;
	if(T==NULL)
	return 0;
	else{
		L=Depth(T->lchild);
		R=Depth(T->rchild);
		MAX=L>R?L:R;
		return 1+MAX;
	}
}
int main(){
	BiTree T;
	CreateBiTree(&T);
	int depth;
	depth=Depth(T);
	printf("%d",depth);
	return 0;
}
//输出所有叶子结点的值 
#include<stdlib.h>
#include<stdio.h>
typedef char ElemType;
typedef struct node{
	ElemType data;//结点值域 
	struct node *lchild,*rchild;
}BTNode,*BiTree;
void CreateBiTree(BiTree *T){//创建二叉树 
	char ch;
	scanf("%c",&ch);
	if(ch=='#') *T=NULL;
	else{
		*T=(BiTree)malloc(sizeof(BTNode));
		(*T)->data=ch;
		CreateBiTree(&(*T)->lchild);
		CreateBiTree(&(*T)->rchild);
	}
}
void visit(BiTree T){
	if(T->lchild==NULL&&T->rchild==NULL) 
	printf("%c ",T->data);
}
void PreOrder(BiTree T){
	if(T!=NULL){
		visit(T);
		PreOrder(T->lchild);
		PreOrder(T->rchild);
	}
}
int main(){
	BiTree T;
	CreateBiTree(&T);
	PreOrder(T);
	return 0;
}
//输出二叉树的最长路径长度以及最长路径上的点 
#include<stdio.h>
#include<stdlib.h>
typedef char ElemType;
typedef struct node{
	ElemType data;
	struct node *lchild,*rchild;
}BTNode,*BiTree;
typedef struct{
	char road[100];
	int length=0;
}Road;
Road sumroad[100];
int k=0;
void CreateBiTree(BiTree *T){
	char ch; 
	scanf("%c",&ch);
	if(ch=='#') *T=NULL;
	else{
		*T=(BiTree)malloc(sizeof(BTNode));
		(*T)->data=ch;
		CreateBiTree(&((*T)->lchild));
		CreateBiTree(&((*T)->rchild));
	}
}
int LeafCount(BiTree b){
	int L,R;
	if(b==NULL) return 0;
	if(b->lchild==NULL&&b->rchild==NULL) return 1;
	L=LeafCount(b->lchild);
	R=LeafCount(b->rchild);
	return L+R;
}

void visit(char current[],int currentlength){
	int i=0;
	for(i=currentlength-1;i>=0;i--){
	printf("%c",current[i]);
	sumroad[k].road[sumroad[k].length++]=current[i];
	}
	k++; 
	printf("\n");
}
void Path(BiTree T,char current[],int currentlength){
	if(T==NULL){
	return;	
	}
	current[currentlength++]=T->data;
	if(T->lchild==NULL&&T->rchild==NULL)
	visit(current,currentlength);
	Path(T->lchild,current,currentlength);
	Path(T->rchild,current,currentlength);
}
int max(Road r[]){
	int i=0;
	int maxlength=r[0].length,maxi=0;
	for(i=0;i<k;i++){
		if(r[i].length>maxlength)
		{
			maxlength=r[i].length;
			maxi=i;
		}
	}
	return maxi;
}
void PrintfMAX(Road r[]){
	int maxi,i;
	maxi=max(r);
	printf("最长路径的长度为：%d\n",r[maxi].length);
	for(i=r[maxi].length-1;i>=0;i--){
		printf("%c ",r[maxi].road[i]);
	}
}
int main(){
	BiTree T;
	int i;
	CreateBiTree(&T);
	int roadcount=LeafCount(T);
	printf("%d\n",roadcount);
	int currentlength=0;
	char current[100];
	Path(T,current,currentlength);
	PrintfMAX(sumroad);
	return 0;
} 
//输出二叉树所有路径
#include<stdio.h>
#include<stdlib.h>
typedef char ElemType;
typedef struct node{
	ElemType data;
	struct node *lchild,*rchild;
}BTNode,*BiTree;
void CreateBiTree(BiTree *T){
	char ch; 
	scanf("%c",&ch);
	if(ch=='#') *T=NULL;
	else{
		*T=(BiTree)malloc(sizeof(BTNode));
		(*T)->data=ch;
		CreateBiTree(&((*T)->lchild));
		CreateBiTree(&((*T)->rchild));
	}
}
int LeafCount(BiTree b){
	int L,R;
	if(b==NULL) return 0;
	if(b->lchild==NULL&&b->rchild==NULL) return 1;
	L=LeafCount(b->lchild);
	R=LeafCount(b->rchild);
	return L+R;
}
void visit(char *current,int currentlength){
	for(int i=currentlength-1;i>=0;i--){
		printf("%c",current[i]);
	}
	printf("\n");
}
void Path(BiTree b,char *current,int currentlength){
	if (b == NULL) {
        return;
    }
    current[currentlength++] = b->data;
    if (b->lchild == NULL && b->rchild == NULL) {
        visit(current, currentlength);
    }
    Path(b->lchild,current,currentlength);
    Path(b->rchild,current,currentlength);
}
int main(){
	BiTree b;
	CreateBiTree(&b);
	int currentlength=0;
	int leafcount=LeafCount(b);
	printf("%d\n",leafcount); 
	char current[100];
	Path(b,current,currentlength);
	return 0;
}
//单链表排序
#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode,*linkList;
void CreateList_R(linkList L,ElemType a[],int n){
	int i=0;
	linkList r,s;
	r=L;
	for(i=0;i<n;i++){
		s=(linkList)malloc(sizeof(LNode));
		s->data=a[i];
		r->next=s;
		r=s;
	}
	r->next=NULL;
}
void Sort(linkList L){
	linkList p,q,r;
	p=L->next;
	if(p!=NULL){
		r=p->next;
		p->next=NULL;
		p=r;
		while(p!=NULL){
			q=L;
			r=p->next;
			while(q->next!=NULL&&q->next->data<p->data){
				q=q->next;
			}
			p->next=q->next;
			q->next=p;
			p=r;
		}
	}
}
void PrintList(linkList L){
	linkList p=L->next;
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
}
int main(){
	linkList L;
	L=(linkList)malloc(sizeof(LNode));
	L->next=NULL;
	int i,n;
	int a[100];
	scanf("%d",&n);
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
	}
	CreateList_R(L,a,n);
	Sort(L);
	PrintList(L);
}
