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
  if(sSock->Send(&message) == -1)
    done = true;  // they left... what to do?

  Sleep(1);  // we dont keep a page size so this makes things cleaner

  if(currChunk == data.GetSize())
    done = true;

	return done;
}
void sendJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;
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
  // set up our session with the socket
  static_cast<ReliableUdpSocet*>(sSock)->Connect(loPort, IP, rePort);

  // set up the data object
	data.SplitFile();
}

void sendJob::end()
{
  // terminate
  static_cast<ReliableUdpSocet*>(sSock)->Disconnect();
}

recJob::recJob(std::string filename, unsigned loPort_, std::string IP_, unsigned rePort_, unsigned filesize)
:data(filename, filesize), sSock(NULL), loPort(loPort_), IP(IP_), rePort(rePort_), ack(0)
{
}
bool recJob::update()
{
  static unsigned timeout = GetTickCount();
  
  // if it has been more than 25 sec since we got a message they are probably gone...
  if(GetTickCount() > timeout + 25000)
    done = true;

  IMessage* mess;
  mess = sSock->Recv();

  unsigned stuff = data.GetSize();

	if(mess)
	{
    // reset the timeout
    timeout = GetTickCount();

		if (mess->my_type != FileData_Msg)
			return false;  // something has gone wrong

		FileDataMsg* fmsg = static_cast<FileDataMsg*>(mess);

		// if the packet is new (and therefore relevant)
		if(fmsg->chunknum == ack)
		{
			data.SetChunk(static_cast<FileDataMsg*>(mess)->data, fmsg->chunknum);
      char char_percent[16];
      float float_percent = ((float)data.GetChunkSize()/(float)data.GetSize())*100.0f;
      sprintf(char_percent, "%f", float_percent);
			SendMessage(SillyWindow::GetWindow()->progress, WM_SETTEXT, 0, (LPARAM)(char_percent));
		
			// finished check
			if(fmsg->chunknum >= data.GetSize()-1)
				done = true;

      ++ack;
		}

    delete(mess);
	}

	return done;
}
void recJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;
}

recJob::~recJob()
{}

void recJob::start()
{
  // set up our session with the socket
  static_cast<ReliableUdpSocet*>(sSock)->Connect(loPort, IP, rePort);
}

void recJob::end()
{
  // terminate
  static_cast<ReliableUdpSocet*>(sSock)->Disconnect();

  // unite!
	data.JoinFiles();
}
