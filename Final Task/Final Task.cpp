/*

 AUTHOR:		Taavi Kappak

 TASK:			Using the amctvb.dll library (see attachments) connect to a drive, pull values of one parameter 
				and draw a graph with that values. Application will be a window that allows the user to insert the 
				number of channel and node (used to connect to the drive), the group and index of pulled parameter 
				and the polling time interval. A button allows to start and stop polling values.

 DATE:			23.07.2014

*/

#include <windows.h>
#include "Header.h"

HWND graphArea, startButton, stopButton;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	
	char mainWindowClassName[] = "mainWindowClass";
	char graphAreaClassName[] = "graphAreaClass";
	WNDCLASSEX mainWindowClass = createWindowClass(hInstance, (WNDPROC)mainWindowProc, mainWindowClassName);
	WNDCLASSEX graphAreaClass = createWindowClass(hInstance, (WNDPROC)graphAreaProc, graphAreaClassName);
	
	RegisterClassEx(&mainWindowClass);
	RegisterClassEx(&graphAreaClass);

	// Rectangles for buttons and text fields and for window
	RECT mainWinRect = {100, 100, 1000, 880};
	RECT startButtonRect = {580, 50, 780, 100};
	RECT stopButtonRect = {580, 190, 780, 240};
	RECT channelFieldRect = {230, 50, 530, 80};
	RECT nodeFieldRect = {230, 90, 530, 120};
	RECT groupFieldRect = {230, 130, 530, 160};
	RECT indexFieldRect = {230, 170, 530, 200};
	RECT timeIntervalFieldRect = {230, 210, 530, 240};
	RECT graphAreaRect = {50, 340, 830, 680};

	// Making windows
	HWND mainWin = createWindowFrame(hInstance, HWND_DESKTOP, mainWinRect, mainWindowClassName, "Parameter Reader v0.1");
	startButton = createButtons(hInstance, mainWin, startButtonRect, "BUTTON", "Start");
	stopButton = createButtons(hInstance, mainWin, stopButtonRect, "BUTTON", "Stop");
	HWND channelField = createTextFields(hInstance, mainWin, channelFieldRect, "EDIT", "0");
	HWND nodeField = createTextFields(hInstance, mainWin, nodeFieldRect, "EDIT", "1");
	HWND groupField = createTextFields(hInstance, mainWin, groupFieldRect, "EDIT", "1");
	HWND indexField = createTextFields(hInstance, mainWin, indexFieldRect, "EDIT", "2");
	HWND timeIntervalField = createTextFields(hInstance, mainWin, timeIntervalFieldRect, "EDIT", "100");
	graphArea = createGraph(hInstance, mainWin, graphAreaRect, graphAreaClassName, NULL);

	ShowWindow(mainWin, nCmdShow);
	ShowWindow(startButton, nCmdShow);
	ShowWindow(stopButton, nCmdShow);
	ShowWindow(channelField, nCmdShow);
	ShowWindow(nodeField, nCmdShow);
	ShowWindow(groupField, nCmdShow);
	ShowWindow(indexField, nCmdShow);
	ShowWindow(timeIntervalField, nCmdShow);
	ShowWindow(graphArea, nCmdShow);
	UpdateWindow(mainWin);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return (int)message.wParam;
}

