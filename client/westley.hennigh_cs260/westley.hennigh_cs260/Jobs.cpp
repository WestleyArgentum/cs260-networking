#include "Jobs.h"
void sendJob::update()
{
	// construct a message from a chunk of data
	FileDataMsg message;
	message.data = data.GetChunk(currChunk++);

	// send that message across
  sSock->Send(&message);
}
void sendJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;

	// set up our session with the socket
  static_cast<ReliableUdpSocet*>(sSock)->Connect(loPort, IP, rePort);
}

void recJob::update()
{
  IMessage* mess;
  mess = sSock->Recv();
  char* buffer = (char*) malloc(sizeof(char) * strlen(static_cast<FileDataMsg*>(mess)->data.c_str())-1);
  buffer = const_cast<char*>(static_cast<FileDataMsg*>(mess)->data.c_str());
  data.SetChunk(buffer, static_cast<FileDataMsg*>(mess)->chunknum);
}
void recJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;

	// set up our session with the socket
  static_cast<ReliableUdpSocet*>(sSock)->Connect(loPort, IP, rePort);
}
