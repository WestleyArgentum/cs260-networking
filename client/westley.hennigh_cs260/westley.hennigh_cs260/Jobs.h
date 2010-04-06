#ifndef JOBS_H
#define JOBS_H

#include <string>

#include "splitter.h"
#include "Socket.hpp"
#include "Window.hpp"

class jobs
{
  public:
		jobs() : done(false) {}
		virtual ~jobs() {};
    virtual bool update() = 0;
    virtual void SetSocket(SuperSocket* sSock_) = 0;
		virtual void start () = 0;
		virtual void end () = 0;
		bool done;
};

class sendJob : public jobs
{
  public:
    sendJob(std::string filename, unsigned filesize, unsigned loPort_, std::string remoteuser, std::string IP_ = std::string(), unsigned rePort_ = 0);
    virtual ~sendJob();
    virtual bool update();
    virtual void SetSocket(SuperSocket* sSock_);
		std::string GetRemoteUser();
		void SetRemoteInfo (std::string IP_, unsigned rePort_);
		virtual void start ();
		virtual void end();
  private:
    Data data;
    SuperSocket* sSock;
    unsigned loPort;
		std::string IP;
    unsigned rePort;
		unsigned currChunk;
		std::string remote_user;
};

class recJob : public jobs
{
  public:
    recJob(std::string filename, unsigned loPort_, std::string IP_, unsigned rePort_, unsigned filesize);
    virtual ~recJob();
    virtual bool update();
    virtual void SetSocket(SuperSocket* sSock_);
		virtual void start ();
		virtual void end();
  private:
    Data data;
    SuperSocket* sSock;
    unsigned loPort;
    std::string IP;
    unsigned rePort;
};

#endif //Jobs
