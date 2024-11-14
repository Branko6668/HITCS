#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXSIZE 100 /* �洢�ռ��ʼ������ */
#define MAX_LEN 50 /* ջ��ʼ������ */
//���ṹ
typedef struct BTNode  /* ���ṹ */
{
    char data;		/* ������� */
    struct BTNode *lchild,*rchild; /* ���Һ���ָ�� */
} BTREE;

int treeIndex=0;
char str[MAXSIZE];
/* ѭ�����е�˳��洢�ṹ */
typedef struct
{
    BTREE *data[MAXSIZE];
    int front;    	/* ͷָ�� */
    int rear;		/* βָ�룬�����в��գ�ָ�����βԪ�ص���һ��λ�� */
} SqQueue;
/*Ϊ���з���ռ�*/
SqQueue *Createqueue()
{
    SqQueue *p;
    p = (SqQueue *)malloc(sizeof(SqQueue));
    p->front = 0;
    p->rear = 0;
    return p;
}

//ջ�ṹ
typedef struct
{
    BTREE *data[MAX_LEN];
    int top;
} Stack;
/*Ϊջ����ռ�*/
Stack *Createstack()
{
    Stack *p;
    p = (Stack *)malloc(sizeof(Stack));
    p->top = -1;
    return p;
}

//ѹջ
int Push(Stack *p,BTREE *x)
{
    if (p->top == MAX_LEN - 1)
    {
        return -1;
    }
    p->top++;
    p->data[p->top] = x;
    return 0;
}

/*��ջ*/
BTREE *Pop(Stack *s)
{
    BTREE *p;
    if (s->top == -1)
    {
        printf("ջ�գ�\n");
        return NULL;
    }
    p = s->data[s->top];
    s->top--;
    return p;
}

/*ջ��*/
BTREE *Top(Stack *s)
{
    BTREE *p;
    if (s->top == -1)
    {
        printf("ջ�գ�\n");
        return NULL;
    }
    p = s->data[s->top];
    return p;
}

//�ж�ջ�Ƿ�Ϊ��
int Empty(Stack *L)
{
    return (L->top == -1);
}
//�ж�ջ�Ƿ�Ϊ��
void MakeNull(Stack *L)
{
    L->top = -1;
}

//����ն�����T
int InitBT(BTREE *T)
{
    T=NULL;
    return 1;
}

//��ǰ������������н���ֵ��һ���ַ���
//#��ʾ������������������ʾ������T��
void CreateBT(BTREE **T)
{
    char ch;
    scanf(" %c",&ch);
    if(ch=='#')
        *T = NULL;
    else
    {
        *T=(BTREE*)malloc(sizeof(BTREE));
        if(!*T)
            exit(OVERFLOW);
        (*T)->data=ch; /* ���ɸ���� */
        CreateBT(&((*T)->lchild)); /* ���������� */
        CreateBT(&((*T)->rchild)); /* ���������� */
    }
}

/* ��ʼ����: ������T���� */
/* �������: ��TΪ�ն�����,�򷵻�TRUE,����FALSE */
int BTEmpty(BTREE *T)
{
    if(T)
        return 0;
    else
        return 1;
}



/* ��ʼ����: ������T���ڡ��������: ����T�ĸ� */
char Root(BTREE *T)
{
    char Nil =' ';
    if(BTEmpty(T))
        return Nil;
    else
        return T->data;
}

/* ��ʼ����: ������T���ڣ�pָ��T��ĳ����� */
/* �������: ����p��ָ����ֵ */
char Value(BTREE *p)
{
    return p->data;
}

/* ��p��ָ��㸳ֵΪvalue */
void Assign(BTREE *p,char value)
{
    p->data=value;
}

/* ��ʼ����: ������T���� */
/* �������: ǰ��ݹ����T */
void PreOrder(BTREE *T)
{
    if(T==NULL)
        return;
    printf("%c",T->data);/* ��ʾ������ݣ����Ը���Ϊ�����Խ����� */
    PreOrder(T->lchild); /* ��������������� */
    PreOrder(T->rchild); /* ���������������� */
}
void Pre_Order(BTREE *T)
{
    Stack *S;
    S=Createstack(); //�ݹ鹤��ջ
    BTREE* p = T;
    while ( p !=NULL )
    {
        printf("%c",p->data);
        if ( p->rchild != NULL )
            Push (S, p->rchild );
        if ( p->lchild != NULL )
            p = p->lchild; //��������
        else
        {
            if(S->top!=-1)
            {
                p=Top(S);
                Pop(S);
            }
            else
            {
                p=NULL;
            }
        }
    }
}
void InOrder(BTREE *T)
{
    if(T==NULL)
        return;
    InOrder(T->lchild); /* ������������� */
    printf("%c",T->data);/* ��ʾ������ݣ����Ը���Ϊ�����Խ����� */
    InOrder(T->rchild); /* ���������������� */
}
void In_Order(BTREE *root)
{
    Stack *s;
    s=Createstack();
    int top= -1; //����˳��ջ�����ٶ����ᷢ������
    while (root!=NULL ||top!= -1)
    {
        while (root!= NULL)
        {
            s->data[++top]=root;
            root=root->lchild;
        }
        if (top!= -1)
        {
            root=s->data[top--];
            printf("%c",root->data);
            root=root->rchild;
        }
    }
}
/* ��ʼ����: ������T���� */
/* �������: ����ݹ����T */
void PostOrder(BTREE *T)
{
    if(T==NULL)
        return;
    PostOrder(T->lchild); /* �Ⱥ������������  */
    PostOrder(T->rchild); /* �ٺ������������  */
    printf("%c",T->data);/* ��ʾ������ݣ����Ը���Ϊ�����Խ����� */
}
void Post_Order (BTREE *t)
{
    BTREE *p, *pr;
    Stack *s;
    s=Createstack();
    p=t;
    while(p!=NULL||!Empty(s))
    {
        while (p!=NULL)
        {
            Push(s,p);
            pr=p->rchild;
            p=p->lchild;
            if(p==NULL)
                p=pr;
        }
        p=Pop(s);
        printf("%c",p->data);
        if(!Empty(s)&&Top(s)->lchild==p)
            p=Top(s)->rchild;
        else
            p=NULL;
    }
}

void LeverOrder (BTREE *root)
{
    BTREE *q;
    SqQueue *Q;
    Q->front=Q->rear=0; //����˳����У����ٶ����ᷢ������
    if (root==NULL)
        return;
    Q->data[++Q->rear]=root;
    while (Q->front!=Q->rear)
    {
        q=Q->data[++Q->front];
        printf("%c",q->data);
        if (q->lchild!=NULL)
            Q->data[++Q->rear]=q->lchild;
        if (q->rchild!=NULL)
            Q->data[++Q->rear]=q->rchild;
    }
}
/*�ж϶������Ƿ�Ϊ��ȫ�����������ò�������ķ�ʽ*/
int CompleteTree(BTREE *T)
{
    SqQueue *p;
    p = Createqueue(); //��������
    if(T==NULL)
        return 0;
    p->rear++; //�������ǿգ���ָ�����
    p->data[p->rear]=T;
    while(p->front !=p->rear)   //ѭ��ֱ�����в���
    {
        if(T->lchild!=NULL&&T->rchild!=NULL) //��������Һ��Ӷ���Ϊ�գ������ӣ����Һ��ӽ���
        {
            p->front++;
            T=p->data[p->front];
            if(T->lchild!=NULL)
            {
                p->rear++;
                p->data[p->rear]=T->lchild;
            }
            if(T->rchild!=NULL)
            {
                p->rear++;
                p->data[p->rear]=T->rchild;
            }
        }
        if(T->lchild==NULL&&T->rchild!=NULL) //���ֻ��������û��������������
            return 0;
        if((T->lchild!=NULL&&T->rchild==NULL)||(T->lchild==NULL&&T->rchild==NULL))
            //���Ӳ����Һ��ӿգ��������Ҷ���,��ý��֮������н��ض���Ҷ�ӽ��
        {
            p->front++;
            while(p->front !=p->rear)
            {
                T=p->data[p->front];
                if(T->lchild==NULL&&T->rchild==NULL)
                    p->front++;
                else
                    return 0;
            }
            return 1;
        }
    }
    return 1;
}
int BTwidth (BTREE *T)
{
    SqQueue *q;
    q = Createqueue(); //��������
    int count=0,max=0,right;
    q->front=q->rear=-1;
    if (T!=NULL)
        q->data[++q->rear]=T;
    max=1;
    right=q->rear;
    while(q->front!=q->rear)
    {
        T=q->data[++q->front];
        if(T->lchild!=NULL)
        {
            q->data[++q->rear]=T->lchild;
            count++;
        }
        if(T->rchild!=NULL)
        {
            q->data[++q->rear]=T->rchild;
            count++;
        }
        if(q->front==right)
        {
            if(max<count)

                max=count;
            count=0;
            right=q->rear;

        }
    }
    return max;
}
int main()
{
    int i=0;
    BTREE *T;
    char e;
    InitBT(T);
    printf("����ǰ׺���ʽ\n");
    CreateBT(&T);
    printf("\n�ݹ�ǰ�����������:");
    PreOrder(T);
    printf("\n�ǵݹ�ǰ�����������:");
    Pre_Order(T);
    printf("\n�ݹ��������������:");
    InOrder(T);
    printf("\n�ǵݹ��������������:");
    In_Order(T);
    printf("\n�ݹ�������������:");
    PostOrder(T);
    printf("\n�ǵݹ�������������:");
    Post_Order(T);
    int flag = CompleteTree(T);
    if(flag)
        printf("\n�ö���������ȫ��������\n");
    else
        printf("\n�ö�����������ȫ��������\n");
    int num;
    num =BTwidth (T);
    printf("���Ϊ%d",num);
    return 0;
}
