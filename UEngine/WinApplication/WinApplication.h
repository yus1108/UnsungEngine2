#pragma once
#include <tchar.h>
#include "../Utility/UTime.h"
#include <functional>

namespace UEngine
{
	struct WINDOWS_APPLICATION_DESC
	{
		HINSTANCE HInstance;
		bool HasTitleBar;
		bool InitUTime;
		bool InitWinInput;
		int NCmdShow;
		LPCTSTR TitleName;
		WNDCLASSEXW* Wcex;
		POINT WindowSize;
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

	public:
		void Create(HINSTANCE hInstance);
		void Create(const WINDOWS_APPLICATION_DESC& desc);

		int UpdateLoop(std::function<void()> f = nullptr);
		void Close();

		const HINSTANCE GetInstance() { return appDesc.HInstance; }
		const HWND GetHandler() { return hWnd; }
		const void GetClientSize(LPRECT clientSize) const { GetClientRect(hWnd, clientSize); }
		const POINT GetClientPixelSize() const;

		void SetWindowSize(int x, int y, int width, int height);

	private:
		//define something for Windows (32-bit and 64-bit, this part is common)
		HWND hWnd;
		WINDOWS_APPLICATION_DESC appDesc;
		bool isDefaultDesc;

		BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, LPCTSTR titleName, LPCTSTR windowClassName, POINT windowSize);
		static LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}


