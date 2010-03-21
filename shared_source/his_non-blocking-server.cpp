#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <string>
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")


using namespace std;

DWORD WINAPI HandleSocket(void* param);

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

	//our listener socket
	SOCKET listenerSocket;
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
	//socketAddress.sin_addr.s_addr  = inet_addr("127.0.0.1");

	//create our socket
	listenerSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	//handle errors
	if(listenerSocket == INVALID_SOCKET){
		printf("WSASocket call failed with error: %ld\n", WSAGetLastError());
		WSACleanup();	
		return 0;
	}

	//bind our listener socket to local endpoint
	result = bind(listenerSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
	if(result == SOCKET_ERROR)
		return WSAGetLastError();

	//now we listen for someone to connect.  This will put the socket into a listening
	//state, but will not automatically accept a connection
	result = listen(listenerSocket, 10);	
	if(result == SOCKET_ERROR)
		return WSAGetLastError();

	cout << "listening for connection" << endl;

	//we need to have another socket here.  When we accept a connection, that connection
	//will give us a socket that is effectively identical to the socket that we were
	//listening with, however this new socket will be part of a socket pair
	//with the remote socket that is connecting.
	SOCKET clientSocket = NULL;
	sockaddr_in remoteEndpoint;
	SecureZeroMemory(&remoteEndpoint, sizeof(remoteEndpoint));

	int endpointSize = sizeof(remoteEndpoint);


	//listen for data
	int rcounter = 0;

	//we're going to keep listening for data until the client closes the 
	//connection.  if you wanted to accept additional clients, you would need
	//to do this differently with additional threads.
	while(true)
	{

		//wait for a new socket
		clientSocket = accept(listenerSocket, (sockaddr*)&remoteEndpoint, &endpointSize);

		if(clientSocket == INVALID_SOCKET)
			cout << "error: " << WSAGetLastError() << endl;
		else
		{
			cout << "accepted a new connection" << endl;
			//now create a new thread and pass in the socket
			DWORD threadHandle;
			CreateThread(NULL, 0, HandleSocket, (void*)&clientSocket, 0, &threadHandle);	
		}
	}

	printf("Shutdown received, ending loop");

	//we only need to shut down the listener socket since the threads shut down everything else
	shutdown(listenerSocket, SD_BOTH);

	//close the sockets
	closesocket(listenerSocket);

	WSACleanup();
	cin.get();

	return 0;
}


//function to handle our Socket on its own thread.
//param- SOCKET* that is connected to a client
DWORD WINAPI HandleSocket(void* param){

	//get our socket out of param.  
	//NOTE:  bad idea to just pass a socket here, would be better design to 
	//create some sort of data transfer object/struct that could be expanded to 
	//hold additional data.
	SOCKET s = *(SOCKET*)param;

	// make socket non-blocking
	u_long val = 1;
	ioctlsocket(s, FIONBIO, &val);
	// notice that this code ONLY makes this one particular socket non-blocking.  The 
	// socket that we called listen() and accept() on is still blocking.
	// if we were to make a new socket now for some reason, it would be blocking
	// until we called ioctlsocket on that new socket.

	//counter and buffer
	int bytesread = 0;	
	int errCode;
	char buffer[BUF_LEN];

	while(true){
		//receive
		bytesread = recv(s, buffer, BUF_LEN, 0);

		//error check
		if(bytesread == SOCKET_ERROR)
		{
			errCode << WSAGetLastError();
			if(errCode == WSAEWOULDBLOCK)
			{
				Sleep(1);
				continue;
			}
			else
			{
				cout << errCode;


				//shutdown and close on error
				shutdown(s, SD_BOTH);
				closesocket(s);

				return 0;
			}
		}


		//check for socket being closed by the client
		if(bytesread == 0){

			//shutdown our socket, it closed
			shutdown(s, SD_BOTH);
			closesocket(s);
			return 0;
		}

		//write the output to the screen.  Not sure if cout is thread safe so
		//it might be a good idea to have some sort of syncronization here
		cout.write(buffer, bytesread);
		cout << endl;
	}
}
