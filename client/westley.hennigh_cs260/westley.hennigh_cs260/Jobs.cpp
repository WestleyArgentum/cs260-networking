#include "Jobs.h"

sendJob::sendJob(std::string filename, unsigned filesize, unsigned loPort_, std::string remoteuser, std::string IP_ /*= std::string()*/, unsigned rePort_ /*= 0*/ )
:data(filename, filesize), sSock(NULL), loPort(loPort_), IP(IP_), rePort(rePort_), currChunk(0), remote_user(remoteuser)
{
}

bool sendJob::update()
{
	// construct a message from a chunk of data
	FileDataMsg message;
	message.data = data.GetChunk(currChunk);
	message.chunknum = currChunk;
  ++currChunk;

	// send that message across
  sSock->Send(&message);
  Sleep(1);

  if(currChunk == data.GetSize())
    done = true;

	return done;
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

void sendJob::start()
{
	data.SplitFile();
}

void sendJob::end()
{}

recJob::recJob(std::string filename, unsigned loPort_, std::string IP_, unsigned rePort_, unsigned filesize)
:data(filename, filesize), sSock(NULL), loPort(loPort_), IP(IP_), rePort(rePort_)
{
}
bool recJob::update()
{
  IMessage* mess;
  mess = sSock->Recv();

  char buffer_a[10];

  unsigned stuff = data.GetSize();

	if(mess)
	{
		if (mess->my_type != FileData_Msg)
			return false;  // something has gone wrong

		data.SetChunk(static_cast<FileDataMsg*>(mess)->data, static_cast<FileDataMsg*>(mess)->chunknum);

    SendMessage(SillyWindow::GetWindow()->progress, WM_SETTEXT, 0, (LPARAM)itoa((data.GetSize()/data.GetChunkSize())*100, buffer_a, 10));

    if(static_cast<FileDataMsg*>(mess)->chunknum >= data.GetSize()-1)
      done = true;

    delete(mess);
	}

	return done;
}
void recJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;

	// set up our session with the socket
  static_cast<ReliableUdpSocet*>(sSock)->Connect(loPort, IP, rePort);
}

recJob::~recJob()
{}

void recJob::start()
{}

void recJob::end()
{
	data.JoinFiles();
}
