// Westley Hennigh
// Message.cpp : The ConstructMessage function
// CS260 Assignment 2
// Feb 22th 2010

#include "Message.hpp"

IMessage* ConstructMessage(char* buffer)
{
	// the second thing buffer is an int that is the enum type of the message.
	// switch on it.
	switch( *( reinterpret_cast<unsigned*>(buffer + sizeof(unsigned)) ) )
	{
	case Invalid_Type:  // horrible error
	case NUM_TYPES:     // horrible error
		{
			std::cout << "A horrible message type has been passed."  << std::endl;
			return NULL;
			break;  // lol
		}

	case RequestForUsername_Msg:
		{
			// no data exists for a request for username, set it up and return
			return new IMessage(RequestForUsername_Msg);
			break;
		}

	case Username_Msg:
		{
			// we need to read potentially 256 bytes for the username
			UsernameMsg* message = new UsernameMsg (Username_Msg);
			memcpy(message->myname, buffer + (2 * sizeof(unsigned)), *reinterpret_cast<unsigned*>(buffer) - (2 * sizeof(unsigned)));
			return message;
			break;
		}

	case ChatData_Msg:
		{
			// we need to read potentially 256 bytes for the message
			ChatDataMsg* message = new ChatDataMsg (ChatData_Msg);
			message->text = buffer + (2 * sizeof(unsigned)), *reinterpret_cast<unsigned*>(buffer) - (2 * sizeof(unsigned));
			//memcpy(message->text, buffer + (2 * sizeof(unsigned)), *reinterpret_cast<unsigned*>(buffer) - (2 * sizeof(unsigned)));
			return message;
			break;
		}

	default:
		{
			return NULL;
			break;
		}
	}
}