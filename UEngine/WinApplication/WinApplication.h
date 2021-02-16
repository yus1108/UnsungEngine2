#pragma once
#include <tchar.h>

namespace UEngine
{
	struct WINDOWS_APPLICATION_DESC
	{
		HINSTANCE HInstance;
		HACCEL HAccelTable;
		LPCTSTR TitleName;
		LPCTSTR WindowClassName;            // the main window class name
		HICON Icon;
		HICON SmallIcon;
		HCURSOR Cursor;
		int NCmdShow;
		bool Resizable;
		POINT WindowSize;
		LRESULT(*WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	class WinApplication final
	{
#pragma region Singleton
	public:
		static WinApplication* Get() { return &instance; }

	private:
		WinApplication();
		~WinApplication() { instance.Close(); }
		static WinApplication instance;
#pragma endregion
	private:
		LRESULT(*customWndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		void Create(const WINDOWS_APPLICATION_DESC& desc);
		void Create
		(
			HINSTANCE hInstance, 
			int nCmdShow, 
			POINT windowSize = { 800, 600 },
			bool resizable = true, 
			const LPCTSTR titleName = _T("Windows Application"),
			const LPCTSTR className = _T("WIN_APPLICATION"),
			LRESULT(*WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = nullptr
		);

		template<typename Lambda>
		int UpdateLoop(Lambda func = []() {});
		void Close();

		const HINSTANCE GetInstance() { return hInstance; }
		const HWND GetHandler() { return hWnd; }
		const void GetClientSize(LPRECT clientSize) const { GetClientRect(hWnd, clientSize); }

	private:
		//define something for Windows (32-bit and 64-bit, this part is common)
		HINSTANCE hInstance;                                // current instance
		HWND hWnd;
		HACCEL hAccelTable;
		LPCWSTR className;

		// Forward declarations of functions included in this code module:
		
		ATOM MyRegisterClass(HINSTANCE hInstance, HICON icon, HICON smallIcon, HCURSOR cursor, LPCTSTR windowClassName);
		BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, LPCTSTR titleName, LPCTSTR windowClassName, POINT windowSize, bool resizable);
		void SetWindowSize(int x, int y, int width, int height);
		static LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	template<typename Lambda>
	inline int WinApplication::UpdateLoop(Lambda func)
	{
		MSG message;
		ZeroMemory(&message, sizeof(MSG));

		while (true)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
				{
					break;
				}
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				func();
			}
		}

		return message.wParam;
	}
}


