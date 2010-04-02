#include "Jobs.h"
void sendJob::update(char* chunk)
{
  sSock->Send(&IMessage(RequestFileTransfer_Msg));
}
void sendJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;
  static_cast<ReliableUdpSocet*>(sSock))->Connect(loPort, IP, rePort);
}

void recJob::update(char* chunk)
{
  IMessage* mess;
  mess = sSock->Recv();
}
void recJob::SetSocket(SuperSocket* sSock_)
{
  sSock = sSock_;
  static_cast<ReliableUdpSocet*>(sSock))->Connect(loPort, IP, rePort);
}
