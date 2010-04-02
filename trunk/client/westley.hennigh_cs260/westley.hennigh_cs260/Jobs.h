#ifndef JOBS_H
#define JOBS_H

#include "splitter.h"
#include "Socket.hpp"

class jobs
{
  public:
    virtual ~jobs() = 0;
    virtual void update() = 0;
    virtual void SetSocket() = 0;
};

class sendJob : public jobs
{
  public:
    sendJob(char* filename):data(filename), sSock(NULL){};
    virtual ~sendJob();
    virtual void update(char* chunk);
    virtual void SetSocket(SuperSocket* sSock_);
  private:
    Data data;
    SuperSocket* sSock;
};

class recJob : public jobs
{
  public:
    recJob(char* filename):data(filename), sSock(NULL){};
    virtual ~recJob();
    virtual void update(char* chunk);
    virtual void SetSocket(SuperSocket* sSock_);
  private:
    Data data;
    SuperSocket* sSock;
};

#endif //Jobs
