// Westley Hennigh
// main.cpp: The main for the client
// CS260 Assignment 2
// Feb 22th 2010

#include <string>
#include <sstream>

#include "Window.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "Defines.hpp"

Client* zclient;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// main -----------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
									 HINSTANCE hPrevInstance,
									 LPSTR lpCmdLine,
									 int nCmdShow)
{
	SillyWindow::GetWindow()->MakeSillyWindow(WndProc, hInstance, nCmdShow);

	// make the client
	Client teh_client("UserInfo.txt");  // give it temp var's so we can hack in the actual values
  zclient = &teh_client;

	teh_client.Connect();

	MSG msg;
	IMessage* message;
	std::string the_conversation;  // only here because windows sucks at readboxes
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
					// now update the conversation and send the text to the read-only textbox
					the_conversation.append(static_cast<ChatDataMsg*>(message)->text);
					the_conversation.append("\r\n");

					SendMessage(SillyWindow::GetWindow()->output, WM_SETTEXT, 0, (LPARAM)the_conversation.c_str());

					// scroll the bar to the bottom
					unsigned linecount = SendMessage(SillyWindow::GetWindow()->output, EM_GETLINECOUNT, 0, 0);
					SendMessage(SillyWindow::GetWindow()->output, EM_LINESCROLL, 0, linecount);
					break;
				}

			case RemoveUser_Msg:
				{
					int pos = SendMessage(SillyWindow::GetWindow()->listbox, LB_FINDSTRING, -1, (LPARAM)(static_cast<RemoveUserMsg*>(message)->user.c_str()));
					SendMessage(SillyWindow::GetWindow()->listbox, LB_DELETESTRING, pos, NULL);
					break;
				}

			case Username_Msg:
				SendMessage(SillyWindow::GetWindow()->listbox, LB_ADDSTRING, 0, (LPARAM)(static_cast<UsernameMsg*>(message)->myname.c_str()));
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
	TCHAR buffer[STD_BUFF_SIZE] = {0};
	int charcount = 0;
	int controlID = 0;
	switch (message)
	{	
	case WM_PAINT:
		BeginPaint(hWnd, &ps);	
		// this is where updates to the screen actually happen.
		// In general you don't need to worry about this so you can
		// ignore it for this assignment.

		EndPaint(hWnd, &ps);
		break;

		// a command message was sent to the window.  What to do?
	case WM_COMMAND:

		// check to see if this is an ID for one of your controls
		controlID = LOWORD(wParam);
		if(controlID != 0)
		{
			switch(controlID)
			{
				// quit button was pressed
			case ID_QUITBUTTON:
				PostQuitMessage(0);
				zclient->ShutDown();

				break;

				// ok button was pressed
			case ID_OKBUTTON:

				// send a message to get the text
				memset(buffer, 0, STD_BUFF_SIZE - 1);
				charcount = (int)SendMessage(SillyWindow::GetWindow()->edit, WM_GETTEXT, STD_BUFF_SIZE - 1, (LPARAM)buffer);

				ChatDataMsg new_message;
				new_message.text = zclient->GetUsername();
				new_message.text.append(": ");
				new_message.text.append(buffer, charcount);

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
		}
		else
		{
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
