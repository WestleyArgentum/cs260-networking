// Westley Hennigh
// Window.hpp : simple wrapper around windows shit
// CS260 Assignment 2
// Feb 22th 2010

#undef UNICODE
#undef _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define ID_EDITCHILD 2
#define ID_QUITBUTTON 100
#define ID_OKBUTTON 101

extern HWND edit;
extern HWND output;
extern HWND listbox;

/*
This is really a cop-out. I should make a window class that has all sorts of
neat shiny tweak-able features and stuff. Screw that though. I will labor for as
long as it takes on almost any code, just not windows bull.
*/

void MakeSillyWindow(LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM), HINSTANCE hInstance, int nCmdShow);

