// Westley Hennigh
// Window.cpp : simple wrapper around windows shit
// CS260 Assignment 2
// Feb 22th 2010

#include "Window.hpp"
#include <tchar.h>

// The string that appears in the application's title bar.
const static TCHAR szTitle[] = _T("Meep_Meep");

// The main window class name.
const static TCHAR szWindowClass[] = _T("win32app");

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


void MakeSillyWindow(LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM), HINSTANCE hInstance, int nCmdShow)
{
	//set up your main window class.  this class will hold all of your controls
	//and will be used for the main window
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//register the class so that you can use it in the future
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			szTitle,
			NULL);

		return;
	}

	//hInst = hInstance; // Store instance handle in our global variable

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application doews not have a menu bar
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
			szTitle,
			NULL);

		return;
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


	listbox = CreateWindow(
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
	SendMessage(edit, EM_SETLIMITTEXT, (WPARAM)200, 0);
	SendMessage(output, EM_SETLIMITTEXT, (WPARAM)25, 0);



	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);


	UpdateWindow(hWnd);
}