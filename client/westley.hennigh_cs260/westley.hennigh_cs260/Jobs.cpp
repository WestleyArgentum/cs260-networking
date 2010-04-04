#include "Jobs.h"

sendJob::sendJob(std::string filename, unsigned filesize, unsigned loPort_, std::string remoteuser, std::string IP_ /*= std::string()*/, unsigned rePort_ /*= 0*/ )
:data(filename.c_str()), sSock(NULL), loPort(loPort_), IP(IP_), rePort(rePort_), currChunk(0), remote_user(remoteuser)
{
	data.ResizeChunk(filesize);
}

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

sendJob::~sendJob()
{}

std::string sendJob::GetRemoteUser()
{
	return remote_user;
}

void sendJob::SetRemoteInfo( std::string IP_, unsigned rePort_ )
{
	IP = IP_;
	rePort = rePort_;
}

recJob::recJob(std::string filename, unsigned loPort_, char* IP_, unsigned rePort_, unsigned filesize)
:data(filename.c_str()), sSock(NULL), loPort(loPort_), IP(IP_), rePort(rePort_)
{
  data.ResizeChunk(filesize);
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

recJob::~recJob()
{}