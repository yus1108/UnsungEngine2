#pragma once
#include <tchar.h>
#include <functional>
#include "../Utility/UTime.h"
#include "../Utility/UThreadPool.h"

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
#pragma data_seg(".ioshare")
		static WinApplication instance;
#pragma data_seg()

#pragma endregion

	public:
		Utility::Sync::UThreadPool threadPool;

		void Create(HINSTANCE hInstance, size_t numThreads = 0);
		void Create(const WINDOWS_APPLICATION_DESC& desc, size_t numThreads = 0);

		int UpdateLoop(std::function<void()> f = nullptr);
		void Close();

		const HINSTANCE GetInstance() { return appDesc.HInstance; }
		const WINDOWS_APPLICATION_DESC GetDesc() { return appDesc; }
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


