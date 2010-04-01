#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <string>
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")


using namespace std;

//buffer length, this is arbitrary, but note that I use a const and not a #define
const int BUF_LEN = 255;

int main(void){

	int result = 0;

	//WSAData, same as in the client
	WSADATA wsData;

	//host info
	char* localIP;
	hostent* localhost;

	//our receive buffer
	char receiveBuffer[BUF_LEN];
	SecureZeroMemory(receiveBuffer, BUF_LEN);

	//initialize winsock
	result = WSAStartup(MAKEWORD(2,2), &wsData);
	if(result){
		printf("Error starting winsock: %d", result);
		return result;
	}

	SOCKET serverSocket;
	struct sockaddr_in socketAddress;

	//get local host IP, same as in client demo
	localhost = gethostbyname("");
	localIP = inet_ntoa(*(in_addr*)*localhost->h_addr_list);

	cout << "Locaohost: " << localhost->h_name << endl;
	cout << "Local IP: " << localIP << endl;

	//create local endpoint for listening.
	socketAddress.sin_family = AF_INET;
	//this time, set the port to something that's greater than 1024
	socketAddress.sin_port = htons(8000);
	socketAddress.sin_addr.s_addr  = inet_addr(localIP);

	//create our socket
	serverSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

	//handle errors
	if(serverSocket == INVALID_SOCKET){
		printf("WSASocket call failed with error: %ld\n", WSAGetLastError());
		WSACleanup();	
		return 0;
	}

	//bind our listener socket to local endpoint
	result = bind(serverSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
	if(result == SOCKET_ERROR)
		return WSAGetLastError();

	int rcounter = 0;

	while(true){

		// this will hold whatever the remote address that sent the datagram was.  this struct can be of
		// variable length
		sockaddr_in remoteAddress;
		SecureZeroMemory(&remoteAddress, sizeof(remoteAddress));

		// you must pass a pointer to the size for recvfrom so be sure to do this.  If you really care to know 
		// why, come by my office hours
		int remoteAddresslength = sizeof(remoteAddress);

		rcounter = recvfrom(serverSocket, receiveBuffer, BUF_LEN, 0, (SOCKADDR*)&remoteAddress, &remoteAddresslength);
		if(rcounter == SOCKET_ERROR){
			rcounter = WSAGetLastError();
			cout << "ERROR: " << rcounter << endl;
			break;
		}

		//print whatever we just got.
		string s;
		s.assign(receiveBuffer, rcounter);
		cout << s << endl;
	}


	//close the sockets
	closesocket(serverSocket);

	WSACleanup();

	cin.get();

	return 0;
}
