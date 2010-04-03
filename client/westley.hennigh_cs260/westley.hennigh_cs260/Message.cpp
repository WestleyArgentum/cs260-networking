// Westley Hennigh
// Message.cpp : The ConstructMessage function
// CS260 Assignment 2
// Feb 22th 2010

#include "Message.hpp"

// instantiation of headersize
const unsigned HEADERSIZE = sizeof(unsigned) + sizeof(Message_Type);

// length function
unsigned length (std::string& str)
{
	return str.size() + 1;
}


IMessage* ConstructMessage(char* buffer)
{
	// append a null to the buffer to the end
	*(buffer + *reinterpret_cast<unsigned*>(buffer)) = 0;

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
			UsernameMsg* message = new UsernameMsg ();
			message->myname = buffer + HEADERSIZE;
			return message;
			break;
		}

	case ChatData_Msg:
		{
			// we need to read potentially 256 bytes for the message
			ChatDataMsg* message = new ChatDataMsg ();
			message->text = buffer + HEADERSIZE;  //, *reinterpret_cast<unsigned*>(buffer) - (2 * sizeof(unsigned));  <--- how the fuck did that compile?
			return message;
			break;
		}

	case RemoveUser_Msg:
		{
			RemoveUserMsg* message = new RemoveUserMsg ();
			message->user = buffer + HEADERSIZE;
			return message;
			break;
		}

	case RequestFileTransfer_Msg:
		{
			RequestFileTransferMsg* message = new RequestFileTransferMsg;
			message->propagator = buffer + HEADERSIZE;
			message->recipient = buffer + HEADERSIZE + length(message->propagator);
			message->port = *( reinterpret_cast<unsigned*>(buffer + HEADERSIZE + length(message->propagator) + length(message->recipient)) );
			message->file_size = *( reinterpret_cast<unsigned*>(buffer + HEADERSIZE + length(message->propagator) + length(message->recipient) + sizeof(unsigned)) );
			return message;
			break;
		}

	case RejectFileTransfer_Msg:
		{
			RejectFileTransferMsg* message = new RejectFileTransferMsg;
			message->propagator = buffer + HEADERSIZE;
			message->recipient = buffer + HEADERSIZE + length(message->propagator);
			return message;
			break;
		}

	case FileData_Msg:
		{
			FileDataMsg* message = new FileDataMsg;
			message->data = buffer + HEADERSIZE;
			return message;
			break;
		}

	case FileDataAck_Msg:
		{
			FileDataAckMsg* message = new FileDataAckMsg;
			message->ack = *reinterpret_cast<unsigned*>(buffer + HEADERSIZE);
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