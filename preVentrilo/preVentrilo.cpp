// preVentrilo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "preVentrilo.h"
#include "defs.h"
#include "LimitSingleInstance.h"

#define MAX_LOADSTRING 100
#define WM_TRAYNOTICE WM_USER + 5

// The one and only CLimitSingleInstance object.
CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{A8988F63-6E3C-40ba-8E0B-B68553B0BABF}"));

// Function typedefs:
typedef void (*LPSetVentHook)(DWORD threadID, HWND hWndXmit);
typedef bool (*LPSetMuteType)(int type, bool bAdjustVolume, int nAdjustedVol);
typedef void (*LPdwhwnd)(int arg);

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HMENU hMenu;									// the menu
NOTIFYICONDATA nSysTrayData;					// data for the system tray icon
HINSTANCE hinstDLL;								// HINSTANCE of prevent.dll
LPSetVentHook pSetVentHook;						// Pointer to SetVentHook function in prevent.dll
FARPROC pKillVentHook;							// Pointer to KillVentHook function in prevent.dll
LPdwhwnd pSetMediaPlayer;						// Pointer to SetMediaPlayer function in prevent.dll
LPSetMuteType pSetMuteType;						// Pointer to SetMuteType function in prevent.dll
DWORD threadID;									// threadID of the main Ventrilo thread
HWND hWndVent;									// HWND of Ventrilo
HWND hWndXmit;									// HWND to --XMIT-- static control
HANDLE hThreadVent;								// thread to continuously monitor that vent is running
DWORD dwThreadVentId;							// thread ID of the ventrilo monitoring thread
struct											// struct to contain info about the state of the settings
{
	int pauseType;
	int mediaPlayer;
	std::string ventPath;
	bool bAdjustVolume;
	int nAdjustedVolume;
} settings;
std::string strSettingsFile = "settings.v";

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
bool				InitializepreVentrilo();
void				SaveSettings();
void				CreateAndShowTrayIcon(HWND hWnd, HINSTANCE hInst);
void				ShowMenu(HWND hWnd);
void				RemoveTrayIcon();
void				OnPauseIfISpeak();
void				OnPauseIfOthersSpeak();
void				OnMediaPlayer();
void				DoCleanup();
DWORD WINAPI		ThreadVentProc(LPVOID lpParam);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    if(g_SingleInstanceObj.IsAnotherInstanceRunning())
		return FALSE;

 	// Load up the DLL, start vent, set the hook, etc.
	if(!InitializepreVentrilo())
		return FALSE;

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PREVENTRILO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PREVENTRILO));

	// Load the menu
	hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_PREVENTRILO));
	hMenu = GetSubMenu(hMenu, 0);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PREVENTRILO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_PREVENTRILO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 200, 100, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, 0/*nCmdShow*/);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		CreateAndShowTrayIcon(hWnd, ((LPCREATESTRUCT) lParam)->hInstance);
		break;
	case WM_TRAYNOTICE:
		if(wParam == IDC_PREVENTRILO)
		{
			if(lParam == WM_RBUTTONUP)
			{
				ShowMenu(hWnd);
			}
			else if(lParam == WM_LBUTTONDBLCLK)
			{
				SendMessage(hWnd, WM_COMMAND, IDM_ABOUT, 0);
			}
		}
		else
			DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_TRAY_PAUSEIFISPEAK:
			OnPauseIfISpeak();
			break;
		case ID_TRAY_PAUSEIFOTHERSSPEAK:
			OnPauseIfOthersSpeak();
			break;
		case ID_TRAY_ITUNES:
            settings.bAdjustVolume = false;
            settings.mediaPlayer = MEDIA_PLAYER_ITUNES;
			OnMediaPlayer();
			break;
		case ID_WINAMP_PAUSE:
			settings.bAdjustVolume = false;
            settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
			OnMediaPlayer();
			break;
		case ID_WINAMP_0:
			settings.bAdjustVolume = true;
			settings.nAdjustedVolume = 0;
            settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
			OnMediaPlayer();
			break;
		case ID_WINAMP_5:
			settings.bAdjustVolume = true;
			settings.nAdjustedVolume = 15;
            settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
			OnMediaPlayer();
			break;
		case ID_WINAMP_10:
			settings.bAdjustVolume = true;
			settings.nAdjustedVolume = 29;
            settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
			OnMediaPlayer();
			break;
		case ID_WINAMP_20:
			settings.bAdjustVolume = true;
			settings.nAdjustedVolume = 54;
            settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
			OnMediaPlayer();
			break;
		case ID_WINAMP_30:
			settings.bAdjustVolume = true;
			settings.nAdjustedVolume = 78;
            settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
			OnMediaPlayer();
			break;
		case ID_WINAMP_40:
			settings.bAdjustVolume = true;
			settings.nAdjustedVolume = 103;
            settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
			OnMediaPlayer();
			break;
		case ID_WINAMP_50:
			settings.bAdjustVolume = true;
			settings.nAdjustedVolume = 130;
            settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
			OnMediaPlayer();
			break;
		case ID_TRAY_WINDOWSMEDIAPLAYER:
            settings.bAdjustVolume = false;
            settings.mediaPlayer = MEDIA_PLAYER_WMP;
			OnMediaPlayer();
			break;
		case ID_TRAY_FOOBAR2000:
            settings.bAdjustVolume = false;
            settings.mediaPlayer = MEDIA_PLAYER_FOOBAR;
			OnMediaPlayer();
			break;
		case ID_TRAY_RHAPSODY:
            settings.bAdjustVolume = false;
            settings.mediaPlayer = MEDIA_PLAYER_RHAPSODY;
			OnMediaPlayer();
			break;
		case ID_TRAY_SPOTIFY:
            settings.bAdjustVolume = false;
            settings.mediaPlayer = MEDIA_PLAYER_SPOTIFY;
			OnMediaPlayer();
			break;
		case IDM_ABOUT:
			// DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DoCleanup();
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// do all the important initialization stuffz
bool InitializepreVentrilo()
{
	std::ifstream fin(strSettingsFile.c_str());
	std::string line;
	if(!std::getline(fin, line))
	{
		char *pProg;
		size_t len;
		errno_t err = _dupenv_s( &pProg, &len, "PROGRAMFILES" );
		if(!err)
		{
			settings.ventPath = pProg;
			settings.ventPath += "\\Ventrilo\\Ventrilo.exe";
			free( pProg );
		}
		else
			settings.ventPath = "";
	}
	else
		settings.ventPath = line;
	if(!(fin >> settings.pauseType))
		settings.pauseType = (PAUSE_TYPE_OTHERS | PAUSE_TYPE_SELF);
	if(!(fin >> settings.mediaPlayer))
		settings.mediaPlayer = MEDIA_PLAYER_WINAMP;
	if(!(fin >> settings.bAdjustVolume))
		settings.bAdjustVolume = true;
	if(!(fin >> settings.nAdjustedVolume))
		settings.nAdjustedVolume = 29;
	fin.close();
	SaveSettings();


	hinstDLL = LoadLibrary("prevent.dll");
	if(hinstDLL == NULL)
	{
		MessageBox(NULL, "Unable to load prevent.dll", NULL, NULL);
		return false;
	}


	pSetVentHook = (LPSetVentHook)GetProcAddress(hinstDLL, "SetVentHook");
	pKillVentHook = GetProcAddress(hinstDLL, "KillVentHook");
	pSetMediaPlayer = (LPdwhwnd)GetProcAddress(hinstDLL, "SetMediaPlayer");
	pSetMuteType = (LPSetMuteType)GetProcAddress(hinstDLL, "SetMuteType");
	if(pSetVentHook == NULL || pKillVentHook == NULL || pSetMediaPlayer == NULL || pSetMuteType == NULL)
	{
		MessageBox(NULL, "Unable to load DLL functions", NULL, NULL);
		return false;
	}


	hWndVent = FindWindow( NULL, "Ventrilo");
	if(hWndVent == NULL)
	{
		if((int)ShellExecute(NULL, "open", settings.ventPath.c_str(), "", NULL, SW_SHOWNORMAL) <= 32)
		{
			MessageBox(NULL, "Please select the Ventrilo executable.", NULL, NULL);

			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "Ventrilo.exe\0Ventrilo.exe\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrDefExt = "exe";

			if(GetOpenFileName(&ofn))
			{
				settings.ventPath = szFileName;
				ShellExecute(NULL, "open", settings.ventPath.c_str(), "", NULL, SW_SHOWNORMAL);
			}
		}
	}

	hThreadVent = CreateThread(
				NULL,				// default security attributes
				0,					// use default stack size  
				ThreadVentProc,		// thread function 
				NULL,				// argument to thread function 
				0,					// use default creation flags 
				&dwThreadVentId);	// returns the thread identifier
	if(hThreadVent == NULL)
	{
		MessageBox(NULL, "Error.", NULL, NULL);
		return false;
	}

	OnMediaPlayer();

	return true;
}

void SaveSettings()
{
	std::ofstream fout(strSettingsFile.c_str());
	fout << settings.ventPath << std::endl;
	fout << settings.pauseType << std::endl;
	fout << settings.mediaPlayer << std::endl;
	fout << settings.bAdjustVolume << std::endl;
	fout << settings.nAdjustedVolume;
	fout.close();
}

// creation of tray icon for WM_CREATE
void CreateAndShowTrayIcon(HWND hWnd, HINSTANCE hInst)
{
	nSysTrayData.cbSize = sizeof(NOTIFYICONDATA);
	nSysTrayData.hWnd = hWnd;
	nSysTrayData.uID = IDC_PREVENTRILO;
	nSysTrayData.uFlags = NIF_ICON + NIF_MESSAGE + NIF_TIP;
	nSysTrayData.uCallbackMessage = WM_TRAYNOTICE;
	nSysTrayData.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PREVENTRILO));
	lstrcpy(&nSysTrayData.szTip[0], "preVentrilo");
	Shell_NotifyIcon(NIM_ADD, &nSysTrayData);
}

// Show the menu
void ShowMenu(HWND hWnd)
{
	POINT mouse;

	// set checked/bolded here
	if(settings.pauseType & PAUSE_TYPE_OTHERS)
		CheckMenuItem(hMenu, ID_TRAY_PAUSEIFOTHERSSPEAK, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_TRAY_PAUSEIFOTHERSSPEAK, MF_UNCHECKED);

	if(settings.pauseType & PAUSE_TYPE_SELF)
		CheckMenuItem(hMenu, ID_TRAY_PAUSEIFISPEAK, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_TRAY_PAUSEIFISPEAK, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_FOOBAR)
		CheckMenuItem(hMenu, ID_TRAY_FOOBAR2000, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_TRAY_FOOBAR2000, MF_UNCHECKED);


	if(settings.mediaPlayer == MEDIA_PLAYER_WINAMP && !settings.bAdjustVolume)
		CheckMenuItem(hMenu, ID_WINAMP_PAUSE, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_WINAMP_PAUSE, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_WINAMP && settings.bAdjustVolume && settings.nAdjustedVolume == 0)
		CheckMenuItem(hMenu, ID_WINAMP_0, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_WINAMP_0, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_WINAMP && settings.bAdjustVolume && settings.nAdjustedVolume == 15)
		CheckMenuItem(hMenu, ID_WINAMP_5, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_WINAMP_5, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_WINAMP && settings.bAdjustVolume && settings.nAdjustedVolume == 29)
		CheckMenuItem(hMenu, ID_WINAMP_10, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_WINAMP_10, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_WINAMP && settings.bAdjustVolume && settings.nAdjustedVolume == 54)
		CheckMenuItem(hMenu, ID_WINAMP_20, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_WINAMP_20, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_WINAMP && settings.bAdjustVolume && settings.nAdjustedVolume == 78)
		CheckMenuItem(hMenu, ID_WINAMP_30, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_WINAMP_30, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_WINAMP && settings.bAdjustVolume && settings.nAdjustedVolume == 103)
		CheckMenuItem(hMenu, ID_WINAMP_40, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_WINAMP_40, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_WINAMP && settings.bAdjustVolume && settings.nAdjustedVolume == 130)
		CheckMenuItem(hMenu, ID_WINAMP_50, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_WINAMP_50, MF_UNCHECKED);


	if(settings.mediaPlayer == MEDIA_PLAYER_WMP)
		CheckMenuItem(hMenu, ID_TRAY_WINDOWSMEDIAPLAYER, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_TRAY_WINDOWSMEDIAPLAYER, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_ITUNES)
		CheckMenuItem(hMenu, ID_TRAY_ITUNES, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_TRAY_ITUNES, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_RHAPSODY)
		CheckMenuItem(hMenu, ID_TRAY_RHAPSODY, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_TRAY_RHAPSODY, MF_UNCHECKED);

	if(settings.mediaPlayer == MEDIA_PLAYER_SPOTIFY)
		CheckMenuItem(hMenu, ID_TRAY_SPOTIFY, MF_CHECKED);
	else
		CheckMenuItem(hMenu, ID_TRAY_SPOTIFY, MF_UNCHECKED);

	GetCursorPos(&mouse);
	SetForegroundWindow(hWnd); // so that when you click off the menu it disappears
	TrackPopupMenu(hMenu, TPM_RIGHTALIGN, mouse.x, mouse.y, 0, hWnd, NULL);
}

// Remove the tray icon
void RemoveTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &nSysTrayData);
}

void OnPauseIfISpeak()
{
	settings.pauseType ^= PAUSE_TYPE_SELF;
	pSetMuteType(settings.pauseType, settings.bAdjustVolume,  settings.nAdjustedVolume);
}

void OnPauseIfOthersSpeak()
{
	settings.pauseType ^= PAUSE_TYPE_OTHERS;
	pSetMuteType(settings.pauseType, settings.bAdjustVolume,  settings.nAdjustedVolume);
}

void OnMediaPlayer()
{
    pSetMuteType(settings.pauseType, settings.bAdjustVolume, settings.nAdjustedVolume);
    pSetMediaPlayer(settings.mediaPlayer);
}

void DoCleanup()
{
	pKillVentHook();
	SaveSettings();
	RemoveTrayIcon();
	DestroyMenu(hMenu);
	FreeLibrary(hinstDLL);
	CloseHandle(hThreadVent);
}

DWORD WINAPI ThreadVentProc(LPVOID lpParam)
{
	hWndVent = NULL;
	while(1)
	{
		if(!IsWindow(hWndVent))
		{
			pKillVentHook();
			hWndVent = FindWindow(NULL, "Ventrilo");
			if(hWndVent != NULL)
			{
				threadID = GetWindowThreadProcessId(hWndVent, NULL);
				hWndXmit = FindWindowEx(hWndVent, NULL, "Static", "--XMIT--");
				if(threadID != NULL && hWndXmit != NULL)
					pSetVentHook(threadID, hWndXmit);
			}
		}
		Sleep(2000);
	}
	return 0;
}