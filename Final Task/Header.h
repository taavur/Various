/*
	Author:			Taavi Kappak
	
	Description:	Header.h file for Final Task in ABB Trainee course in 2014.

	Date:			23.07.2014
*/

#include <Windows.h>

LRESULT CALLBACK mainWindowProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK graphAreaProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ButtonProc (HWND, UINT, WPARAM, LPARAM);
WNDCLASSEX createWindowClass (HINSTANCE, WNDPROC, char[]);
HWND createWindowFrame (HINSTANCE, HWND, RECT, char[], char[]);
HWND createButtons(HINSTANCE, HWND, RECT, char[], char[]);
HWND createTextFields (HINSTANCE, HWND, RECT, char[], char[]);
HWND createGraph (HINSTANCE, HWND, RECT, char[], char[]);