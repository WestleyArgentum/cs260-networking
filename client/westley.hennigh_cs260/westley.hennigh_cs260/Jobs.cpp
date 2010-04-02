#include "Jobs.h"

//sendJob::sendJob(char* filename):data(filename){};
//sendJob::~sendJob();
void sendJob::update(char* chunk)
{
  sSock->Send(&IMessage(RequestFileTransfer_Msg));
}
void sendJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;
}

//recJob::recJob(char* filename):data(filename){};
//recJob::~recJob();
void recJob::update(char* chunk)
{
  IMessage* mess;
  mess = sSock->Recv();
}
void recJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;
}
