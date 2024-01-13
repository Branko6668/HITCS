#include <stdio.h>
#include <string.h>
#define MAXSIZE 20 /* �洢�ռ��ʼ������ */

typedef struct linklist
{
    int data;
    struct linklist *next;
} LinkList;

/* ��ʼ��������ʽ���Ա�L�Ѵ��ڡ��������������L������Ԫ�ظ��� */

/* ��ʼ��������ʽ���Ա�L�Ѵ��ڣ�1��i��ListLength(L) */
/* �����������e����L�е�i������Ԫ�ص�ֵ */
int GetElem(LinkList *L,int i,int *e)
{
    int j;
    LinkList *p;		/* ����һ���p */
    p = L;		/* ��pָ������L�ĵ�һ����� */
    j = 1;		/*  jΪ������ */
    while (p && j<i)  /* p��Ϊ�ջ��߼�����j��û�е���iʱ��ѭ������ */
    {
        p = p->next;  /* ��pָ����һ����� */
        ++j;
    }
    if ( !p || j>i )
        return 0;  /*  ��i��Ԫ�ز����� */
    *e = p->data;   /*  ȡ��i��Ԫ�ص����� */
    return 1;
}
/* ��ʼ��������ʽ���Ա�L�Ѵ��� */
/* �������������L�е�1����e�����ϵ������Ԫ�ص�λ�� */
/* ������������Ԫ�ز����ڣ��򷵻�ֵΪ0 */
int LocateElem(LinkList *L,int e)
{
    int i=0;
    LinkList *p=L->next;
    while(p)
    {
        i++;
        if(p->data==e) /* �ҵ�����������Ԫ�� */
            return i;
        p=p->next;
    }

    return 0;
}
/* ��ʼ��������ʽ���Ա�L�Ѵ���,1��i��ListLength(L)�� */
/* �����������L�е�i��λ��֮ǰ�����µ�����Ԫ��e��L�ĳ��ȼ�1 */
void ListInsert(LinkList *L,int e)
{
    LinkList *p,*s;
    p = L;

    s = (LinkList*)malloc(sizeof(LinkList));  /*  �����½��(C���Ա�׼����) */
    s->data = e;
    s->next = p->next;      /* ��p�ĺ�̽�㸳ֵ��s�ĺ��  */
    p->next = s;          /* ��s��ֵ��p�ĺ�� */
}

/* ��ʼ��������ʽ���Ա�L�Ѵ��ڣ�1��i��ListLength(L) */
/* ���������ɾ��L�ĵ�i������Ԫ�أ�����e������ֵ��L�ĳ��ȼ�1 */
void ListDelete(LinkList *L,int e)//ɾ��Ԫ��e
{
    LinkList *p,*q;
    p = L;

    while(p->data==e)
    {
        L=p->next;
        free(p);
        p=L;
    }
    q=p->next;
    while (q->next!=NULL )	/* ����Ѱ�ҵ�i��Ԫ�� */
    {
         if(q->data==e)
        {
            p->next = q->next;
            free(q);
            q=p->next;
        }
        else
        {
           p = p->next;
           q=p->next;
        }
    }
    if((q->next==NULL)&&(q->data==e))
    {
        p->next=NULL;
    }
    else
    {
        q->next=NULL;
    }
}
//ð���������Ҫ˼���������ԱȽϣ�ÿ�Ƚ�һ�λ��һ��������С�����������
void Sort(LinkList *L){
	LinkList *cur,*tail;
	cur=L;
	tail=NULL;
	if(cur==NULL||cur->next==NULL){
		return;
	}
	while(cur!=tail){
		while(cur->next!=tail){
			if(cur->data>cur->next->data){
				int temp=cur->data;
				cur->data=cur->next->data;
				cur->next->data=temp;

			}
			cur=cur->next;
		}
		tail=cur;
		cur=L;
	}

}
void DeleteList(LinkList *L)//�����򣬺�ǰһ��Ԫ�رȽϣ���ͬ����һ������ͬ��ֱ��ɾ��
{

    Sort(L);
    LinkList *p,*q;
    p = L->next;
    while(p->next!=NULL)
    {
        if(p->data==p->next->data)
        {
            p -> next = p->next->next;

        }
        else
        {
            p = p->next;
        }
    }
}
LinkList *reverse( LinkList *head )
{
    LinkList *L=(LinkList*)malloc(sizeof(LinkList)),*p,*q;//L�����µ�ͷ�ڵ�
    L->next=NULL;
    p=head;//��������p���൱���м����
    while(p)
    {
        q=(LinkList*)malloc(sizeof(LinkList));
        q->data=p->data;
        q->next=L->next;//ͷ�巨
        L->next=q;//ͷ�巨  q,�½ڵ�
        p=p->next;
    }
    return L->next;
}
LinkList *rotateRight(LinkList *head, int k)
{
    if (head == NULL || head->next == NULL || k == 0) return head;
    int len = 1;
    LinkList *tail = head;

    /* find the end of list */
    while (tail->next != NULL)
    {
        tail = tail->next;
        len++;
    }

    /* form a circle */
    tail->next = head;
    k = k % len;
    for (int i = 0; i < len - k; i++)
    {
        tail = tail->next;
    }
    head = tail->next;
    tail->next = NULL;
    return head;
}
void Union(LinkList *La, LinkList *Lb)//�ϲ��������ź�������Ա�
{
    LinkList *p=La;
    int  i=1;
    int e;                        /*������La��Lb��ͬ������Ԫ��e*/
    for (p=La; p!=NULL; p=p->next)
    {
        GetElem(Lb,i,&e);
        i++;             /*ȡLb�е�i������Ԫ�ظ���e*/
        if (!LocateElem(La, e))        /*La�в����ں�e��ͬ����Ԫ��*/
            ListInsert(La, e); /*����*/
    }
}
void Print(LinkList *L)
{
    LinkList *p=L;
    while(p!=NULL)
    {
        printf("%d   ",p->data);
        p=p->next;
    }
    printf("\n");
}


int main()
{
    LinkList *L=NULL;
    LinkList *p=NULL;
    p=(LinkList*) malloc(sizeof(LinkList));
    p -> next == NULL;
    L=p;
    int e;
    printf("input values of L\n");
    while (1)
    {
        scanf("%d",&e);
        p->data = e;
        if (getchar() == '\n') break;
        p->next = (LinkList*) malloc(sizeof(LinkList));
        p->next->next = NULL;
        p = p->next;
    }
    p=NULL;

    LinkList *Lb=NULL;
    LinkList *q=NULL;
    q=(LinkList*) malloc(sizeof(LinkList));
    q -> next == NULL;
    Lb=q;
    printf("input values of L\n");
    while (1)
    {
        scanf("%d",&e);
        q->data = e;
        if (getchar() == '\n') break;
        q->next = (LinkList*) malloc(sizeof(LinkList));
        q->next->next = NULL;
        q = q->next;
    }
    q=NULL;

    printf("which element do you want to delete in L\n");//ɾ���ض�Ԫ��
    scanf("%d", &e);
    ListDelete(L, e);
    Print(L);

    printf("Remove duplicate elements in L\n");//ɾ���ظ�Ԫ��
    Sort(L);
    DeleteList(L);
    Print(L);

    printf("The inversion of L\n");//����
    L=reverse( L);
    Print(L);

    printf("��ѭ����λ\n");
    int k;
    scanf("%d", &k);
    L=rotateRight(L, k);
    Print(L);

    printf("The union of L and Lb\n");//�ϲ�
    Sort(L);
    Sort(Lb);
    Union(L, Lb);
    Print(L);

    return 0;
}
