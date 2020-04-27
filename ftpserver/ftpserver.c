#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<winsock2.h>

#define MAX_SIZE 1024

void error(const char*);

int main(void) {

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET servSock = socket(PF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(2200);

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == -1) error("Bind error");

	if (listen(servSock, 1) == -1) error("Listen error");
	printf("FTP server is running on port: 2200\n");
	printf("Waiting for client...\n");

	SOCKADDR_IN clientAddr;
	int szSockaddr = sizeof(SOCKADDR_IN);
	SOCKET clientSock = accept(servSock, (SOCKADDR*)&clientAddr, &szSockaddr);
	if (clientSock == INVALID_SOCKET) error("clientSocket accpet error");
	printf("Client %s connected.\n", inet_ntoa(clientAddr.sin_addr));

	short flag = 0;
	FILE* file = NULL;
	char buff[MAX_SIZE];
	while (!flag) {
		recv(clientSock, buff, sizeof(buff), 0);
		char* filename = strdup(buff);//Retrive the file name
		file = fopen(filename, "rb");
		if (file != NULL) {
			flag = 1;
			send(clientSock, (char*)&flag, sizeof(short), 0);
		}
		else {
			printf("Failed to open file %s\n%s\n", filename, strerror(errno));
			memset(buff, 0, sizeof(buff));
		}
	}
		 

	printf("Starting to transmit the file...\n");

	int rc = 0;
	while ((rc = fread(buff, 1, sizeof(buff), file)) > 0) {
		printf("%dbytes sended\n", rc);
		if (send(clientSock, buff, rc, 0) < 0) error("Send error");
	}

	printf("Transmission completed.\n");

	fclose(file);
	closesocket(servSock);
	closesocket(clientSock);
	WSACleanup();
	return 0;

}

void error(const char* message) {
	fprintf(stderr, message);
	system("pause");
	exit(1);
}