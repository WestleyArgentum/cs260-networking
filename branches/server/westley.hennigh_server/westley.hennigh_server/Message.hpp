// Westley Hennigh
// Message.hpp : The collection of messages
// CS260 Assignment 2
// Feb 22th 2010

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>

enum Message_Type
{
	Invalid_Type,
	RequestForUsername_Msg,
	Username_Msg,
	RemoveUser_Msg,
	ChatData_Msg,
	NUM_TYPES
};


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

	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		int total_size = sizeof(unsigned) + sizeof(Message_Type);

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		return total_size;
	}

	Message_Type my_type;  // every message will have a type associated with it
};


struct UsernameMsg : public IMessage
{
	UsernameMsg (Message_Type type) : IMessage(type) {}

	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = sizeof(unsigned) + sizeof(Message_Type) + myname.size();

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (2*sizeof(unsigned)), myname.c_str());

		return total_size;
	}

	std::string myname;
};


struct ChatDataMsg : public IMessage
{
	ChatDataMsg (Message_Type type) : IMessage(type) { /*memset(text, 0, 256);*/ }

	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = sizeof(unsigned) + sizeof(Message_Type) + text.size();

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (2*sizeof(unsigned)), text.c_str());

		return total_size;
	}

	std::string text;
};


struct RemoveUserMsg : public IMessage
{
	RemoveUserMsg (Message_Type type) : IMessage(type) {}

	virtual unsigned WriteOut (char* buffer)
	{
		// calculate the total size of the message
		unsigned total_size = sizeof(unsigned) + sizeof(Message_Type) + user.size();

		// set up the size of the message
		*reinterpret_cast<unsigned*>(buffer) = total_size;

		// set up the type of the message
		*reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) = my_type;

		// copy the string over
		strcpy(buffer + (2*sizeof(unsigned)), user.c_str());

		return total_size;
	}

	std::string user;
};


/*
ConstructMessage

Make sure to modify me. I have not quite found a disconnect that would let me just put a
virtual method in IMessage that could always construct the right thing. I'm working on it.
*/
IMessage* ConstructMessage(char* buffer);

#endif