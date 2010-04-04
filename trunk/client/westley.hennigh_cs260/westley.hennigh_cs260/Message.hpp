// Westley Hennigh
// Message.hpp : The collection of messages
// CS260 Assignment 2
// Feb 22th 2010

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>
#include <vector>

enum Message_Type  // the different message types
{
	Invalid_Type,
	RequestForUsername_Msg,
	Username_Msg,
	RemoveUser_Msg,
	ChatData_Msg,
	RequestFileTransfer_Msg,
	AcceptFileTransfer_Msg,
	RejectFileTransfer_Msg,
	FileData_Msg,
	FileDataAck_Msg,
	//^! InvalidUsername_Msg,
	NUM_TYPES
};

/*
We need to know the size of the header when we are parsing buffers etc.
The size is not going to change post compile time, however, so it is just a constant.
*/
extern const unsigned HEADERSIZE;

/*
I have grown tired of adding one to the length passed back by string.size.
length(std::string& str) returns the actual length of the string as it will be in memory.
*/
unsigned length (std::string& str);


/*
Much as I despise object based message systems, at this point I don't believe I have any
alternatives that would be better. The solution, as I see it, would be to create a
message system that utilized function binding. The only reasonable way to do that would
be to incorporate both the server and client into one program (indeed literally into one
thing) (because how else could you ensure that the functions you want to call exist).
This approach would actually be very powerful because the superficial distinction between
server and client will be torn away. Instead each node is a client when it is asking or
sending new data and each is a server when it is responding.

The major block to this is the shitty windows framework we are provided with. I am not
going to waste time trying to get the server to work in it.

Derive your message from IMessage and give it whatever data you want, then add your name
to the enum...

Make sure to fill out a WriteOut function that takes all the new data that you stored
and puts it into a buffer that is provided. This buffer will be send and your message
will be re-constructed using it.

Also, put in a case for your message in ConstructMessage so that your message can be
re-constructed.

If you do not have any data that will be added then it is fine to not fill out a struct,
just add a type to the enum and the WriteOut of IMessage will take care of you.
*/
struct IMessage
{
	IMessage (Message_Type type) : my_type(type) {}  // no default ctor, must give type

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = HEADERSIZE;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		return HEADERSIZE;
	}
	// ----------------------------

	/*virtual const IMessage* operator-> () const
	{
		return this;
	}

	template <typename type>
	operator type* () 
	{
		return this;
	}*/


	Message_Type my_type;  // every message will have a type associated with it
};


struct UsernameMsg : public IMessage
{
	UsernameMsg () : IMessage(Username_Msg) {}

	/*virtual const UsernameMsg* operator -> () const
	{
		return this;
	}*/

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = HEADERSIZE + length(myname);

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (HEADERSIZE), myname.c_str());

		return total_size;
	}
	// ----------------------------

	std::string myname;
};


struct ChatDataMsg : public IMessage
{
	ChatDataMsg () : IMessage(ChatData_Msg) {}

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = HEADERSIZE + length(text);

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (HEADERSIZE), text.c_str());

		return total_size;
	}
	// ----------------------------

	std::string text;
};


struct RemoveUserMsg : public IMessage
{
	RemoveUserMsg () : IMessage(RemoveUser_Msg) {}

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = HEADERSIZE + length(user);

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (HEADERSIZE), user.c_str());

		return total_size;
	}
	// ----------------------------

	std::string user;
};


/*
We are going to need to send a range of ports that we can transfer on with a request and the client responding will have
to pick one that works for her and send it with the response (or send a deny if they cant send on those).

Otherwise all hell could break loose.
*/
struct RequestFileTransferMsg : public IMessage
{
	RequestFileTransferMsg () : IMessage(RequestFileTransfer_Msg) {}

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = HEADERSIZE + length(propagator) + length(recipient) + (2 * sizeof(unsigned));

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (HEADERSIZE), propagator.c_str());
		strcpy(buffer + (HEADERSIZE + length(propagator)), recipient.c_str());
		strcpy(buffer + (HEADERSIZE + length(propagator) + length(recipient)), ip_address.c_str());
		strcpy(buffer + (HEADERSIZE + length(propagator) + length(recipient) + length(ip_address)), filename.c_str());
		*reinterpret_cast<unsigned*>(buffer + HEADERSIZE + length(propagator) + length(recipient) + length(ip_address) + length(filename)) = port;
		*reinterpret_cast<unsigned*>(buffer + HEADERSIZE + length(propagator) + length(recipient) + length(ip_address) + length(filename) + sizeof(unsigned)) = file_size;

		return total_size;
	}
	// ----------------------------

	std::string propagator;
	std::string recipient;
	std::string ip_address;
	std::string filename;
	unsigned port;
	unsigned file_size;

	/*
	I need to find a way to wrap up all the file transfer messages in one... or something.
	Also, there needs to be a way to give file transfers a unique id across all the chat going on.
	Maybe something given by the server.
	The reasoning behind this is simply that if two clients were to send two file requests to each other in rapid
	succession then when they got a response it would not always be easy to determine to which request (without
	bloating the request with data that it shouldn't need).
	Generally I think I'll do it this way for the assignment but then find some alternative for my personal use.
	*/
};

struct AcceptFileTransferMsg : public IMessage
{
	AcceptFileTransferMsg () : IMessage(AcceptFileTransfer_Msg) {}

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = HEADERSIZE + length(propagator) + length(recipient) + (2 * sizeof(unsigned));

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (HEADERSIZE), propagator.c_str());
		strcpy(buffer + (HEADERSIZE + length(propagator)) , recipient.c_str());
		strcpy(buffer + (HEADERSIZE + length(propagator) + length(recipient)), ip_address.c_str());
		*reinterpret_cast<unsigned*>(buffer + HEADERSIZE + length(propagator) + length(recipient) + length(ip_address)) = port;
		*reinterpret_cast<unsigned*>(buffer + HEADERSIZE + length(propagator) + length(recipient) + length(ip_address) + sizeof(unsigned)) = file_size;

		return total_size;
	}
	// ----------------------------

	std::string propagator;
	std::string recipient;
	std::string ip_address;
	unsigned port;
	unsigned file_size;
};

struct RejectFileTransferMsg : public IMessage
{
	RejectFileTransferMsg () : IMessage(RejectFileTransfer_Msg) {}

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = HEADERSIZE + length(propagator) + length(recipient);

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (HEADERSIZE), propagator.c_str());
		strcpy(buffer + (HEADERSIZE + length(propagator)) , recipient.c_str());

		return total_size;
	}
	// ----------------------------

	std::string propagator;
	std::string recipient;
};


struct FileDataMsg : public IMessage
{
	FileDataMsg () : IMessage(FileData_Msg) {}

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = HEADERSIZE + (2 * sizeof(unsigned)) + data.size();

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the vector over, the first 4 bytes are the size
		*reinterpret_cast<unsigned*>(buffer + HEADERSIZE) = data.size();
		memcpy(buffer + HEADERSIZE + sizeof(unsigned), &(data[0]), data.size());

		// copy over the chunk number
		*reinterpret_cast<unsigned*>(buffer + HEADERSIZE + sizeof(unsigned) + data.size()) = chunknum;

		return total_size;
	}
	// ----------------------------

	std::vector<char> data;
	unsigned chunknum;
	//unsigned TransferId;  // not yet set up (will be transer id issued by the server)
};

struct FileDataAckMsg : public IMessage
{
	FileDataAckMsg () : IMessage(FileDataAck_Msg) {}

	// Serialization for message -------------------
	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = HEADERSIZE + sizeof(unsigned);

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the ack over
		*reinterpret_cast<unsigned*>(buffer + 2 * sizeof(unsigned)) = ack;

		return total_size;
	}
	// ----------------------------

	unsigned ack;
	//unsigned TransferId;  // not yet set up (will be transer id issued by the server)
};


/*
ConstructMessage

Make sure to modify me.

//^! I need to remove this by adding in a base class for message creators and then a templated class
		 that will serve as a creator for any type of message. That is a far superior method of serialization.
*/
IMessage* ConstructMessage(char* buffer);

#endif