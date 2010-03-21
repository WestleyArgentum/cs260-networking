// Westley Hennigh
// main.cpp: The main for the client
// CS260 Assignment 2
// Feb 22th 2010

#include <string>
#include <sstream>

#include "Window.hpp"
#include "Message.hpp"
#include "Client.hpp"

#define BUFF_SIZE 1024

// handles to the controls -----
HWND edit;
HWND output;
Client* zclient;
HWND listbox;

// --------------

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// main -----------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
									 HINSTANCE hPrevInstance,
									 LPSTR lpCmdLine,
									 int nCmdShow)
{
	MakeSillyWindow(WndProc, hInstance, nCmdShow);

	// make zeh client
	Client teh_client("foo!", 58008);  // give it temp var's so we can hack in the actual values
  zclient = &teh_client;

	MSG msg1;
	while (!teh_client.GetRdy())
	{
		if(PeekMessage(&msg1, NULL, 0, 0, PM_REMOVE))
		{
			if(msg1.message == WM_QUIT){
				PostQuitMessage(0);
				break;
			}
			TranslateMessage(&msg1);
			DispatchMessage(&msg1);
		}
	}

	teh_client.Connect();

	MSG msg;
	IMessage* message;
	while (teh_client.StillConnected())
	{
		// if we update the screen without sending a message then
		// the screen won't be redrawn until a WM_PAINT message is sent.
		// there are ways around this that we'll talk about in class
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT){
				PostQuitMessage(0);
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// look for data coming in
		message = teh_client.Receive();

		if (message)
		{
			switch(message->my_type)
			{
			case ChatData_Msg:
				{
					// now send the buffer to the actual read-only textbox
					char temp [BUFF_SIZE];
					SendMessage(output, WM_GETTEXT, BUFF_SIZE-1, (LPARAM)temp);
					std::string temp2(temp);
					temp2.append(static_cast<ChatDataMsg*>(message)->text);
					temp2.append("\r\n");
					SendMessage(output, WM_SETTEXT, 0, (LPARAM)temp2.c_str());
					break;
				}

			case RemoveUser_Msg:
				{
					int pos = SendMessage(listbox, LB_FINDSTRING, -1, (LPARAM)(static_cast<RemoveUserMsg*>(message)->user));
					SendMessage(listbox, LB_DELETESTRING, pos - 1, NULL);
					break;
				}

			case Username_Msg:
				SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)(static_cast<UsernameMsg*>(message)->myname));
				break;

			case RequestForUsername_Msg:
			case Invalid_Type:
			case NUM_TYPES:
				//^! something has gone horribly wrong
				break;
			}
		}
	}

	teh_client.ShutDown();

	return 0;
}
// ----------------------------------------------------------------------------


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int horrible_hack = 0;
	TCHAR buffer[BUFF_SIZE] = {0};
	int charcount = 0;
	int controlID = 0;
	switch (message)
	{	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);	
		// this is where updates to the screen actually happen.
		// In general you don't need to worry about this so you can
		// ignore it for this assignment.

		// TextOut(hdc,
		//    5, 5,
		//  greeting, _tcslen(greeting));
		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;

		// a command message was sent to the window.  What to do?
	case WM_COMMAND:

		// check to see if this is an ID for one of your controls
		controlID = LOWORD(wParam);
		if(controlID != 0){
			switch(controlID){

				// quit button was pressed
	case ID_QUITBUTTON:
		PostQuitMessage(0);
		zclient->ShutDown();

		break;

		// ok button was pressed
	case ID_OKBUTTON:

		// send a message to get the text
		memset(buffer, 0, BUFF_SIZE - 1);
		charcount = (int)SendMessage(edit, WM_GETTEXT, BUFF_SIZE - 1, (LPARAM)buffer);

		// do the nasty
		if (horrible_hack == 0)
		{
			// give it as the ip address -----
			zclient->ipAddress = buffer;
			// -----

			++horrible_hack;
			break;
		}
		else if (horrible_hack == 1)
		{
			// give it as the port -------
			std::stringstream converter;
			converter << std::string(buffer);
			converter >> zclient->remote_port;
			// --------

			++horrible_hack;
			break;
		}
		else if (horrible_hack == 2)
		{
			// give it as a username -----
			zclient->username = buffer;
			// -----

			++horrible_hack;
			zclient->rdy = 1;
			break;
		}

    ChatDataMsg new_message(ChatData_Msg);
		new_message.text = zclient->username;
		new_message.text.append(": ");
		new_message.text.append(buffer);
		//new_message.text.append("0");

		/*strcpy(new_message.text, zclient->username.c_str());
		strcpy(new_message.text + zclient->username.size(), ": ");
    strcpy(new_message.text + zclient->username.size() + 2, buffer);*/

    zclient->Send(new_message);

		//// is there text?
		//if(charcount > 0){
		//	// how many characters of text?  If this were unicode, counting the bytes
		//	// would be more complicated but it's not unicode so we don't care.
		//	_itoa_s(charcount, itoabuf, 65, 10);

			// show how many characters we just read
			//MessageBox(NULL, itoabuf, _T("Chars read"), NULL);

			// show what those characters are
			//MessageBox(NULL, buffer, _T("Some text I just got"), NULL);

			// first copy the message that's in our textbox
			/*char tempbuffer[BUFF_SIZE];
			memcpy_s(tempbuffer, BUFF_SIZE, buffer, charcount + 1);*/

			// now send the buffer to the actual read-only textbox
			//SendMessage(output, WM_SETTEXT, 0, (LPARAM)tempbuffer);
		break;

			}


  }else{

			switch(wParam)
			{
			case BN_CLICKED:

				// button with no id was clicked.  which button?
				int n = GetDlgCtrlID((HWND)lParam);
				// we can get its handle here and then do something with it if we wanted to	
				break;
			}
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		zclient->ShutDown();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
