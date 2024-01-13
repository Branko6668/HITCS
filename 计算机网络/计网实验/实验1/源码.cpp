#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <string.h>
#include <tchar.h>
#include <fstream>
#include<string>
#include<iostream>

using namespace std;

#pragma comment(lib,"Ws2_32.lib")
#define MAXSIZE 65507 //�������ݱ��ĵ���󳤶�
#define HTTP_PORT 80 //http �������˿�

//���ڴ洢 HTTP ����ͷ����Ϣ
struct HttpHeader {
	char method[4]; // �洢 HTTP ������POST ���� GET��ע����ЩΪ CONNECT����ʵ���ݲ�����
	char url[1024];  // ����� url
	char host[1024]; // Ŀ������
	char cookie[1024 * 10]; //cookie
	HttpHeader() {// ���캯������ʼ���ṹ��ĳ�Ա����
		ZeroMemory(this, sizeof(HttpHeader));// ���ṹ����ڴ�ռ�����
	}
};

//�û�����
char ForbiddenIP[1024][17];// �洢����ֹ���ʵ� IP ��ַ�б�
int IPnum = 0;// ��¼����ֹ���ʵ� IP ��ַ������

//������վ
char fishUrl[1024][1024];// �洢������վ�� URL �б�
int fishUrlnum = 0;// ��¼������վ�� URL ������

BOOL InitSocket();// ��ʼ���׽��ֿ�ʹ���������׽���
void ParseHttpHead(char* buffer, HttpHeader* httpHeader);// ���� HTTP ����ͷ���洢���ṹ����
int ParseCacheHttpHead(char* buffer, HttpHeader* httpHeader);// ���������е� HTTP ��Ӧͷ���洢���ṹ����
BOOL ConnectToServer(SOCKET* serverSocket, char* host);// ���ӵ�Ŀ������������ط������׽���
unsigned int __stdcall ProxyThread(LPVOID lpParameter);// �̺߳�������������ÿ���ͻ�������Ĵ����߳�

BOOL ParseDate(char* buffer, char* field, char* tempDate);// �� HTTP ��Ӧͷ�н��������ֶ�
void makeNewHTTP(char* buffer, char* value);// �޸� HTTP ����ͷ�е��������� URL
void makeFilename(char* url, char* filename); // ���� URL ���ɻ����ļ���
void makeCache(char* buffer, char* url);// �� HTTP ��Ӧ����д�뻺���ļ���
void getCache(char* buffer, char* filename);// �ӻ����ļ��ж�ȡ HTTP ��Ӧ����

//������ز���
BOOL haveCache = FALSE;// ����Ƿ��л����ļ�����
BOOL needCache = TRUE;// ����Ƿ���Ҫ�����ļ�
char* strArr[100];//�洢 HTTP ��Ӧͷ�еĸ����ֶ�

bool ForbiddenToConnect(char* httpheader);// �ж��Ƿ��ֹ����ĳ����վ
bool GotoFalseWebsite(char* url);// �ж��Ƿ���Ҫ��ת��������վ
void ParseCache(char* buffer, char* status, char* last_modified);// ���������ļ��е� HTTP ״̬�������޸�����
bool UserIsForbidden(char* userID);  //�û����ˣ��ж��Ƿ��ֹĳ���û����ʴ��������

//������ز���
SOCKET ProxyServer;// ��ʾ������������׽���
sockaddr_in ProxyServerAddr;// ��ʾ����������ĵ�ַ��Ϣ
const int ProxyPort = 10240;// ��ʾ����������ļ����˿�

struct ProxyParam {// ���ݸ������̵߳Ĳ���
	SOCKET clientSocket;// ��ʾ�ͻ��˵��׽���
	SOCKET serverSocket;// ��ʾĿ����������׽���
};

int _tmain(int argc, _TCHAR* argv[])// �����ֱ��ʾ�����в����ĸ�����ֵ
{
	printf("�����������������\n");
	printf("��ʼ��...\n");
	if (!InitSocket()) {
		printf("socket ��ʼ��ʧ��\n");
		return -1;
	}
	printf("����������������У������˿� %d\n", ProxyPort);
	SOCKET acceptSocket = INVALID_SOCKET;// ��ʾ���ܿͻ������ӵ���ʱ�׽���
	SOCKADDR_IN acceptAddr;// ��ʾ�ͻ��˵ĵ�ַ��Ϣ
	ProxyParam* lpProxyParam;// ָ�򴫵ݸ������̵߳Ĳ���
	HANDLE hThread;// ��ʾ�����̵߳ľ��
	DWORD dwThreadID;// ��ʾ�����̵߳� ID
	//������������ϼ���
	char client_IP[16];// �洢�ͻ��˵� IP ��ַ�ַ���
	//���ý���IP
	memcpy(ForbiddenIP[IPnum++], "127.0.0.1", 10);
	//���÷�����Щ��վ�ᱻ�ض��򵽵�����վ
	memcpy(fishUrl[fishUrlnum++], "http://pku.edu.cn/", 18);

	while (true) {
		
		/* ���ܴ���������׽����ϵĿͻ�����������
		������һ���µ��׽�������ͻ���ͨ�ţ�ͬʱ���ͻ��˵ĵ�ַ��Ϣ�洢�� acceptAddr �ṹ����*/
		acceptSocket = accept(ProxyServer, (SOCKADDR*)&acceptAddr, NULL);


		//�����û�IP����
		int ff = sizeof(acceptAddr);
		//���ܴ���������׽����ϵĿͻ����������󣬲�����һ���µ��׽�������ͻ���ͨ�ţ�
		acceptSocket = accept(ProxyServer, (SOCKADDR*)&acceptAddr, &(ff));
		printf("��ȡ�û�IP��ַ��%s\n",inet_ntoa(acceptAddr.sin_addr));
		memcpy(client_IP, inet_ntoa(acceptAddr.sin_addr), 16);// ���ͻ��˵� IP ��ַ�ַ������Ƶ� client_IP ������
		if (UserIsForbidden(client_IP))
		{
			printf("***********��IP�ѱ�����***************\n");
			closesocket(acceptSocket);
			exit(0);
		}


		lpProxyParam = new ProxyParam;//�ڶ��ϴ���һ�� ProxyParam �ṹ�����
		if (lpProxyParam == NULL) {
			continue;
		}
		lpProxyParam->clientSocket = acceptSocket;// �� acceptSocket �׽��ָ�ֵ�� lpProxyParam ָ��Ľṹ������ clientSocket ��Ա����
		
		hThread = (HANDLE)_beginthreadex(NULL, 0,
			&ProxyThread, (LPVOID)lpProxyParam, 0, 0);// ����һ���µĴ����̣߳�����������
		CloseHandle(hThread);
		Sleep(500);
	}
	closesocket(ProxyServer);
	WSACleanup();
	return 0;
}

// Qualifier: ��ʼ���׽���
BOOL InitSocket() {
	//�����׽��ֿ⣨���룩
	WORD wVersionRequested;
	WSADATA wsaData; // �洢�׽��ֿ����Ϣ
	//�׽��ּ���ʱ������ʾ
	int err;
	//�汾 2.2
	wVersionRequested = MAKEWORD(2, 2);
	//���� dll �ļ� Scoket ��
	err = WSAStartup(wVersionRequested, &wsaData);//���� WSAStartup ��������ʼ���׽��ֿ�
	if (err != 0) {// ��� err ���������� 0����ʾ��ʼ���׽��ֿ�ʧ��
		//�Ҳ��� winsock.dll
		printf("���� winsock ʧ�ܣ� �������Ϊ: %d\n", WSAGetLastError());
		return FALSE;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)// ��� wsaData �ṹ���е� wVersion ��Ա�����ĵ��ֽڻ���ֽڲ����� 2����ʾ�����ҵ���ȷ���׽��ֿ�汾
	{
		printf("�����ҵ���ȷ�� winsock �汾\n");
		WSACleanup();
		return FALSE;
	}
	ProxyServer = socket(AF_INET, SOCK_STREAM, 0);// ���� socket ����������һ��������������׽��֣���������ֵ��ֵ�� ProxyServer ����
	if (INVALID_SOCKET == ProxyServer) {//��� ProxyServer �������� INVALID_SOCKET ֵ����ʾ�����׽���ʧ��
		printf("�����׽���ʧ�ܣ��������Ϊ��%d\n", WSAGetLastError());
		return FALSE;
	}
	
	ProxyServerAddr.sin_family = AF_INET;// �ڽ� AF_INET ֵ��ֵ�� ProxyServerAddr �ṹ���е� sin_family ��Ա��������ʾ��ַ��Ϊ IPv4
	ProxyServerAddr.sin_port = htons(ProxyPort);// �� ProxyPort ����ת��Ϊ�����ֽ����ֵ�� ProxyServerAddr �ṹ���е� sin_port ��Ա��������ʾ�˿ں�Ϊ ProxyPort
	ProxyServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;// �� INADDR_ANY ֵ��ֵ�� ProxyServerAddr �ṹ���е� sin_addr.S_un.S_addr ��Ա��������ʾ IP ��ַΪ�����ַ
	
	if (bind(ProxyServer, (SOCKADDR*)&ProxyServerAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {// ������� bind ������������������׽������ַ��Ϣ��ʧ��
		printf("���׽���ʧ��\n");
		return FALSE;
	}
	if (listen(ProxyServer, SOMAXCONN) == SOCKET_ERROR) {// ������� listen ������������������׽�������Ϊ����״̬ʧ��
		printf("�����˿�%d ʧ��", ProxyPort);
		return FALSE;
	}
	return TRUE;
}

// Qualifier: �߳�ִ�к���
unsigned int __stdcall ProxyThread(LPVOID lpParameter) {
	char Buffer[MAXSIZE], fileBuffer[MAXSIZE];//�洢�ӿͻ��˻���������յ��ı���
	ZeroMemory(Buffer, MAXSIZE);
	char sendBuffer[MAXSIZE];//�洢Ҫ���͸��ͻ��˻������������
	ZeroMemory(sendBuffer, MAXSIZE);
	char FishBuffer[MAXSIZE];/// �洢Ҫ���͸��ͻ��˵ĵ�����վ��Ӧ����
	ZeroMemory(FishBuffer, MAXSIZE);
	char* CacheBuffer;// ָ�򻺴��е�����
	SOCKADDR_IN clientAddr;// �洢�ͻ��˵ĵ�ַ��Ϣ
	int length = sizeof(SOCKADDR_IN);// ��ʾ clientAddr �ṹ��Ĵ�С
	int recvSize;// �洢���յ������ݴ�С
	int ret;// �洢���ͻ���պ����ķ���ֵ

	//���տͻ��˵�����
	recvSize = recv(((ProxyParam*)lpParameter)->clientSocket, Buffer, MAXSIZE, 0);// ���� recv �����ӿͻ����׽��ֽ�������

	memcpy(sendBuffer, Buffer, recvSize);// �� Buffer �����е� recvSize ���ֽڸ��Ƶ� sendBuffer ������
	
	HttpHeader* httpHeader = new HttpHeader(); // �ڶ��ϴ���һ�� HttpHeader �ṹ�����
	
	CacheBuffer = new char[recvSize + 1]; //�ڶ��ϴ���һ�� recvSize + 1 ��С���ַ����飬�������ַ��ֵ�� CacheBuffer ָ�����
	ZeroMemory(CacheBuffer, recvSize + 1);
	memcpy(CacheBuffer, Buffer, recvSize);// �� Buffer �����е� recvSize ���ֽڸ��Ƶ� CacheBuffer ָ���������
	
	ParseHttpHead(CacheBuffer, httpHeader);//���� ParseHttpHead �������� HTTP ����ͷ���洢�� httpHeader ָ��Ľṹ����


	//����
	char* DateBuffer;// ָ�������ֶ�����
	DateBuffer = (char*)malloc(MAXSIZE); // �ڶ��Ϸ��� MAXSIZE ��С���ڴ�ռ䣬�������ַ��ֵ�� DateBuffer ָ�����
	ZeroMemory(DateBuffer, strlen(Buffer) + 1);
	memcpy(DateBuffer, Buffer, strlen(Buffer) + 1);// �� Buffer �����е� strlen(Buffer) + 1 ���ֽڸ��Ƶ� DateBuffer ָ���������
	char filename[100];//�洢�����ļ���
	ZeroMemory(filename, 100);
	makeFilename(httpHeader->url, filename);// ���� makeFilename �������� URL ���ɻ����ļ��������洢�� filename ������
	char* field = (char*)"Date";// ָ�������ֶε������ַ���
	char date_str[30];  //�����ֶ�Date��ֵ
	ZeroMemory(date_str, 30);
	ZeroMemory(fileBuffer, MAXSIZE);
	
	FILE* in;//��ȡ���ػ���
	if ((in = fopen(filename, "rb")) != NULL) {
		printf("\n**********��ȡ���ػ���****************\n");
		fread(fileBuffer, sizeof(char), MAXSIZE, in);
		fclose(in);
		ParseDate(fileBuffer, field, date_str);//��ȡ�����ļ��е� HTTP ��Ӧͷ������ȡ���е������ֶ�
		printf("date_str:%s\n", date_str);
		makeNewHTTP(Buffer, date_str);
		haveCache = TRUE;
		goto success;
	}
	delete CacheBuffer;

	if (!ConnectToServer(&((ProxyParam*)lpParameter)->serverSocket, httpHeader->host)) {// ���� ConnectToServer �������ӵ�Ŀ�������ʧ��
		goto error;
	}
	printf("������������ %s �ɹ�\n", httpHeader->host);


	//������վ��Ϣ
	if (ForbiddenToConnect(httpHeader->url))// ���� ForbiddenToConnect �����ж��Ƿ��ֹ����ĳ����վ
	{
		printf("*************��������� %s *******************\n", httpHeader->url);
		goto error;

	}

	//��վ����  ����http://pku.edu.cn/  �ض��� http://today.hit.edu.cn/
	if (GotoFalseWebsite(httpHeader->url))
	{

		char* pr;// ָ�� FishBuffer �����еĵ�ǰλ��
		int fishing_len = 0;//ʹ��fishing_len����¼�Ѷ�ȡ���ĵĳ��ȣ��Է���������޸ĺ��汨��
		fishing_len = strlen("HTTP/1.1 302 Moved Temporarily\r\n");
		memcpy(FishBuffer, "HTTP/1.1 302 Moved Temporarily\r\n", fishing_len);
		pr = FishBuffer + fishing_len;
		fishing_len = strlen("Connection:keep-alive\r\n");
		memcpy(pr, "Connection:keep-alive\r\n", fishing_len);
		pr = pr + fishing_len;// �� pr �������� fishing_len ��ֵ����ʾ�ƶ�����һ��λ��
		fishing_len = strlen("Cache-Control:max-age=0\r\n");
		memcpy(pr, "Cache-Control:max-age=0\r\n", fishing_len);
		pr = pr + fishing_len;
		//�ض��򵽽��չ�����
		fishing_len = strlen("Location: http://today.hit.edu.cn/\r\n\r\n");
		memcpy(pr, "Location: http://today.hit.edu.cn/\r\n\r\n", fishing_len);
		//��302���ķ��ظ��ͻ���
		ret = send(((ProxyParam*)lpParameter)->clientSocket, FishBuffer, sizeof(FishBuffer), 0); // ���� send ������ͻ����׽��ַ��͵�����վ��Ӧ����
		goto error;
	}
	if (recvSize <= 0) {
		goto error;
	}

success://�л���ֱ�Ӷ�ȡ���͸��ͻ���
	if (!ConnectToServer(&((ProxyParam*)lpParameter)->serverSocket, httpHeader->host)) {// HttpHeader �ṹ���е����������������ӵ�ԭ�������׽���
		printf("����Ŀ�������ʧ�ܣ�����\n");
		goto error;
	}
	printf("������������ %s �ɹ�\n", httpHeader->host);
	//���ͻ��˷��͵� HTTP ���ݱ���ֱ��ת����Ŀ�������
	ret = send(((ProxyParam*)lpParameter)->serverSocket, Buffer, strlen(Buffer) + 1, 0);
	//�ȴ�Ŀ���������������
	recvSize = recv(((ProxyParam*)lpParameter)->serverSocket, Buffer, MAXSIZE, 0);
	if (recvSize <= 0) {
		printf("����Ŀ�������������ʧ�ܣ�����\n");
		goto error;
	}
	//�л���ʱ���жϷ��ص�״̬���Ƿ���304�������򽫻�������ݷ��͸��ͻ���
	if (haveCache == TRUE) {// ��� haveCache �������� TRUE����ʾ�л����ļ�����
		getCache(Buffer, filename);
	}
	if (needCache == TRUE) {// ��� needCache �������� TRUE����ʾ��Ҫ�����ļ�
		makeCache(Buffer, httpHeader->url);  //���汨��
	}
	//��Ŀ����������ص�����ֱ��ת�����ͻ���
	ret = send(((ProxyParam*)lpParameter)->clientSocket, Buffer, sizeof(Buffer), 0);


	//������
error:
	printf("�ر��׽���\n");
	Sleep(200);
	closesocket(((ProxyParam*)lpParameter)->clientSocket);
	closesocket(((ProxyParam*)lpParameter)->serverSocket);
	delete lpParameter;
	_endthreadex(0);
	return 0;

}

//Qualifier:ʵ����վ���ˣ����������ĳЩ��վ
bool ForbiddenToConnect(char* httpheader)
{
	char* forbiddernUrl = (char*)"http://www.hit.edu.cn/";
	if (!strcmp(httpheader, forbiddernUrl))
	{
		return true;
	}
	else
		return false;
}


//Qualifier:ʵ���û����ˣ�����IP
bool UserIsForbidden(char* userID)
{
	for (int i = 0; i < IPnum; i++)// ��������ֹ���ʴ���������� IP ��ַ����
	{
		if (strcmp(userID, ForbiddenIP[i]) == 0)
		{
			//�û�IP�ڽ���IP����
			return true;
		}
	}
	return false;
}

//Qualifier:ʵ�ַ���������ģ����վ
bool GotoFalseWebsite(char* url)
{
	cout << url << endl;
	for (int i = 0; i < fishUrlnum; i++)// �� 0 �� fishUrlnum - 1 ����������վ URL ����
	{
		if (strcmp(url, fishUrl[i]) == 0)
		{
			return true;
		}
	}
	return false;
}


// Qualifier: ���� TCP �����е� HTTP ͷ��
void ParseHttpHead(char* buffer, HttpHeader* httpHeader) {
	char* p;//ָ��ָ����ַ���
	char* ptr;//�洢�ָ������������Ϣ	
	const char* delim = "\r\n";
	
	p = strtok_s(buffer, delim, &ptr);//���� strtok_s ������ buffer ָ����ַ����зָ����һ�����ַ���
	
	if (p[0] == 'G') {//GET ��ʽ
		memcpy(httpHeader->method, "GET", 3);
		memcpy(httpHeader->url, &p[4], strlen(p) - 13);
	}
	else if (p[0] == 'P') {//POST ��ʽ
		memcpy(httpHeader->method, "POST", 4);
		memcpy(httpHeader->url, &p[5], strlen(p) - 14);
	}
	
	p = strtok_s(NULL, delim, &ptr);//���� strtok_s ������ buffer ָ����ַ����м����ָ����һ�����ַ���
	
	while (p) {
		switch (p[0]) {
		case 'H':// ��ʾ HTTP ����ͷ�е��������ֶ�
			memcpy(httpHeader->host, &p[6], strlen(p) - 6);
			break;
		case 'C':// ��ʾ HTTP ����ͷ�п����� Cookie �ֶ�
			if (strlen(p) > 8) {
				char header[8];
				ZeroMemory(header, sizeof(header));
				memcpy(header, p, 6);
				if (!strcmp(header, "Cookie")) {
					memcpy(httpHeader->cookie, &p[8], strlen(p) - 8);
				}
			}
			break;
		default:
			break;
		}
		p = strtok_s(NULL, delim, &ptr);
	}
}


// Qualifier: ������������Ŀ��������׽��֣�������
BOOL ConnectToServer(SOCKET* serverSocket, char* host) {
	sockaddr_in serverAddr;//�洢Ŀ��������ĵ�ַ��Ϣ
	serverAddr.sin_family = AF_INET;// IPv4 Э��
	serverAddr.sin_port = htons(HTTP_PORT);//ʹ�� HTTP �˿ں�
	HOSTENT* hostent = gethostbyname(host);//�������ַ���
	if (!hostent) {
		return FALSE;
	}
	in_addr Inaddr = *((in_addr*)*hostent->h_addr_list);//��ȡĿ��������� IP ��ַ
	serverAddr.sin_addr.s_addr = inet_addr(inet_ntoa(Inaddr));//����Ŀ��������� IP ��ַ
	*serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (*serverSocket == INVALID_SOCKET) {
		return FALSE;
	}
	if (connect(*serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr))
		== SOCKET_ERROR) {
		closesocket(*serverSocket);
		return FALSE;
	}
	return TRUE;
}


//����HTTPͷ����field�ֶΣ����������field�򷵻�true������ȡ����
BOOL ParseDate(char* buffer, char* field, char* tempDate) {
	char* p, * ptr, temp[5];// ָ��ָ����ַ���, �洢�ָ������������Ϣ, ��ʱ����
	const char* delim = "\r\n";
	ZeroMemory(temp, 5);
	p = strtok(buffer, delim);
	int len = strlen(field) + 2;
	while (p) {
		if (strstr(p, field) != NULL) {
			memcpy(tempDate, &p[len], strlen(p) - len);
			return TRUE;
		}
		p = strtok(NULL, delim);
	}
	return TRUE;
}

//����HTTP������
void makeNewHTTP(char* buffer, char* value) {
	const char* field = "Host";// HTTP ����ͷ��������
	const char* newfield = "If-Modified-Since: ";//HTTP ����ͷ�������ֶ�
	char temp[MAXSIZE];
	ZeroMemory(temp, MAXSIZE);
	char* pos = strstr(buffer, field);//�� buffer ָ����ַ����в��� field ָ����ַ���
	int i = 0;
	for (i = 0; i < strlen(pos); i++) {
		temp[i] = pos[i];
	}
	*pos = '\0';
	while (*newfield != '\0') {  //����If-Modified-Since�ֶ�
		*pos++ = *newfield++;
	}
	while (*value != '\0') {
		*pos++ = *value++;
	}
	*pos++ = '\r';
	*pos++ = '\n';
	for (i = 0; i < strlen(temp); i++) {
		*pos++ = temp[i];
	}
}

//����url�����ļ���
void makeFilename(char* url, char* filename) {
	while (*url != '\0') {
		if (*url != '/' && *url != ':' && *url != '.') {
			*filename++ = *url;//�� url ָ����ַ����Ƶ� filename ָ���λ��
		}
		url++;
	}
	strcat(filename, ".txt");
}

//���л���
void makeCache(char* buffer, char* url) {
	char* p, * ptr, num[10], tempBuffer[MAXSIZE + 1]; // ָ��ָ����ַ���, �洢�ָ������������Ϣ, ״̬���ַ���, ��ʱ����
	const char* delim = "\r\n";
	ZeroMemory(num, 10);
	ZeroMemory(tempBuffer, MAXSIZE + 1);
	memcpy(tempBuffer, buffer, strlen(buffer));
	p = strtok(tempBuffer, delim);//�ָ����һ�����ַ���
	memcpy(num, &p[9], 3);
	if (strcmp(num, "200") == 0) {  //״̬����200ʱ����
		char filename[1024] = { 0 };  
		makeFilename(url, filename);// ���� url ָ����ַ������ɻ����ļ���
		printf("filename : %s\n", filename);
		ofstream of;
		of.open(filename, ios::out);
		of << buffer << endl;//�� buffer ָ����ַ���д�뵽 of ������
		of.close();
		printf("\n=====================================\n\n");
		printf("\n***********��ҳ�Ѿ�������**********\n");
	}
}

//��ȡ����
void getCache(char* buffer, char* filename) {
	char* p, * ptr, num[10], tempBuffer[MAXSIZE + 1];//ָ��ָ����ַ���,�洢�ָ������������Ϣ,״̬���ַ���,��ʱ����
	const char* delim = "\r\n";
	ZeroMemory(num, 10);
	ZeroMemory(tempBuffer, MAXSIZE + 1);
	memcpy(tempBuffer, buffer, strlen(buffer));
	p = strtok(tempBuffer, delim);//�ָ����һ�����ַ���
	memcpy(num, &p[9], 3);
	if (strcmp(num, "304") == 0) {  //�������صı����е�״̬��Ϊ304ʱ�����ѻ��������
		printf("\n=====================================\n\n");
		printf("***********�ӱ�����û���**************\n");
		ZeroMemory(buffer, strlen(buffer));
		FILE* in = NULL;
		if ((in = fopen(filename, "r")) != NULL) {
			fread(buffer, sizeof(char), MAXSIZE, in);
			fclose(in);
		}
		needCache = FALSE;
	}
}