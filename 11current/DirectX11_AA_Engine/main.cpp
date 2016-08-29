/*
	File Created by: Wei
	Contributor: George Wulfers

	main.cpp
*/

#include <memory>
#include "constants.h"
#include "GameEngineManager.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//Windows Callback Procedure
bool CreateMainWindow(HWND& hwnd, HINSTANCE hInstance, int cmdShow);
HWND hwnd = NULL; //handle window, it is a pointer
WINDOWPLACEMENT globalWindowPos = { sizeof(globalWindowPos) };
b32 isRunning = false;
ConfigFile *config = 0;	//We are using our Global config variable here that remembers the widnow width and height

// Structure used to read in and write out data.
struct FileResult
{
	void* data;
	u32 fileSize;
};

FileResult file;

void FreeMemory(void* data)
{
	if (data)
	{
		VirtualFree(data, 0, MEM_RELEASE);
		data = 0;
	}
}

void* AllocMemory(u32 size)
{
	return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

FileResult ReadEntireFile(i8* filename)
{
	FileResult result = {};

	HANDLE filehandle = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

	if (filehandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER filesize;
		
		if (GetFileSizeEx(filehandle, &filesize))
		{
			u32 size = filesize.QuadPart;
			result.data = AllocMemory(size);

			if (result.data)
			{
				DWORD bytesRead = 0;

				if (ReadFile(filehandle, result.data, size, &bytesRead, 0) && size == bytesRead)
				{
					result.fileSize = bytesRead;
				}
			}
		}

		CloseHandle(filehandle);
	}

	return result;
}

b32 WriteEntireFile(i8* filename, FileResult fileResult)
{
	HANDLE filehandle = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

	if (filehandle != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWritten = 0;
		WriteFile(filehandle, fileResult.data, fileResult.fileSize, &bytesWritten, 0);
		CloseHandle(filehandle);
	}
	else
	{
		// LOGGING
		return false;
	}

	return true;
}

// Thank you Raymond Chen!!
void Win32FullscreenToggle(HWND window)
{
	DWORD style = GetWindowLong(window, GWL_STYLE);
	if (style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO monitorInfo = { sizeof(monitorInfo) };
		if (GetWindowPlacement(window, &globalWindowPos) &&
			GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
		{//NOT BITWISE OPERATOR
			SetWindowLong(window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(window, HWND_TOP,
				monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLong(window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(window, &globalWindowPos);
		SetWindowPos(window, NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);//nothing special for this function
	UNREFERENCED_PARAMETER(cmdLine);//nothing special for this function
	
	file = ReadEntireFile("Config.cfg");

	if (!file.data)
	{
		if (MessageBox(0, "You dont seem to have your Config.cfg file\nWould you like to create it?\n", "CFG Not Found!", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
		{
			config = (ConfigFile*)AllocMemory(sizeof(ConfigFile));
			config->name[0] = 'C';
			config->name[1] = 'F';
			config->name[2] = 'G';
			config->name[3] = 0;
			config->windowHeight = GAME_HEIGHT;
			config->windowWidth = GAME_WIDTH;
			config->graphicsLib = GL_DX11;

			file.data = config;
			file.fileSize = sizeof(ConfigFile);

			WriteEntireFile("Config.cfg", file);

			if (!CreateMainWindow(hwnd, hInstance, cmdShow))
			{
				FreeMemory(config);
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		config = (ConfigFile*)(file.data);
		
		if (config->name[0] == 'C' && config->name[1] == 'F' && config->name[2] == 'G')
		{
			//Do stuff with config file
			if (!CreateMainWindow(hwnd, hInstance, cmdShow))
			{
				FreeMemory(config);
				return 0;
			}

		}
	}

	/*/=====================================================================================
	//Demo initialze
	//=====================================================================================*/
	std::auto_ptr<GameEngineManager> gameEngineMananger(new GameEngineManager()); // need #include <memory>

	if (!gameEngineMananger->Init(hInstance, hwnd))	//bool result was pointless so I made it better.
	{
		DestroyWindow(hwnd);
		return -2;
	}

	MSG msg = { 0 };

	isRunning = true;

	while (isRunning)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_CLOSE:
			{
				isRunning = false;
			}break;

			case WM_SYSKEYUP:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_KEYDOWN:
			{
				u32 keyCode = (u32)msg.wParam; // wParam holds keycodes in WINAPI Key messages
				b32 isDown = (msg.lParam & (1 << 31)) == 0;
				b32 wasDown = (msg.lParam & (1 << 30)) != 0;
				b32 isAltDown = (msg.lParam & (1 << 29)) != 0;

				if (isDown != wasDown)
				{
					//Handle keys here!!
					if (keyCode == VK_ESCAPE && isDown)
					{
						i32 btn = MessageBox(msg.hwnd, "Are you sure you want to exit?\n", "WARNING!", MB_OKCANCEL);
						
						switch (btn)
						{
						case IDOK:
							OutputDebugString("OK PRESSED!\n");
							isRunning = false;
							PostQuitMessage(0);
							break;

						case IDCANCEL:
							OutputDebugString("CANCEL PRESSED!\n");
							break;
						}
					}

					if (isDown)
					{
						if (keyCode == VK_RETURN && isAltDown)
						{
							FULLSCREEN = !FULLSCREEN;	// Toggle Fullscreen On and Off
							if (msg.hwnd)
							{
								Win32FullscreenToggle(msg.hwnd);
							}
						}
					}
				}

			}break;

			default:
				TranslateMessage(&msg);  //decode and pass messages on to WinProc //If our application is going to accept character input from the user, it needs to call the TranslateMessage function inside the message loop
				DispatchMessage(&msg); /*to WinProc*/ //The messagesare sent to our WinProc function for processing by the Dispatch Message function.
			}
		}

		if (FULLSCREEN)
		{

		}

		gameEngineMananger->Update();
	}

	// Demo Shutdown
	gameEngineMananger->ReleaseAll();
	
	FreeMemory(config);

	return static_cast<int>(msg.wParam); //casting numerical data, like we are doing in Listing 2.3 by casting the wParam to an integer.
}

bool CreateMainWindow(HWND& hwnd, HINSTANCE hInstance,int cmdShow)
{
	//=====================================================================================
	//Set up the property for wndClass.
	//=====================================================================================
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);				 //The size of the structure of wndClass in bytes.
	wndClass.style = CS_DBLCLKS | CS_OWNDC | 
					 CS_HREDRAW | CS_VREDRAW;			 //Style flags used to define the window’s look
	wndClass.lpfnWndProc = WndProc;						 //call back Function, we will create.
	wndClass.hInstance = hInstance;                      //The application instance that contains the windows procedure(callback) for this window class.
	//wndClass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(ICON_GL));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);   //A handle to the background brush that will be used for	painting the window’s background.
	wndClass.lpszMenuName = NULL;                          //A null-terminated string of the resource name for the	menu. //we dont wanna a windows meanu, so we set to null
	wndClass.lpszClassName = CLASS_NAME;        //A null - terminated string for what you wish to name your window class.The maximum length is 256 characters.
	//wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(ICON_GL));

	//=====================================================================================											  
	//register the window class.
	//=====================================================================================
	if (!RegisterClassEx(&wndClass))
		return -1;

	/*/=====================================================================================
	//to calculate the size required of the window based on our desired dimensions and style
	//=====================================================================================*/
	RECT rc = { 0, 0, config->windowWidth, config->windowHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);//false means no menu
	
	/*/=====================================================================================
	//Create the actual window
	//=====================================================================================*/
		hwnd = CreateWindowA(
		CLASS_NAME, //lpClassName---window class name, Same to the name, we setuped.
		"Blank Win32 Window",  //lpWindowName--the window title bar text.
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,//dwStyle-------window's style flag.
		CW_USEDEFAULT,         //x-------------the Window's horizontal position
		CW_USEDEFAULT,		 //y-------------the Window's vertical position
		rc.right - rc.left,	 //width---------the window's width
		rc.bottom - rc.top,    //height--------the window's height
		NULL,					 //hWndParent----Handle to the parent window’s handle (optional if this new window is a pop - up or child window).
		NULL,					 //hMenu---------Resource handle to the window’s menu
		hInstance,
		NULL);
	/*/=====================================================================================
	//Show the window
	//=====================================================================================*/
	if (!hwnd)
		return -1;

	ShowWindow(hwnd, cmdShow);

	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) //Windows Callback Procedure
{
	LRESULT result = 0;

	switch (message)
	{
	case WM_CREATE:
	{
		// do initialization stuff here
		// return success
	} break;

	case WM_SIZE:
	{
		RECT rect = {};
		GetWindowRect(hwnd, &rect);

		config->windowWidth = rect.right - rect.left;
		config->windowHeight = rect.bottom - rect.top;

		RECT rc = { 0, 0, config->windowWidth, config->windowHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);//false means no menu
		
	}break;

	case WM_PAINT:
	{
		PAINTSTRUCT paintStruct = { 0 };
		HDC hDC = BeginPaint(hwnd, &paintStruct); //Handle to Device Context //hDC=GetDC(hWnd)
		EndPaint(hwnd, &paintStruct);
	}break;

	case WM_CLOSE:
	{
		file.data = config;
		file.fileSize = sizeof(ConfigFile);

		WriteEntireFile("Config.cfg", file);

		isRunning = false;
	}break;

	case WM_DESTROY:
	{
		isRunning = false;
		PostQuitMessage(0);
	}break;

	default:
		result = DefWindowProc(hwnd, message, wParam, lParam);
	}

	return result;
}