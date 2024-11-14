#include <stdio.h>
#include <stdlib.h>
#define MAXVEX  50
#define MAXSIZE 50
#define NumVertices  50
#define INFINITY   65535
typedef  struct
{
    char verlist[NumVertices];
    int edge[NumVertices][NumVertices];
    int n,e;
} MTGraph;//�ڽӾ���
void CreatMGraph(MTGraph *G)//�����ڽӾ���
{
    int i,j,k,w;
    /*printf("please input vertex.\n");
    i=0;
    while(1)
    {
        if(getchar()!='\n'&&i<G->n)
        {
            G->verlist[i]=getchar();
             i++;
        }
        if(i==G->n)
            break;
    }*/
    for(i=0; i<G->n; i++)
    {
        for(j=0; j<G->n; j++)
        {
            G->edge[i][j]=INFINITY;
        }
    }
    for(k=0; k<G->e; k++)
    {
        printf("please input Vertices of edge and its weight.\n");
        scanf("%d,%d,%d",&i,&j,&w);
        G->edge[i][j]=w;
        G->edge[j][i]=w;
    }
}
void DisplayMGraph(MTGraph *G)
{
    int i,j;
    for(i=0; i<G->n; i++)
    {
        printf("����%d ",i);
        for(j=0; j<G->n; j++)
        {
            printf(" %d",G->edge[i][j]);
        }
        printf("\n");
    }
}
typedef struct node//�߱�ڵ�
{
    int adjvex;
    int cost;
    struct node*next;
} EdgeNode;
typedef struct//�����ڵ�
{
    char vertex;
    EdgeNode*firstedge;
} VertexNode;
typedef struct
{
    VertexNode vexlist[NumVertices];
    int n,e;
} AdjGraph;
void CreatAGraph(AdjGraph *G)//�����ڽӱ�
{
    int i;
    int tail,head,weight;
    /*printf("please input vertex.\n");
    i=0;
    while(1)
    {
        if(getchar()!='\n'&&i<G->n)
        {
            G->vexlist[i].vertex=getchar();
            G->vexlist[i].firstedge=NULL;
             i++;
        }
        if(i==G->n)
            break;
    }*/
    for(i=0; i<G->e; i++)
    {
        EdgeNode *p=NULL;
        printf("please input Vertices of edge and its weight.\n");
        scanf("%d,%d,%d",&tail,&head,&weight);
        p=(EdgeNode *)malloc(sizeof(EdgeNode));
        p->adjvex=head;
        p->cost = weight;
        p->next=G->vexlist[tail].firstedge;
        G->vexlist[tail].firstedge=p;
        p=(EdgeNode *)malloc(sizeof(EdgeNode));
        p->adjvex=tail;
        p->cost = weight;
        p->next=G->vexlist[head].firstedge;
        G->vexlist[head].firstedge=p;
    }
}
void DisplayAGraph(AdjGraph *G)
{
    int k;
    for(int k=0; k<G->n; k++)
    {
        printf("�ڵ�%d ",k);
        EdgeNode *p=G->vexlist[k].firstedge;
        while(p!=NULL)
        {
            printf("%d,%d  ",p->adjvex,p->cost);
            p=p->next;
        }
        printf("\n");
    }
}
void matrixtochart(MTGraph G1,AdjGraph *G2)
{
    int i,j;
    G2->n=G1.n;
    G2->e=G1.e;
    for( i=0; i<G2->n; i++)
    {
        G2->vexlist[i].vertex=G1.verlist[i];
        G2->vexlist[i].firstedge=NULL;
    }
    for(i=0; i<G2->n; i++)
    {
        for(j=0; j<G2->n; j++)
        {
            if(G1.edge[i][j]<INFINITY)
            {
                EdgeNode *p=NULL;
                p=(EdgeNode *)malloc(sizeof(EdgeNode));
                p->adjvex=i;
                p->cost = G1.edge[i][j];
                p->next=G2->vexlist[j].firstedge;
                G2->vexlist[j].firstedge=p;
            }
        }
    }
}
void charttomatrix(AdjGraph G1,MTGraph *G2)
{
    int i,j;
    G2->n=G1.n;
    G2->e=G1.e;
    for( i=0; i<G2->n; i++)
    {
        G2->verlist[i]=G1.vexlist[i].vertex;
    }
    for(i=0; i<G2->n; i++)
    {
        for( j=0; j<G2->n; j++)
        {
            G2->edge[i][j]=INFINITY;
        }
    }
    for(int k=0; k<G2->n; k++)
    {
        EdgeNode *p=G1.vexlist[k].firstedge;
        i=k;
        while(p!=NULL)
        {
            j=p->adjvex;
            G2->edge[i][j]=p->cost;
            p=p->next;
        }
    }
}
#define TRUE 1
#define FALSE 0
typedef int Boolean;
Boolean visited[MAXVEX];
typedef struct
{
    int data[MAXSIZE];
    int front;
    int rear;
} Queue;
void MakeNull(Queue*Q)
{
    Q->front=0;
    Q->rear=0;
}
int Empty(Queue*Q)
{
    if(Q->front=Q->rear)
        return 1;
    else
        return 0;
}
int EnQueue(int k,Queue*Q)
{
    if ((Q->rear+1)%MAXSIZE == Q->front)	/* ���������ж� */
        return 0;
    Q->data[Q->rear]=k;			/* ��Ԫ��e��ֵ����β */
    Q->rear=(Q->rear+1)%MAXSIZE;/* rearָ�������һλ�ã� */								/* ���������ת������ͷ�� */
    return  1;
}
int DeQueue(Queue *Q)
{
    int e;
    if (Q->front == Q->rear)			/* ���пյ��ж� */
        return 0;
    e=Q->data[Q->front];				/* ����ͷԪ�ظ�ֵ��e */
    Q->front=(Q->front+1)%MAXSIZE;	/* frontָ�������һλ�ã� */
    return  e;
}

#define    MAXSIZE 50
void DFS_1(MTGraph *G)//�ǵݹ�����ڽӾ���
{
    int stacks[MAXSIZE];
    int i;
    for(i = 0; i < G->n; i++)
        visited[i] = FALSE;
    int top = -1;
    i=0;
    visited[i] = TRUE;
    printf("%d ",i);
    stacks[++top] =i;
    while(top !=-1)
    {
        i = stacks[top];
        int tmp = 0;
        for(int j = 0 ; j <G->n; j++)
        {
            if( G->edge[i][j] <INFINITY && !visited[j])
            {
                visited[j] = TRUE;
                printf("%d ",j);
                stacks[++top] =j;
                break;
            }
            tmp = j;
        }
        if( tmp == G->n-1)
            top--;
    }
}
void DFS1(MTGraph *G, int i)//�ݹ�����ڽӾ���
{
    int j;
    visited[i] = TRUE;
    printf("%d ",i);
    for(j=0; j<G->n; j++)
    {
        if((G->edge[i][j]<INFINITY)&&(!visited[j]))
            DFS1(G, j);
    }
}
void DFSTraverse1(MTGraph *G)//�ݹ�����ڽӾ���
{
    int i;
    for(i = 0; i < G->n; i++)
        visited[i] = FALSE;
    for(i = 0; i < G->n; i++)
        if(!visited[i])
            DFS1(G, i);
}
void BFS1(MTGraph *G,int k)//�ǵݹ����ڽӾ���
{
    int i;
    Queue Q;
    MakeNull(&Q);
    visited[k]=TRUE;
    printf("%d ",k);
    EnQueue(k,&Q);
    while(!Empty(&Q))
    {
        i=DeQueue(&Q);
        for(int j=0; j<G->n; j++)
        {
            if((G->edge[i][j]<INFINITY)&&(!visited[j]))
            {
                visited[j]=TRUE;
                printf("%d ",j);
                EnQueue(j,&Q);
            }
        }
    }
}
void BFSTraverse1(MTGraph *G)//�ǵݹ����ڽӾ���
{
    int i;
    for(i = 0; i < G->n; i++)
        visited[i] = FALSE;
    for(i = 0; i < G->n; i++)
        if(!visited[i])
            BFS1(G, i);
}
int count =0;
void MBFS1(MTGraph *G,int k)//�ǵݹ����ڽӾ������
{
    int i;

    Queue Q;
    MakeNull(&Q);
    visited[k]=TRUE;
    printf("����%d����Ϊ%d ",k,count++);
    EnQueue(k,&Q);
    while(!Empty(&Q))
    {
        i=DeQueue(&Q);
        for(int j=0; j<G->n; j++)
        {
            if((G->edge[i][j]==1)&&(!visited[j]))
            {
                visited[j]=TRUE;
                printf("����%d����Ϊ%d ",j,count++);
                EnQueue(j,&Q);
            }
        }
    }
}
void MBFSTraverse1(MTGraph *G)//�ǵݹ����ڽӾ������
{
    int i;
    for(i = 0; i < G->n; i++)
        visited[i] = FALSE;
    for(i = 0; i < G->n; i++)
        if(!visited[i])
            MBFS1(G, i);
}

void DFS2(AdjGraph *G, int i)//�ݹ�����ڽӱ�
{
    EdgeNode *p;
    visited[i] = TRUE;
    printf("%d ",i);
    p = G->vexlist[i].firstedge;
    while(p)
    {
        if(!visited[p->adjvex])
            DFS2(G, p->adjvex);
        p = p->next;
    }
}
void DFSTraverse2(AdjGraph *G)//�ݹ�����ڽӱ�
{
    int i;
    for(i = 0; i < G->n; i++)
        visited[i] = FALSE;
    for(i = 0; i < G->n; i++)
        if(!visited[i])
            DFS2(G, i);
}
void DFS_2(AdjGraph *G)//�ǵݹ�����ڽӱ�
{
    int stacks[MAXSIZE];
    int i;
    for(i = 0; i < G->n; i++)
        visited[i] = FALSE;
    int top = -1;
    i=0;
    visited[i] = TRUE;
    printf("%d ",i);
    stacks[++top] =i;
    while(top !=-1)
    {
        i = stacks[top];
        int tmp = 0;
        EdgeNode *p;
        p = G->vexlist[i].firstedge;
        while(p)
        {
            if(!visited[p->adjvex])
            {
                visited[p->adjvex]=TRUE;
                printf("%d ",p->adjvex);
                stacks[++top] =p->adjvex;
                break;
            }
            p = p->next;
        }
        if( p==NULL)
            top--;
    }
}

void BFS2(AdjGraph *G,int k)//�ǵݹ����ڽӱ�
{
    int i;
    EdgeNode *p;
    Queue Q;
    MakeNull(&Q);
    printf("%d ",k);
    visited[k]=TRUE;
    EnQueue(k,&Q);
    while(!Empty(&Q))
    {
        i=DeQueue(&Q);
        p=G->vexlist[i].firstedge;
        while(p)
        {
            if(!visited[p->adjvex])
            {
                printf("%d ",p->adjvex);
                visited[p->adjvex]=TRUE;
                EnQueue(p->adjvex,&Q);
            }
            p=p->next;
        }
    }
}
void BFSTraverse2(AdjGraph *G)//�ǵݹ����ڽӱ�
{
    int i;
    for(i = 0; i < G->n; i++)
        visited[i] = FALSE;
    for(i = 0; i < G->n; i++)
        if(!visited[i])
            BFS2(G, i);
}
int count2 =0;
void ABFS2(AdjGraph *G,int k)//�ǵݹ����ڽӱ����
{
    int i;

    EdgeNode *p;
    Queue Q;
    MakeNull(&Q);
    printf("����%d����Ϊ%d ",k,count2++);
    visited[k]=TRUE;
    EnQueue(k,&Q);
    while(!Empty(&Q))
    {
        i=DeQueue(&Q);
        p=G->vexlist[i].firstedge;
        while(p)
        {
            if(!visited[p->adjvex])
            {
                printf("����%d����Ϊ%d ",p->adjvex,count2++);
                visited[p->adjvex]=TRUE;
                EnQueue(p->adjvex,&Q);
            }
            p=p->next;
        }
    }
}
void ABFSTraverse2(AdjGraph *G)//�ǵݹ����ڽӱ����
{
    int i;
    for(i = 0; i < G->n; i++)
        visited[i] = FALSE;
    for(i = 0; i < G->n; i++)
        if(!visited[i])
            ABFS2(G, i);
}
typedef struct Tnode
{
    int data;
    struct Tnode *firstchild;
    struct Tnode *nextsibling;
} Node;
void DFSTree(MTGraph *G, int i, Node* T)
{
    visited[i] = TRUE; // ���һ�¸õ� �Ѿ�������
    int first = 1;// ��ǵ�һ������
    Node *q;
    for(int j= 0; j < G->n; j++)
    {
        if((G->edge[i][j]==1)&&(!visited[j]))
        {
            Node *p = (Node *)malloc(sizeof(Node));//����ռ� �� �����ڵ�
            p->data = j;
            p->firstchild =NULL;
            p->nextsibling = NULL;
            if(first)// ����ǵ�һ��
            {
                T = p;
                first = 0;
            }
            else
            {
                q->nextsibling = p;
            }
            q  = p;
            DFSTree(G, j, q->firstchild);//���� ������
        }
    }
}
Node* DFSForest(MTGraph *G, Node *T)
{
    T = NULL;// ��ʼ�� ��ɭ��Ϊ��
    int i;
    for( i = 0; i <G->n; i++) // ��ʼ���������
        visited[i] = FALSE;

    Node *q = NULL;// ָ�� ��һ���ֵ�
    for( i = 0; i < G->n; i++)
    {
        if(!visited[i])
        {
            Node *p = (Node *)malloc(sizeof(Node)); // ���� �ռ�
            //��ֵ ��ʼ���ڵ�
            p->data = i;
            p->firstchild = NULL;
            p->nextsibling = NULL;

            if(!T) // �����ɭ��Ϊ�գ� ���õ�һ�����ĸ��� ��Ϊɭ�ֵĸ�
            {
                T = p;
            }
            else // ���������һ���ֵ� ��������
            {
                q->nextsibling = p;
            }
            q = p; // ����һ��q

            DFSTree(G, i, p->firstchild); //  �� ������ɵ�ɭ��  ������ ������
        }
    }
    return T;
}
void DFS_Tree(AdjGraph *G, int i, Node* T)
{
    visited[i] = TRUE; // ���һ�¸õ� �Ѿ�������
    int first = 1;// ��ǵ�һ������
    Node *q;
    EdgeNode *pe;
    pe = G->vexlist[i].firstedge;
    while(pe)
    {
        if(!visited[pe->adjvex])
        {
            Node *p = (Node *)malloc(sizeof(Node));//����ռ� �� �����ڵ�
            p->data =pe->adjvex;
            p->firstchild =NULL;
            p->nextsibling = NULL;
            if(first)// ����ǵ�һ��
            {
                T->firstchild = p;
                first = 0;
            }
            else
            {
                q->nextsibling = p;
            }
            q  = p;
            DFS_Tree(G, pe->adjvex, q);//���� ������
        }
        pe = pe->next;
    }
}
Node* DFS_Forest(AdjGraph *G, Node *T)
{
    T = NULL;// ��ʼ�� ��ɭ��Ϊ��
    int i;
    for( i = 0; i <G->n; i++) // ��ʼ���������
        visited[i] = FALSE;
    Node *q = NULL;// ָ�� ��һ���ֵ�
    for( i = 0; i < G->n; i++)
    {
        if(!visited[i])
        {
            Node *p = (Node *)malloc(sizeof(Node)); // ���� �ռ�
            //��ֵ ��ʼ���ڵ�
            p->data = i;
            p->firstchild = NULL;
            p->nextsibling = NULL;
            if(!T) // �����ɭ��Ϊ�գ� ���õ�һ�����ĸ��� ��Ϊɭ�ֵĸ�
            {
                T = p;
            }
            else // ���������һ���ֵ� ��������
            {
                q->nextsibling = p;
            }
            q = p; // ����һ��q
            DFS_Tree(G, i, p); //  �� ������ɵ�ɭ��  ������ ������
        }
    }
    return T;
}
void PreOrder(Node *T)
{
    if(T==NULL)
        return;
    printf("%d  ",T->data);/* ��ʾ������ݣ����Ը���Ϊ�����Խ����� */
    PreOrder(T->firstchild); /* ��������������� */
    PreOrder(T->nextsibling); /* ���������������� */
}
void degree(MTGraph *G,int degrees[])
{
    int i,j;
    for(i=0; i<G->n; i++)
    {
        degrees[i]=0;
        for(j=0; j<G->n; j++)
        {
            if(G->edge[i][j]<INFINITY)
                degrees[i]++;
        }
        printf("����%d�Ķ���Ϊ%d\n",i,degrees[i]);
    }
}
void degree2(AdjGraph *G,int degrees[])
{
    int i,j;
    for(i=0; i<G->n; i++)
    {
        degrees[i]=0;
        EdgeNode *p;
        p= G->vexlist[i].firstedge;
        while(p)
        {
            degrees[i]++;
            p=p->next;
        }
        printf("����%d�Ķ���Ϊ%d\n",i,degrees[i]);
    }
}
int main()
{
   MTGraph G1;
    printf("please input the number of vertex and edge.\n");
    scanf("%d,%d",&G1.n,&G1.e);
    CreatMGraph(&G1);
    printf("�ڽӾ���\n");
    DisplayMGraph(&G1);

    AdjGraph G2;
    printf("please input the number of vertex and edge.\n");
    scanf("%d,%d",&G2.n,&G2.e);
    CreatAGraph(&G2);
    printf("�ڽӱ�\n");
    DisplayAGraph(&G2);

    matrixtochart( G1,&G2);
    printf("\n�ڽӾ���תΪ�ڽӱ�\n");
    DisplayAGraph(&G2);

    charttomatrix( G2,&G1);
    printf("\n�ڽӱ�תΪ�ڽӾ���\n");
    DisplayMGraph(&G1);

    printf("\n�ǵݹ�������������ڽӾ���\n");
    DFS_1(&G1);
    printf("\n�ݹ�������������ڽӾ���\n");
    DFSTraverse1(&G1);
    printf("\n������������ڽӾ���\n");
    BFSTraverse1(&G1);
    printf("\n������������ڽӾ������\n");
    MBFSTraverse1(&G1);

    printf("\n�ǵݹ�������������ڽӱ�\n");
   DFS_2(&G2);
    printf("\n�ݹ�������������ڽӱ�\n");
    DFSTraverse2(&G2);
    printf("\n������������ڽӱ�\n");
    BFSTraverse2(&G2);
    printf("\n������������ڽӱ����\n");
    ABFSTraverse2(&G2);

    Node *T;
    T = DFSForest(&G1, T);
    printf("\n��������ڽӾ���������\n");
    PreOrder(T);
    Node *T1;
    T1 = DFS_Forest(&G2, T1);
    printf("\n��������ڽӱ�������\n");
    PreOrder(T1);
   int degrees[MAXVEX];
    printf("\n�ڽӾ�����������\n");
    degree(&G1,degrees);
    printf("\n�ڽӱ���������\n");
    degree2(&G2,degrees);
    return 0;
}
