/*
	Author:			Taavi Kappak
	
	Description:	Header.cpp file to match Header.h file in Final Task in ABB Trainee course in 2014.

	Date:			23.07.2014
*/

#include "Header.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#define BUFFER 1000

extern HWND graphArea, startButton, stopButton;
HMODULE hLib = LoadLibrary("amctvb.dll");
unsigned int (__stdcall *InitComm)(unsigned short&);
unsigned int (__stdcall *StopComm)();
unsigned int (__stdcall *ReadPara)(unsigned short, unsigned short, unsigned short, unsigned short, float&);
USHORT channel, node, group, index, interval, prevInterval[BUFFER] = {0};
static int buttonCount = 10, editBoxCount = 1, i = 0, j, x;
float value[BUFFER];
double SCALING = 5;
bool isStarted = false, initDraw = false;
POINT prevPosition;

//runtime linking, getprocadr, casting vpoid pointer
LRESULT CALLBACK mainWindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
		case WM_CREATE:
			return 0 ;

		case WM_PAINT:
			PAINTSTRUCT paintStr;
			HDC hdc;
			
			hdc = BeginPaint(hWnd, &paintStr);
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 100, 53, "Channel", sizeof("Channel"));
			TextOut(hdc, 100, 93, "Node", sizeof("Node"));
			TextOut(hdc, 100, 133, "Group", sizeof("Group"));
			TextOut(hdc, 100, 173, "Index", sizeof("Index"));
			TextOut(hdc, 100, 213, "Time Interval", sizeof("Time Interval"));

			

			EndPaint(hWnd, &paintStr);
			return 0;

		case WM_COMMAND:			//Command from Child windows and menus are under this message
			switch(wParam) {
				case 10:
					channel = GetDlgItemInt(hWnd, 1, NULL, false);
					node = GetDlgItemInt(hWnd, 2, NULL, false);
					group = GetDlgItemInt(hWnd, 3, NULL, false);
					index = GetDlgItemInt(hWnd, 4, NULL, false);
					interval = GetDlgItemInt(hWnd, 5, NULL, false);
					SendMessage(graphArea, WM_USER + 5727, wParam, lParam);
					break;
				case 11:
					SendMessage(graphArea, WM_USER + 5709, wParam, lParam);
					break;
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK graphAreaProc (HWND hGrph, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message) {
		case WM_CREATE:
			InitComm = (unsigned int (__stdcall *)(unsigned short &))GetProcAddress(hLib, "InitCommunication");
			ReadPara = (unsigned int (__stdcall *)(unsigned short, unsigned short, unsigned short, unsigned short, float&))GetProcAddress(hLib, "ParamReadReal");
			StopComm = (unsigned int (__stdcall *)())GetProcAddress(hLib, "StopCommunication");
			EnableWindow(stopButton, false);
			for (j = 0; j < BUFFER; j++) {value[j] = NULL;}
			return 0 ;

		case WM_PAINT:
			PAINTSTRUCT paintStr;
			HDC hdc;
			RECT recta;
			hdc = BeginPaint(hGrph, &paintStr);
			GetClientRect(hGrph, &recta);
			HGDIOBJ currBrush;
			currBrush = SelectObject(hdc, CreateSolidBrush(RGB(128, 128, 128)));
			Rectangle(hdc, recta.left-1, recta.top-1, recta.right+1, recta.bottom+1);
			DeleteObject(SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 0))));
			Rectangle(hdc, recta.left + 1, recta.top + 1, recta.right - 1, recta.bottom - 1);
			DeleteObject(SelectObject(hdc, currBrush));

			//Scaling options
			if (index == 2) { SCALING = 5;}			//Speed
			if (index == 3) { SCALING = 0.25;}		//Frequency
			if (index == 4) { SCALING = 0.007;}		//Current
			if (index == 5) { SCALING = 0.2;}		//Torque
			
			HGDIOBJ currPen;
			currPen = SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0)));
			MoveToEx(hdc, recta.right + 1, recta.top + int(value[i]/SCALING), (LPPOINT)NULL);

			x = 0;
			for (j = i - 1; j >= 0; j--) {
				if(value[j] != NULL) {
					if((recta.bottom-(value[j]/SCALING)) < 400 && (recta.bottom-(value[j]/SCALING)) > 0) {
						LineTo(hdc, recta.right - x, recta.bottom - (int(value[j]/SCALING)));
					}
				}
				x += 1;
			}

			DeleteObject(SelectObject(hdc, currPen));
			EndPaint(hGrph, &paintStr);
			return 0;

		case WM_TIMER:
			ReadPara(channel, node, group, index, value[i]);
			i++;
			if (i > BUFFER) {
				for (j = 1; j <= BUFFER; j++) {
					value[j - 1] = value [j];
				}
				i = 0;
			}
			InvalidateRect(hGrph, NULL, false);
			return 0;

		//Start button
		case WM_USER + 5727:
			USHORT temp;
			InvalidateRect(hGrph, NULL, false);
			SetTimer(hGrph, 10, interval, NULL);
			if (isStarted == false) {
				InitComm(temp);
				EnableWindow(startButton, false);
				EnableWindow(stopButton, true);
			}
			isStarted = true;
			return 0;

		//Stop button
		case WM_USER + 5709:
			KillTimer(hGrph, 10);
			if (isStarted == true) {
				StopComm();
				isStarted = false;
				EnableWindow(startButton, true);
				EnableWindow(stopButton, false);
			}
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(hGrph, message, wParam, lParam);
	}
}
WNDCLASSEX createWindowClass (HINSTANCE hInstance, WNDPROC windowProc, char className[]) {
	WNDCLASSEX temp;																	// Structure to hold window's attributes
	temp.cbSize			= sizeof(WNDCLASSEX);											// The size of the structure
    temp.style			= CS_HREDRAW | CS_VREDRAW;										// The class style(s)
    temp.lpfnWndProc	= windowProc;													// Window procedure address
    temp.cbClsExtra		= 0;															// Class extra bytes
    temp.cbWndExtra		= 0;															// Window extra bytes
    temp.hInstance		= hInstance;													// Instance handle
    temp.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));		// Icon handle
    temp.hCursor		= LoadCursor(NULL, IDC_ARROW);									// Cursor handle
    temp.hbrBackground	= (HBRUSH)(COLOR_WINDOW);										// Background color
    temp.lpszMenuName	= NULL;															// Menu name
	temp.lpszClassName	= className;													// WNDCLASS name
    temp.hIconSm		= LoadIcon(temp.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));	// Small icon handle
	return temp;
}
HWND createWindowFrame (HINSTANCE hInstance, HWND hParent, RECT rectangular, char className[], char title[]) {
	HWND hWnd = CreateWindowEx (
		NULL,										// Extended Window style
		className,									// WNDCLASS name
		title,										// Window title
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	// Window style
		rectangular.left, rectangular.top,			// Horizontal position and Vertical position
		rectangular.right - rectangular.left,		// Initial width
		rectangular.bottom - rectangular.top,		// Initial height
		hParent,									// Handle of parent window
		NULL,										// Menu handle
		hInstance,									// Application's instance handle
		NULL										// Window-creation data
	);
	return hWnd;
}
HWND createButtons(HINSTANCE hInstance, HWND hParent, RECT rectangular, char className[], char title[]) {
	HWND button = CreateWindowEx (
		NULL,										// Extended Window style
		className,									// WNDCLASS name
		title,										// Window title
		WS_CHILD | WS_BORDER,						// Button styles
		rectangular.left, rectangular.top,			// Horizontal position and Vertical position
		rectangular.right - rectangular.left,		// Initial width
		rectangular.bottom - rectangular.top,		// Initial height
		hParent,									// Handle of parent window
		(HMENU) buttonCount,						// Menu handle
		hInstance,									// Application's instance handle
		NULL										// Window-creation data
	);
	buttonCount++;
	return button;
}
HWND createTextFields(HINSTANCE hInstance, HWND hParent, RECT rectangular, char className[], char title[]) {
	HWND textField = CreateWindowEx (
		WS_EX_CLIENTEDGE,							// Extended Window style
		className,									// WNDCLASS name
		title,										// Window title
		WS_CHILD | WS_BORDER | WS_VISIBLE |			// EditBox styles
		ES_LEFT | ES_AUTOHSCROLL,
		rectangular.left, rectangular.top,			// Horizontal position and Vertical position
		rectangular.right - rectangular.left,		// Initial width
		rectangular.bottom - rectangular.top,		// Initial height
		hParent,									// Handle of parent window
		(HMENU)editBoxCount,						// Menu handle
		hInstance,									// Application's instance handle
		NULL										// Window-creation data
	);
	editBoxCount++;
	return textField;
}
HWND createGraph (HINSTANCE hInstance, HWND hParent, RECT rectangular, char className[], char title[]) {
	HWND hWnd = CreateWindowEx (
		NULL,										// Extended Window style
		className,									// WNDCLASS name
		title,										// Window title
		WS_CHILD | WS_HSCROLL | ES_AUTOHSCROLL,		// Window style
		rectangular.left, rectangular.top,			// Horizontal position and Vertical position
		rectangular.right - rectangular.left,		// Initial width
		rectangular.bottom - rectangular.top,		// Initial height
		hParent,									// Handle of parent window
		NULL,										// Menu handle
		hInstance,									// Application's instance handle
		NULL										// Window-creation data
	);
	return hWnd;
}