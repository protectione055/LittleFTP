#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>

#define FILENAME_SIZE 50
#define BUFF_SIZE 1024

void error(const char*);

int main(void) {

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	/*Create clientSock*/
	SOCKET clientSock = socket(PF_INET, SOCK_STREAM, 0);

	/*Input the ip*/
	char ip[20];
	fputs("Please input server's ip: ", stdout);
	fgets(ip, sizeof(ip), stdin);

	/*Config the servAddr*/
	SOCKADDR_IN servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(2200);
	servAddr.sin_addr.s_addr = inet_addr(ip);

	if (connect(clientSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR_IN)) < 0) error("Connect error");
	printf("Successfully connected to the server %s\n", ip);

	char filename[FILENAME_MAX];
	short flag = 0;
	/*Determine whatever the file was opened successfully on server*/
	while (!flag) {
		printf("Please input the filename: ");
		scanf("%s", filename);

		send(clientSock, filename, sizeof(filename), 0);

		if (recv(clientSock, (char*)&flag, sizeof(short), 0) < 0) error("Receive error");//receive the flag message from server
		else printf("flag = %hd\n", flag);
	}

	FILE* file = fopen(filename, "wb");
	if (file == NULL) error("Failed to open file");
	else printf("Start to receive the file...\n");

	int rc;
	char buff[BUFF_SIZE] = { 0 };
	do {
		rc = recv(clientSock, buff, sizeof(buff), 0);
		printf("%dbytes received\n", rc);
		if(fwrite(buff, 1, rc, file) < 0) error("File write error");
	} while (rc > 0);
	printf("File received successfully\n");
	
	fclose(file);
	closesocket(clientSock);
	WSACleanup();
	return 0;
	
}

void error(const char* message) {
	fprintf(stderr, "%s\n", message);
	system("pause");
	exit(1);
}