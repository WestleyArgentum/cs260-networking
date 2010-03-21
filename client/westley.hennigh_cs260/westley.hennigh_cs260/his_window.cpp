// GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c
#undef UNICODE
#undef _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define ID_EDITCHILD 2
#define ID_QUITBUTTON 100
#define ID_OKBUTTON 101

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Meep_Meep");
const static int outputbuffersize = 1024;

const int windowSizeX = 800;
const int windowSizeY = 600;

const int readOnlyTextBoxSizeX = 500;
const int readOnlyTextBoxSizeY = 400;
const int readOnlyTextBoxPosX = 150;
const int readOnlyTextBoxPosY = 10;

const int textBoxSizeX = 500;
const int textBoxSizeY = 100;
const int textBoxPosX = 150;
const int textBoxPosY = 450;

const int okButtonSizeX = 100;
const int okButtonSizeY = 100;
const int okButtonPosX = 670;
const int okButtonPosY = 450;

const int quitButtonSizeX = 75;
const int quitButtonSizeY = 400;
const int quitButtonPosX = 30;
const int quitButtonPosY = 10;

const int listBoxSizeX = 100;
const int listBoxSizeY = 400;
const int listBoxPosX = 670;
const int listBoxPosY = 10;




HWND edit;
HWND output;

char OutputBuffer[outputbuffersize];


CHAR lpszLatin[] =  "Lorem ipsum dolor sit amet, consectetur "
"adipisicing elit, sed do eiusmod tempor " 
"incididunt ut labore et dolore magna " 
"aliqua. Ut enim ad minim veniam, quis " 
"nostrud exercitation ullamco laboris nisi " 
"ut aliquip ex ea commodo consequat. Duis " 
"aute irure dolor in reprehenderit in " 
"voluptate velit esse cillum dolore eu " 
"fugiat nulla pariatur. Excepteur sint " 
"occaecat cupidatat non proident, sunt " 
"in culpa qui officia deserunt mollit " 
"anim id est laborum."; 



HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// main -----------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
									 HINSTANCE hPrevInstance,
									 LPSTR lpCmdLine,
									 int nCmdShow)
{

	//set up your main window class.  this class will hold all of your controls
	//and will be used for the main window
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//register the class so that you can use it in the future
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Meep_Meep"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application dows not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowSizeX, windowSizeY,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Meep_Meep"),
			NULL);

		return 1;
	}
	//now hWnd is the handle to our main window

	//we're going to create a textbox.  This is exactly the same as creating a window, however 
	//it's actually a child "window" of the parent window that we just created, so it's technically
	//not a window although in some ways it behaves like one
	//We give the class name "EDIT" so that it creates
	//a textbox instead of an actual window
	edit = CreateWindow(
		_T("EDIT"),
		NULL,
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | WS_BORDER,
		textBoxPosX, 
		textBoxPosY,
		textBoxSizeX, 
		textBoxSizeY,
		hWnd,  //instead of NULL, here we pass the handle to the window that we want this control to live in
		NULL,
		hInstance,
		NULL
		);



	output = CreateWindow(
		_T("EDIT"),
		NULL,
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_READONLY | WS_BORDER, //adding readonly for this one
		readOnlyTextBoxPosX, 
		readOnlyTextBoxPosY,
		readOnlyTextBoxSizeX, 
		readOnlyTextBoxSizeY,
		hWnd,//same parent window as before
		NULL,
		hInstance,
		NULL
		);

	HWND hwndButton = CreateWindow( 
		_T("BUTTON"),  // Predefined class; Unicode assumed. 
		_T("OK"),      // Button text. 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER,  // Styles. 
		okButtonPosX,        // x position. 
		okButtonPosY,        // y position. 
		okButtonSizeX,        // Button width.
		okButtonSizeY,        // Button height.
		hWnd,      // Parent window.
		(HMENU)ID_OKBUTTON,      // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
		NULL);      // Pointer not needed.

	HWND hwndQuitButton = CreateWindow(
		_T("BUTTON"),  // Predefined class; Unicode assumed. 
		_T("BIG QUIT"),      // Button text. 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER,  // Styles. 
		quitButtonPosX,        // x position. 
		quitButtonPosY,        // y position. 
		quitButtonSizeX,        // Button width.
		quitButtonSizeY,        // Button height.
		hWnd,      // Parent window.
		(HMENU)ID_QUITBUTTON,      // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
		NULL);      // Pointer not needed.


	HWND listBox = CreateWindow(
		_T("LISTBOX"),
		_T("ListBox1"),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | LBS_NOSEL | LBS_SORT | WS_BORDER,
		listBoxPosX,
		listBoxPosY,
		listBoxSizeX,
		listBoxSizeY,
		hWnd,
		NULL,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	// use SendMessage to control the properties of controls.  pass the handle to the control
	// that you want to set the property of
	// This particular message is useful because it allows you to limit the amount of text that a 
	// user can type into a textbox so that you can ensure that you don't have more text than your 
	// buffer can hold when sending the text accross the network.  Otherwise you'll have to call
	// send and recv multiple times to ensure that you get all of the text.  Don't assume that the text
	// will be null-terminated in the buffer when you call recv just because it is when you called send()
	SendMessage(edit, EM_SETLIMITTEXT, (WPARAM)1000, 0);



	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);


	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;

	// you can send text to an edit control using sendmessage.  string must be null terminated.
	// remember that just because you read a string from the network and the sender null-terminated it
	// it does NOT mean that the data you read is necessarily null-terminated
	SendMessage(edit, WM_SETTEXT, 0, (LPARAM) lpszLatin);
	SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)_T("some string"));
	SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)_T("somestring"));
	SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)_T("somestring32"));

	while (true)
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
	}

	return (int) msg.wParam;
}// ---------------------------------------------------------------------------


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//char greeting[] = "Hello, World!";
	TCHAR buffer[1024];
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
		break;

		// ok button was pressed
	case ID_OKBUTTON:
		//now get the text from the editbox
		TCHAR itoabuf[65];

		// send a message to get the text
		charcount = (int)SendMessage(edit, WM_GETTEXT, 1023, (LPARAM)buffer);

		// is there text?
		if(charcount > 0){
			// how many characters of text?  If this were unicode, counting the bytes
			// would be more complicated but it's not unicode so we don't care.
			_itoa_s(charcount, itoabuf, 65, 10);

			// show how many characters we just read
			//MessageBox(NULL, itoabuf, _T("Chars read"), NULL);

			// show what those characters are
			//MessageBox(NULL, buffer, _T("Some text I just got"), NULL);

			//send some text to the read-only:

			// first copy the message that's in our textbox
			char tempbuffer[outputbuffersize];
			memcpy_s(tempbuffer, outputbuffersize, buffer, charcount + 1);

			// now send the buffer to the actual read-only textbox
			SendMessage(output, WM_SETTEXT, 0, (LPARAM)tempbuffer);
		}
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
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
