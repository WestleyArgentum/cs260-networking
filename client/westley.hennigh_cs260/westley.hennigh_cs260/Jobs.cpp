#include "Jobs.h"
void sendJob::update()
{
	//^! construct a message from a chunk of data
	FileDataMsg message;
	//message.data = data.getchunk(currchunk++);

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

	//^! pass the received data into the data object
}
void recJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;

	// set up our session with the socket
  static_cast<ReliableUdpSocet*>(sSock)->Connect(loPort, IP, rePort);
}
