// IS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include "cJSON.h"
#include "math.h"
#include "wlan.h"
#pragma comment(lib,"wlan.lib")
#pragma comment(lib,"ws2_32.lib")

static char *id = "123";
static char *secret = "123";
static SOCKET tcpsock;

HANDLE g_hmutex = NULL;

#define BUFFLEN 512
#define PORT 4567

struct position{
	double x;
	double y;
}posi;

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	//send location function
	cJSON *root ,*x,*y;
	root = cJSON_CreateObject();
	x = cJSON_CreateNumber(posi.x);
	y = cJSON_CreateNumber(posi.y);
	cJSON_AddItemToObject(root,"x",x);
	cJSON_AddItemToObject(root,"y",y);

	char *mess;
	
	for(;;){
		mess = cJSON_Print(root);
		if(send(tcpsock,mess,strlen(mess),0)<0){
			printf("send message failed\n");
			exit(2);
		}
		WaitForSingleObject(g_hmutex,INFINITE);
		posi.x += 10;
		posi.y += 50*sin((double)(posi.x - 100));
		x->valuedouble = posi.x;
		y->valuedouble = posi.y;
		ReleaseMutex(g_hmutex);
		Sleep(500);
	}
	return 0;
}

DWORD WINAPI threadwlan(LPVOID lpParameter)
{
	char *tar = "MI";
	char *tarkey = "123456qaz";
	changetarget(tar,tarkey);
	in();
	
	printf("searching MI ...\n");
	for(;;){
		for(;;){
			if(searchwlan()){
				printf("MI is searched\n");
				
				if (!isconnected()) {
					connectwlan();
				}else{
					printf("connect success\n");
					break;
				}
				Sleep(3000);
				
			}else{
				setwlanInterfacesoff();
				setwlanInterfaceson();
				printf("MI can not be searched\n");
				Sleep(800);
			}
		}
		for(;;){
			//¼ì²éwifiÁ¬½Ó×´Ì¬
            searchwlan();
            if(isconnected()){
                printf("wlan connected\n");
                Sleep(6000);
            }else{
                setwlanInterfacesoff();
                setwlanInterfaceson();
                break;
            }
		}

	}
	wlanfree();
	return 0;
}

void init();

int main(int argc, char* argv[])
{
	/*
	HANDLE hthread2 = CreateThread(NULL,0,threadwlan,NULL,0,NULL);
	CloseHandle(hthread2);*/
	char *tar = "MI";
	char *tarkey = "123456qaz";
	changetarget(tar,tarkey);
	in();
	
	printf("searching MI ...\n");
	for(;;){
		if(searchwlan()){
			printf("MI is searched\n");
			
			if (!isconnected()) {
				connectwlan();
			}else{
				printf("connect success\n");
				break;
			}
			Sleep(5000);
			
		}else{
			setwlanInterfacesoff();
			setwlanInterfaceson();
			printf("MI can not be searched\n");
			Sleep(2000);
		}
	}

	wlanfree();

	init();
    return 0;
}

void init(){
	//initialization
	g_hmutex = CreateMutex(NULL,FALSE,NULL);
    SOCKET s;
	struct sockaddr_in local,si_other;
	int slen,recv_len;
	char buf[BUFFLEN];
	WSADATA wsa;

	slen = sizeof(si_other);

	if(WSAStartup(MAKEWORD(2,2),&wsa) != 0){
		printf("failed.error code: %d",WSAGetLastError());
		exit(1);
	}
	
	if(INVALID_SOCKET == (s = socket(AF_INET,SOCK_DGRAM,0))){
		printf("could not create socket:%d",WSAGetLastError());
	}
	printf("socket created\n");

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(PORT);

	if(SOCKET_ERROR == bind(s,(struct sockaddr *)&local,sizeof(local))){
		printf("bind failed with error code:%d",WSAGetLastError());
		exit(1);
	}

	memset(buf,'\0',BUFFLEN);

	if(SOCKET_ERROR == (recv_len = recvfrom(s,buf,BUFFLEN,0,(struct sockaddr *)&si_other,&slen))){
		printf("recvfrom() failed with error code:%d",WSAGetLastError());
		exit(1);
	}

	printf("Received packet from %s:%d\n",inet_ntoa(si_other.sin_addr),ntohs(si_other.sin_port));
	printf("Data:%s\n",buf);


	cJSON *root = cJSON_Parse(buf);
	char *ip = cJSON_GetObjectItem(root,"ip")->valuestring;
	int tcpport = cJSON_GetObjectItem(root,"port")->valueint;

	
	struct sockaddr_in server;
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip); 
	server.sin_port = htons(tcpport);

	if(INVALID_SOCKET == (tcpsock = socket(AF_INET,SOCK_STREAM,0))){
		printf("socket error:%d",WSAGetLastError());
		exit(1);
	}
	
	if(connect(tcpsock,(struct sockaddr *)&server,sizeof(server))<0){
		printf("connect failed :%d",WSAGetLastError());
		exit(1);
	}
	
	cJSON *checmess;
	checmess = cJSON_CreateObject();
	cJSON_AddStringToObject(checmess,"id",id);
	cJSON_AddStringToObject(checmess,"secret",secret);
	char *mess = cJSON_Print(checmess);
	printf("send mess:%s\n",mess);
	
	if(send(tcpsock,mess,strlen(mess),0) <0){
		printf("send check message failed \n");
		exit(1);
	}

	int rec_size;
	char rec[BUFFLEN];
	if(SOCKET_ERROR == (rec_size = recv(tcpsock,rec,BUFFLEN,0))){
		printf("receive message failed!\n");
		exit(1);
	}
	rec[rec_size] = '\0';
	printf("receive message:%s\n",rec);
	
	posi.x = 100;
	posi.y = 100;
	
	HANDLE hthread1 = CreateThread(NULL,0,ThreadProc,NULL,0,NULL);
	CloseHandle(hthread1);
	for(;;){
		if(SOCKET_ERROR == (rec_size = recv(tcpsock,rec,BUFFLEN,0))){
			printf("receive message failed!\n");
			exit(1);
		}
		rec[rec_size] = '\0';
		printf("receive message:%s\n",rec);		
	}
	closesocket(tcpsock);
	closesocket(s);
	WSACleanup();
	cJSON_Delete(root);
	cJSON_Delete(checmess);
}