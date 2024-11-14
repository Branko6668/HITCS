#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20

typedef struct list
{
    int data[MAXSIZE]; /* ���飬�洢����Ԫ�� */
    int length;             /* ���Ա�ǰ���� */
} SeqList;

int GetElem(SeqList L, int i, int* e)/* �����������e����L�е�i������Ԫ�ص�ֵ,ע��i��ָλ�ã���1��λ�õ������Ǵ�0��ʼ */
{
    if (L.length == 0 || i<1 || i>L.length)
        return 0;
    *e = L.data[i - 1];
}

int LocateElem(SeqList L, int e)/* �������������L�е�1����e�����ϵ������Ԫ�ص�λ�� */
{
    int i;
    if (L.length == 0)
        return 0;
    for (i = 0; i < L.length; i++)
    {
        if (L.data[i] == e)
            break;
    }
    if (i >= L.length)
        return 0;

    return i + 1;
}

int ListInsert(SeqList* L, int i, int e)/* �����������L�е�i��λ��֮ǰ�����µ�����Ԫ��e��L�ĳ��ȼ�1 */
{
    int k;
    if (L->length == MAXSIZE)  /* ˳�����Ա��Ѿ��� */
        return 0;
    if (i<1 || i>L->length + 1)/* ��i�ȵ�һλ��С���߱����һλ�ú�һλ�û�Ҫ��ʱ */
        return 0;

    if (i <= L->length)        /* ����������λ�ò��ڱ�β */
    {
        for (k = L->length - 1; k >= i - 1; k--) /* ��Ҫ����λ��֮�������Ԫ������ƶ�һλ */
            L->data[k + 1] = L->data[k];
    }
    L->data[i - 1] = e;          /* ����Ԫ�ز��� */
    L->length++;

    return 1;
}

void ListDelete(SeqList* L, int e)/*ɾ������Ԫ��*/
{
    int k = 0;//��¼L�е���x������Ԫ�ظ���
    for (int i = 0; i < L->length; i++)
    {
        if (L->data[i] == e)
            k++;
        else
            L->data[i - k] = L->data[i];
    }
    L->length -= k;
}

void DeleteList(SeqList* L)/*ɾ���ظ�Ԫ��*/
{
    int i = 0;
    int k = 0;//k���ظ��Ĵ�������Ҫɾ�������ĸ���
    for (i = 0; i < L->length - 1; i++)
    {
        if (L->data[i] != L->data[i + 1])
            L->data[i - k + 1] = L->data[i + 1];
        else
            k++;
    }

    L->length = L->length - k;
}

void BubbleSort(SeqList* L)/*����*/
{
    int i, j, temp;
    for (i = 0; i < L->length - 1; i++)
    {
        for (j = 1; j < L->length - i; j++)
        {
            if (L->data[j] < L->data[j - 1])
            {
                temp = L->data[j];
                L->data[j] = L->data[j - 1];
                L->data[j - 1] = temp;
            }
        }
    }
}
void Coverts(SeqList* L)     //��˳����е�Ԫ������
{
    int i, n;
    int temp;
    n = L->length;              //n Ϊ���Ա�*A �ĳ���
    for (i = 0; i < n / 2; i++)  //ʵ������
    {
        temp = L->data[i];
        L->data[i] = L->data[n - i - 1];
        L->data[n - i - 1] = temp;
    }
}

void Cirmove(SeqList* L, int k)//��˳����е�Ԫ��ѭ������Kλ
{
    int temp;
    int i, j;
    for (i = 1; i < k + 1; i++)
    {

        temp = L->data[L->length - 1];
        for (j = L->length - 1; j > 0; j--)
        {
            L->data[j] = L->data[j - 1];
        }
        L->data[0] = temp;

    }
}
void Union(SeqList* La, SeqList Lb)//�ϲ��������ź�������Ա�
{
    int La_len, Lb_len, i;
    int e;                        /*������La��Lb��ͬ������Ԫ��e*/
    La_len = La->length;            /*�����Ա�ĳ��� */
    Lb_len = Lb.length;
    for (i = 1; i <= Lb_len; i++)
    {
        GetElem(Lb, i, &e);              /*ȡLb�е�i������Ԫ�ظ���e*/
        if (!LocateElem(*La, e))        /*La�в����ں�e��ͬ����Ԫ��*/
            ListInsert(La, ++La_len, e); /*����*/
    }
}
void Print(SeqList* L)       //���˳���
{
    int i;
    for (i = 0; i < L->length; i++)
    {
        printf("%4d", L->data[i]);
    }
    printf("\n");
}

int main()
{
    SeqList L;
    SeqList Lb;
    int e;
    int i = 0;
    L.length = 0;
    printf("input values of L\n");
    while (1)
    {
        scanf("%d", &e);
        L.data[i] = e;
        L.length++;
        i++;
        if (getchar() == '\n') break;
    }
    int j = 0;
    Lb.length = 0;
    printf("input values of Lb\n");
    while (1)
    {
        scanf("%d", &e);
        Lb.data[j] = e;
        Lb.length++;
        j++;
        if (getchar() == '\n') break;
    }
    printf("which element do you want to delete in L\n");//ɾ���ض�Ԫ��
    scanf("%d", &e);
    ListDelete(&L, e);
    Print(&L);

    printf("Remove duplicate elements in L\n");//ɾ���ظ�Ԫ��
    BubbleSort(&L);
    DeleteList(&L);
    Print(&L);

    printf("The inversion of L\n");//����
    Coverts(&L);
    Print(&L);

    printf("��ѭ����λ\n");
    int k;
    scanf("%d", &k);
    Cirmove(&L, k);
    Print(&L);

    printf("The union of L and Lb\n");//�ϲ�
    BubbleSort(&L);
    BubbleSort(&Lb);
    Union(&L, Lb);
    Print(&L);

    return 0;
}
