#ifndef JOBS_H
#define JOBS_H

#include "splitter.h"
#include "Socket.hpp"

class jobs
{
  public:
    virtual ~jobs() = 0;
    virtual void update() = 0;
    virtual void SetSocket(SuperSocket* sSock_) = 0;
};

class sendJob : public jobs
{
  public:
    sendJob(char* filename, unsigned loPort_, char* IP_, unsigned rePort_)
    :data(filename), sSock(NULL), loPort(loPort_), IP(IP_), rePort(rePort_) {};
    virtual ~sendJob();
    virtual void update();
    virtual void SetSocket(SuperSocket* sSock_);
  private:
    Data data;
    SuperSocket* sSock;
    unsigned loPort;
    char* IP;
    unsigned rePort;
};

class recJob : public jobs
{
  public:
    recJob(char* filename, unsigned loPort_, char* IP_, unsigned rePort_)
    :data(filename), sSock(NULL), loPort(loPort_), IP(IP_), rePort(rePort_) {};
    virtual ~recJob();
    virtual void update();
    virtual void SetSocket(SuperSocket* sSock_);
  private:
    Data data;
    SuperSocket* sSock;
    unsigned loPort;
    char* IP;
    unsigned rePort;
};

#endif //Jobs
