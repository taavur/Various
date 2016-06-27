/*

 AUTHOR:		Taavi Kappak

 TASK:			Create an animated button in a child window (write your own).

 DATE:			17.07.2014

*/


#include <iostream>
#include <Windows.h>
#include <tchar.h>

extern HWND hwndRecv;
bool bMessageSent = false;

LRESULT CALLBACK parentWindowProc(HWND hWndPar, UINT message, WPARAM wParam, LPARAM lParam) {
	
	static RECT rectangular;; 
	
	// Messages handler
	switch (message) {
	case WM_PAINT:
		PAINTSTRUCT paintStr;
		HDC hdc;

		hdc = BeginPaint(hWndPar, &paintStr);
		GetClientRect(hWndPar, &rectangular);

		HGDIOBJ currBrush;
		currBrush = SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
		Rectangle(hdc, rectangular.left, rectangular.top, rectangular.right, rectangular.bottom);
		DeleteObject(SelectObject(hdc, currBrush));

		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, "\nWell hello there! Push this button please.\nJust for no reason.\nJust do it.\nPlease.", -1, &rectangular, DT_NOFULLWIDTHCHARBREAK | DT_CENTER | DT_VCENTER);
		
		
		EndPaint(hWndPar, &paintStr);
		return 0;

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWndPar, message, wParam, lParam);
		break;
	}

	return 0;

}
LRESULT CALLBACK childWindowProc (HWND hWndChild, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT BtnRect = {140, 195, 232, 260};
	static PAINTSTRUCT paintStr;
	static HDC hdc;
	switch (message) {
		case WM_USER + 666:
			InvalidateRect(hWndChild, NULL, TRUE);
			return 0;

		case WM_PAINT:
			hdc = BeginPaint(hWndChild, &paintStr);
			GetClientRect(hWndChild, &BtnRect);

			HGDIOBJ currBrush;
			currBrush = SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
			Rectangle(hdc, BtnRect.left, BtnRect.top, BtnRect.right, BtnRect.bottom);
			DeleteObject(SelectObject(hdc, currBrush));
			HGDIOBJ currPen;
			currPen = SelectObject(hdc, CreatePen(PS_SOLID, 0, RGB(255, 255, 255)));
			MoveToEx(hdc, BtnRect.left, BtnRect.bottom, NULL);
			LineTo(hdc, BtnRect.left, BtnRect.top);
			LineTo(hdc, BtnRect.right, BtnRect.top);
			DeleteObject(SelectObject(hdc, CreatePen(PS_SOLID, 0, RGB(0, 0, 0))));
			LineTo(hdc, BtnRect.right, BtnRect.bottom);
			LineTo(hdc, BtnRect.left, BtnRect.bottom);
			DeleteObject(SelectObject(hdc, currPen));
			SetBkMode(hdc, TRANSPARENT);
			DrawText(hdc, "Click me! Please!", -1, &BtnRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hWndChild, &paintStr);

			return 0;

		case WM_LBUTTONDOWN:
			InvalidateRect(hWndChild, NULL, TRUE);
			hdc = BeginPaint(hWndChild, &paintStr);

			currBrush = SelectObject(hdc, GetStockObject(RGB(190, 190, 190)));
			Rectangle(hdc, BtnRect.left, BtnRect.top, BtnRect.right, BtnRect.bottom);
			DeleteObject(SelectObject(hdc, currBrush));

			SetBkMode(hdc, TRANSPARENT);
			DrawText(hdc, "Good job! Take a cookie", -1, &BtnRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hWndChild, &paintStr);

			return 0;

		case WM_LBUTTONUP:
			InvalidateRect(hWndChild, NULL, TRUE);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(hWndChild, message, wParam, lParam);
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	/* Creating window class for parent window */
	WNDCLASSEX windowParentClass;
	windowParentClass.cbSize			= sizeof(WNDCLASSEX);
	windowParentClass.style				= CS_HREDRAW | CS_VREDRAW;
	windowParentClass.lpfnWndProc		= parentWindowProc;
	windowParentClass.cbClsExtra		= 0;
	windowParentClass.cbWndExtra		= 0;
	windowParentClass.hInstance			= hInstance;
	windowParentClass.hIcon				= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	windowParentClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	windowParentClass.hbrBackground		= (HBRUSH)(COLOR_WINDOW);
	windowParentClass.lpszMenuName		= NULL;
	windowParentClass.lpszClassName		= "parentWindowClass";
	windowParentClass.hIconSm			= LoadIcon(windowParentClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	/* Creating window class for child window */
	WNDCLASSEX windowChildClass;
	windowChildClass.cbSize				= sizeof(WNDCLASSEX);
	windowChildClass.style				= CS_HREDRAW | CS_VREDRAW;
	windowChildClass.lpfnWndProc		= childWindowProc;
	windowChildClass.cbClsExtra			= 0;
	windowChildClass.cbWndExtra			= 0;
	windowChildClass.hInstance			= hInstance;
	windowChildClass.hIcon				= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	windowChildClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	windowChildClass.hbrBackground		= (HBRUSH)(COLOR_WINDOW);
	windowChildClass.lpszMenuName		= NULL;
	windowChildClass.lpszClassName		= "childWindowClass";
	windowChildClass.hIconSm			= LoadIcon(windowChildClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	/* Registering window classes with RegisterClassEx */
	RegisterClassEx(&windowParentClass);
	RegisterClassEx(&windowChildClass);

	/* Creating windows */
	HWND hParentWindow = CreateWindow (
		"parentWindowClass",			// WNDCLASS name
		"This is parent window",		// Window title
		WS_OVERLAPPEDWINDOW,					// Window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// Horizontal and vertical position
		400, 300,						// Window size
		HWND_DESKTOP,					// Handle of parent window
		NULL,							// Menu handle
		hInstance,						// Application's instance handle
		NULL							// Window creation data
	);

	HWND hWndButton = CreateWindow (
		"childWindowClass",				// WNDCLASS name
		"Push the button",				// Window title
		WS_CHILD | WS_BORDER,			// Window size
		90, 170,						// Horizontal and vertical position
		210, 50,						// Window size
		hParentWindow,					// Handle of parent window
		NULL,							// Menu handle
		hInstance,						// Application's instance handle
		NULL							// Window creation data
	);

	// Display window
	ShowWindow(hParentWindow, nCmdShow);
	UpdateWindow(hParentWindow);
	ShowWindow(hWndButton, nCmdShow);
	UpdateWindow(hWndButton);

	//Loop to listsen operating system messages
	MSG message;
	while(GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return (int) message.wParam;
}