#include<stdio.h>
#include<stdlib.h>
#define MAXLEAF 50
#define MAXNODE 2*MAXLEAF-1
#define MAXWEIGHT 10000
#define MAXBIT 100
#include<windows.h>//���ͷ�ļ��������
typedef struct character
{
    char ch;
    int count ;
} CHARACTER;         //��¼ԭ�ı��ļ��г��ֵ��ַ�����Ӧ���ִ���������
typedef struct htnode
{
    int weight;
    int parent;
    int lchild;
    int rchild;
    char value;
    int i;
} HTNODE;            //���������Ľ�㶨��
typedef struct codenode
{
    int bit[MAXBIT];
    int start;
} CodeNode;            //��¼ÿ���ַ�����������
typedef struct heap
{
    HTNODE node[MAXNODE+1];
    int n;
} HEAP;                 //��С�ѵ����Ͷ���
void Statistic(CHARACTER chars[],int *n)    //ͳ��ԭ�ı��г��ֵ��ַ�����ʹ��Ƶ��
{
    FILE *f=NULL;
    if((f=fopen("original.txt","r"))==NULL)
    {
        printf("Fail to open original.txt!\n");
        exit(0);
    }
    char ch;
    int i=0,j=0,flag=0;
    fscanf(f,"%c",&ch);
    chars[i].ch=ch;
    chars[i].count=1;
    i++;
    while(ch!='#')  //���ļ������ı�����#��ǽ���
    {
        fscanf(f,"%c",&ch);
        flag=0;
        if(ch!='#')
        {
            for(j=0; j<i; j++)
            {
                if(ch==chars[j].ch)  //���������ַ��ѱ���¼���򽫳��ִ���+1
                {
                    chars[j].count++;
                    flag=1;
                    break;
                }
            }
            if(flag==0)
            {
                chars[i].ch=ch;
                chars[i].count=1;
                i++;
            }
        }
    }
    *n=i;
    fclose(f);
}
int  Displaych(CHARACTER chars[],int n)//չʾ�ַ�����Ƶ��
{
    int total=0;
    for(int i=0; i<n; i++)
    {
        total=total+chars[i].count;
    }
    for(int i=0; i<n; i++)
    {
        printf("�ַ�%c��Ƶ��Ϊ%f\n",chars[i].ch,chars[i].count*1.0/total);
    }
    return total;
}
void CreateHT(HTNODE huffnode[],int n,CHARACTER chars[])     //������������
{
    int i,j,w1,w2,x1,x2;
    for(i=0; i<2*n-1; i++)      //��ʼ��
    {
        huffnode[i].weight=chars[i].count;
        huffnode[i].parent=-1;
        huffnode[i].lchild=-1;
        huffnode[i].rchild=-1;
        huffnode[i].value=chars[i].ch;
    }
    for(i=0; i<n-1; i++)
    {
        w1=w2=MAXWEIGHT;
        x1=x2=0;
        for(j=0; j<n+i; j++)    //Ѱ��Ȩֵ��С���������
        {
            if(huffnode[j].parent==-1&&huffnode[j].weight<w1)
            {
                w2=w1;
                x2=x1;
                w1=huffnode[j].weight;
                x1=j;
            }
            else if(huffnode[j].parent==-1&&huffnode[j].weight<w2)
            {
                w2=huffnode[j].weight;
                x2=j;
            }
        }
        huffnode[x1].parent=n+i;
        huffnode[x2].parent=n+i;
        huffnode[n+i].lchild=x1;
        huffnode[n+i].rchild=x2;
        huffnode[n+i].weight=w1+w2;
    }
}
void Insert(HEAP *heap,HTNODE item)//��������Ԫ��
{
    int i;
    if(heap->n<MAXNODE-1)
    {
        i=heap->n+1;
        while((i!=1)&&(item.weight<heap->node[i/2].weight))
        {
            heap->node[i]=heap->node[i/2];
            i/=2;
        }
        heap->node[i]=item;
        heap->n++;
    }
}
HTNODE Delete(HEAP *heap)//ȡ�Ѷ�Ԫ�ز�ɾ��
{
    int parent=1,child=2;
    HTNODE item,temp;
    if(heap->n>0)
    {
        item=heap->node[1];
        temp=heap->node[heap->n--];
        while(child<=heap->n)
        {
            if(child<heap->n&&heap->node[child].weight>heap->node[child+1].weight)
                child++;
            if(temp.weight<=heap->node[child].weight)
                break;
            heap->node[parent]=heap->node[child];
            parent=child;
            child*=2;
        }
        heap->node[parent]=temp;
        return item;
    }
}
void CreateHT_heap(HTNODE huffnode[],CHARACTER chars[],int n,HEAP *heap)//����С���Ż��������������Ĺ���
{
    int i;
    HTNODE node1,node2;
    for(i=0; i<2*n-1; i++)
    {
        huffnode[i].weight=chars[i].count;
        huffnode[i].parent=-1;
        huffnode[i].lchild=-1;
        huffnode[i].rchild=-1;
        huffnode[i].value=chars[i].ch;
        huffnode[i].i=i;
        if(i<n)
            Insert(heap,huffnode[i]);
    }
    for(i=0; i<n-1; i++)
    {
        node1=Delete(heap);
        node2=Delete(heap);
        huffnode[node1.i].parent=n+i;
        huffnode[node2.i].parent=n+i;
        huffnode[n+i].lchild=node1.i;
        huffnode[n+i].rchild=node2.i;
        huffnode[n+i].weight=node1.weight+node2.weight;
        huffnode[n+i].i=n+i;
        Insert(heap,huffnode[n+i]);
    }
}
void Coding(HTNODE huffnode[],CodeNode huffcode[],int n)        //�����ַ��Ĺ����������
{
    int p,c,i;
    for(i=0; i<n; i++)
    {
        huffcode[i].start=MAXBIT-1;
        c=i,p=huffnode[i].parent;
        while(p!=-1)
        {
            if(c==huffnode[p].lchild)
                huffcode[i].bit[huffcode[i].start]=0;
            else
                huffcode[i].bit[huffcode[i].start]=1;
            c=p;
            p=huffnode[p].parent;
            huffcode[i].start--;
        }
        huffcode[i].start++;
    }
}
void Display(HTNODE huffnode[],CodeNode huffcode[],int n) //��ӡ�����������
{
    for(int i=0; i<n; i++)
    {
        printf("%c: ",huffnode[i].value);
        for(int j=huffcode[i].start; j<MAXBIT; j++)
        {
            printf("%d",huffcode[i].bit[j]);
        }
        printf("\n");
    }
}
void CodingFile(HTNODE huffnode[],CodeNode huffcode[],int n,int *chnum)     //��ԭ�ı����벢������д���ļ�
{
    FILE *fo=NULL;
    FILE *fc=NULL;
    if((fo=fopen("original.txt","r"))==NULL)
    {
        printf("Fail to open original.txt!\n");
        exit(0);
    }
    if((fc=fopen("compressed.txt","w"))==NULL)
    {
        printf("Fail to open compressed.txt!\n");
        exit(0);
    }
    char ch;
    do
    {
        fscanf(fo,"%c",&ch);
        if(ch!='#')
        {
            for(int i=0; i<n; i++)
            {
                if(huffnode[i].value==ch)
                {
                    for(int j=huffcode[i].start; j<MAXBIT; j++)
                    {
                        fprintf(fc,"%d",huffcode[i].bit[j]);
                        (*chnum)++;
                    }
                }
            }
        }
    }
    while (ch!='#');
    fclose(fo);
    fclose(fc);
}
void bin2text()
{
    FILE *fc=NULL;
    FILE *fa=NULL;
    if((fc=fopen("compressed.txt","r"))==NULL)
    {
        printf("Fail to open original.txt!\n");
        exit(0);
    }
    if((fa=fopen("ASCII.txt","w"))==NULL)
    {
        printf("Fail to open ASCII.txt!\n");
        exit(0);
    }
    int i = 0, j = 0, iTemp = 0;
    char str[8];
    char cChar;
    int a=0;
    while(fscanf(fc, "%c", &str[a%8])!=EOF)
    {
        if(a%8==7)
        {
            iTemp = 1;
            cChar = 0;
            for (j = 7; j >= 0; j--)
            {
                cChar += (str[j]-'0') * iTemp;
                iTemp *= 2;
            }
            fprintf(fa, "%c", cChar);
        }
        a++;
    }
    fclose(fc);
    fclose(fa);
}
void text2bin()
{
    SetConsoleOutputCP(437);//ָ��CMD��ʾ�ķ�ʽΪӢ�ģ�������������ʾASCII��128-255�е��ַ�
    FILE *fa=NULL;
    FILE *fs=NULL;
    if((fa=fopen("ASCII.txt","r"))==NULL)
    {
        printf("Fail to open ASCII.txt!\n");
        exit(0);
    }
    if((fs=fopen("small.txt","w"))==NULL)
    {
        printf("Fail to open small.txt!\n");
        exit(0);
    }
    int ch;
    char temp;
    int str[8];
    while(fscanf(fa, "%c", &temp)!=EOF)
    {
        ch=temp+128;
        int i=7;
        while(i>=0)
        {
            str[i]=ch%2;
            ch=ch/2;
            i--;
        }
        for(i=0;i<8;i++)
        {
            fprintf(fs, "%d", str[i]);
        }
    }

    fclose(fa);
    fclose(fs);
}

void Calculate(CHARACTER chars[],int n,int chnum,int chartotal,CodeNode huffcode[])   //����ѹ����
{
    int total=0;
    float length =0;
    for(int i=0; i<n; i++)
    {
        total+=chars[i].count;
    }
    for(int i=0; i<n; i++)
    {
        length+=chars[i].count *1.0/chartotal*(MAXBIT-huffcode[i].start);
    }
    printf("\nƽ�����볤��%f",length);
    printf("\nѹ����Ϊ%f%%",length/8*100);
    printf("\nѹ����Ϊ%f%%",chnum*1.0/8/total*100);
}
void Decoding(HTNODE huffnode[],int n)   //����
{
    FILE *fc=NULL,*fd=NULL;
    if((fc=fopen("small.txt","r"))==NULL)
    {
        printf("Fail to open compressed.txt!\n");
        exit(0);
    }
    if((fd=fopen("decoding.txt","w"))==NULL)
    {
        printf("Fail to open decoding.txt!\n");
        exit(0);
    }
    char code[100000];
    fscanf(fc,"%s",code);
    int i=0,j=0,temp;
    for(i=0; code[i]!='\0'; i++);
    while(j<i)
    {
        temp=2*n-2;
        while(huffnode[temp].lchild!=-1&&huffnode[temp].rchild!=-1)
        {
            if(code[j]=='0')
                temp=huffnode[temp].lchild;
            else
                temp=huffnode[temp].rchild;
            j++;
        }
        fprintf(fd,"%c",huffnode[temp].value);
    }
    fclose(fc);
    fclose(fd);
}
int main()
{
   SetConsoleOutputCP(437);//ָ��CMD��ʾ�ķ�ʽΪӢ�ģ�������������ʾASCII��128-255�е��ַ�����Ҫ��ʾ���ģ�
   SetConsoleOutputCP(936);

    CHARACTER chars[MAXLEAF];
    HTNODE huffnode[MAXNODE];
    CodeNode huffcode[MAXLEAF];
    HEAP heap;
    heap.n=0;
    int n=0,chnum=0;
    Statistic(chars,&n);
    int chartotal;
    chartotal=Displaych(chars,n);
    CreateHT_heap(huffnode,chars,n,&heap);
    //CreateHT(huffnode,n,chars);
    Coding(huffnode,huffcode,n);
    printf("\nÿ���ַ��Ĺ���������Ϊ\n");
    Display(huffnode,huffcode,n);
    CodingFile(huffnode,huffcode,n,&chnum);
    bin2text();
    text2bin();
    Calculate(chars,n,chnum,chartotal,huffcode);
    Decoding(huffnode,n);
}
