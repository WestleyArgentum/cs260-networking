//get rid of all the crap in windows.h that we won't ever use
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

//usual stuff
#include <iostream>
#include <string>

//winsock
#include "winsock2.h"

//need for getnameinfo() function
#include "Ws2tcpip.h"

//help the compiler figure out where winsock lives.  You could
//also add this to the linker but this means no 
//extra settings for the .csproj file
#pragma comment(lib, "ws2_32.lib")

using namespace std;

//length of our buffers
const int BUF_LEN = 255;


int main(void){

	//our buffer
	char sendBuffer[BUF_LEN];

	SecureZeroMemory(sendBuffer, BUF_LEN);

	int ret = 0;

	//start up winsock
	WSADATA wsaData;
	SecureZeroMemory(&wsaData, sizeof(wsaData));
	ret = WSAStartup(MAKEWORD(2,2), &wsaData);

	if(ret != 0)
	{
		cout << "Error initializing winsock!  Error was: " << ret << endl;
		return ret;
	}

	struct sockaddr_in socketAddress;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = 0;

	hostent* localhost;
	localhost = gethostbyname("");
	char* localIP;
	localIP = inet_ntoa(*(in_addr*)*localhost->h_addr_list);

	socketAddress.sin_addr.s_addr  = inet_addr(localIP);

	// the remote socket that we'll be sending data to.  This is hard coded with the 
	// port on the local machine but all of you know better than to do this by now, right?
	struct sockaddr_in remoteAddress;
	remoteAddress.sin_family = AF_INET;
	remoteAddress.sin_port = htons(8000u);
	remoteAddress.sin_addr.s_addr  = inet_addr(localIP);

	// and the socket, similar to a TCP socket but note the different parameters
	SOCKET clientSocket;	
	clientSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

	if(clientSocket == INVALID_SOCKET)
	{
		int errorcode = WSAGetLastError();
		cout << "Error on creating socket: " << errorcode << endl;
		return errorcode;
	}


	ret = bind(clientSocket, (sockaddr*)&socketAddress, sizeof(socketAddress));
	if(ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		return ret;
	}

	sprintf(sendBuffer, "Hello, world");

	ret = sendto(clientSocket, sendBuffer, BUF_LEN, 0, (sockaddr*)&remoteAddress, sizeof(remoteAddress));
	if(ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		return ret;
	}


	ret = closesocket(clientSocket);
	if(ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		return ret;
	}

	WSACleanup();

	cout << "done" << endl;

	cin.get();


	return 0;

}
