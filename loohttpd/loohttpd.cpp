//*****************************************************************************
//@ProjectName      ZYhttpd
//@Description      my http server
//@Author           NicoleRobin
//@Date             2015/02/09
//*****************************************************************************
#include <cstdio>
#include <WinSock2.h>
using namespace std;
#define BUFFER_SIZE 1024
#define HOST "127.0.0.1"
#define PORT 81
#define HEADER "\
HTTP/1.1 200 OK\r\n\
Content-Type: text/html; charset=UTF-8\r\n\
Server: ZYhttp_v1.0.1\r\n\
Content-Length: %d\r\n\r\n%s\
"
#define HTML "\
<html>\
    <head>\
        <title>ZYhttp</title>\
    <head>\
    <body>\
        <h1>Hello ZYhttp!</h1>\
    </body>\
</html>\
"
#pragma comment(lib, "WS2_32")
int main(int argc, char **argv)
{
	// define and init an server sockaddr
	sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
	addrServer.sin_port = htons(PORT);
	// init socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 0);
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		printf("Init socket dll error!");
		exit(1);
	}
	// create socket
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == socketServer)
	{
		printf("Create socket error!");
		exit(1);
	}
	// bind server socket host
	if (SOCKET_ERROR == bind(socketServer, (LPSOCKADDR)&addrServer, sizeof(addrServer)))
	{
		printf("Bind server host failed!");
		exit(1);
	}
	// listen
	if (SOCKET_ERROR == listen(socketServer, 10))
	{
		printf("Listen failed!");
		exit(1);
	}

	while (true)
	{
		printf("Listening ... \n");
		sockaddr_in addrClient;
		int nClientAddrLen = sizeof(addrClient);
		SOCKET socketClient = accept(socketServer, (sockaddr*)&addrClient, &nClientAddrLen);
		if (SOCKET_ERROR == socketClient)
		{
			printf("Accept failed!");
			break;
		}
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		if (recv(socketClient, buffer, BUFFER_SIZE, 0) < 0)
		{
			printf("Recvive data failed!");
			break;
		}
		printf("Recv data : \n%s", buffer);
		// response 
		memset(buffer, 0, BUFFER_SIZE);
		sprintf_s(buffer, HEADER, strlen(HTML), HTML);
		if (send(socketClient, buffer, strlen(buffer), 0) < 0)
		{
			printf("Send data failed!");
			break;
		}
		printf("Send data : \n%s", buffer);
		closesocket(socketClient);
	}

	closesocket(socketServer);
	WSACleanup();
	return 0;
}