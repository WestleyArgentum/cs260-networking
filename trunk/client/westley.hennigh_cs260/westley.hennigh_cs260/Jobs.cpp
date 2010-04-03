#include "Jobs.h"
bool sendJob::update()
{
	// construct a message from a chunk of data
	FileDataMsg message;
	message.data = data.GetChunk(currChunk);
	message.chunknum = currChunk++;

	// send that message across
  sSock->Send(&message);

	return false;
}
void sendJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;

	// set up our session with the socket
  static_cast<ReliableUdpSocet*>(sSock)->Connect(loPort, IP, rePort);
}

bool recJob::update()
{
  IMessage* mess;
  mess = sSock->Recv();

	if(mess)
	{
		if (mess->my_type != FileData_Msg)
			return false;  // something has gone wrong

		data.SetChunk(static_cast<FileDataMsg*>(mess)->data, static_cast<FileDataMsg*>(mess)->chunknum);
		delete(mess);
	}

	return false;
}
void recJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;

	// set up our session with the socket
  static_cast<ReliableUdpSocet*>(sSock)->Connect(loPort, IP, rePort);
}
