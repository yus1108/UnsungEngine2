#include <functional>
#include "uengine_winapp.h"
#include "WinApplication.h"

namespace UEngine
{
    WinApplication* WinApplication::instance = nullptr;

    WinApplication::WinApplication()
        : hWnd(NULL)
        , appDesc({ 0 })
        , isDefaultDesc(false)
        , hDLL(NULL)
    {
    }

    void WinApplication::Create(HINSTANCE hInstance, size_t numThreads)
    {
        WNDCLASSEXW Wcex;
        Wcex.cbSize = sizeof(WNDCLASSEX);
        Wcex.style = CS_HREDRAW | CS_VREDRAW;
        Wcex.lpfnWndProc = WndProc;
        Wcex.cbClsExtra = 0;
        Wcex.cbWndExtra = 0;
        Wcex.hInstance = hInstance;
        Wcex.hIcon = nullptr;
        Wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        Wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        Wcex.lpszMenuName = nullptr;
        Wcex.lpszClassName = _T("WIN_APPLICATION");
        Wcex.hIconSm = nullptr;

        UEngine::WINDOWS_APPLICATION_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        
        desc.HInstance = hInstance;
        desc.HasTitleBar = true;
        desc.InitUTime = true;
        desc.InitWinInput = true;
        desc.TitleName = _T("Window Application");
        desc.NCmdShow = 10;
        desc.WindowSize = { 800, 600 };
        desc.Wcex = &Wcex;

        Create(desc, numThreads);
        isDefaultDesc = true;
    }

    void WinApplication::Create(const WINDOWS_APPLICATION_DESC& desc, size_t numThreads)
    {
        appDesc = desc;
        if (appDesc.Wcex->lpfnWndProc == nullptr)
            appDesc.Wcex->lpfnWndProc = WndProc;

        RegisterClassEx(desc.Wcex);
        // Perform application initialization:
        InitInstance(desc.HInstance, desc.NCmdShow, desc.TitleName, desc.Wcex->lpszClassName, desc.WindowSize);
        threadPool.Init(numThreads);
    }

    int WinApplication::UpdateLoop(std::function<void()> f)
    {
        MSG message;
        ZeroMemory(&message, sizeof(MSG));

        UEngine::Utility::UTime::Get()->Signal();
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
                if (appDesc.InitUTime)
                    UEngine::Utility::UTime::Get()->Signal();
                if (f) f();
            }
        }

        return message.wParam;
    }

    void WinApplication::Close()
    {
        FreeDLL();
        //UnregisterClass(isDefaultDesc ? _T("WIN_APPLICATION") : appDesc.Wcex->lpszClassName, appDesc.HInstance);
        //PostQuitMessage(0);
    }

    void WinApplication::ReLoadDLL()
    {
        FreeDLL();
        LoadDLL(dllSourceFile);
    }

    void WinApplication::LoadDLL(std::wstring name)
    {
        dllSourceFile = name;
        std::wstring target = name + L".temp.dll";

        try // If you want to avoid exception handling, then use the error code overload of the following functions.
        {
            std::filesystem::copy_file(dllSourceFile, target, std::filesystem::copy_options::overwrite_existing);
        }
        catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
        {
            throw std::runtime_error(e.what());
        }

        hDLL = LoadLibrary(target.c_str());
        if (hDLL == NULL) throw std::runtime_error("dll not found");
    }

    FARPROC WinApplication::FindFunction(std::string func_name)
    {
        auto function = GetProcAddress(hDLL, func_name.c_str());
        if (!function)
        {
            // handle the error  
            FreeLibrary(hDLL);
            throw std::runtime_error("dll function not found");
        }
        return function;
    }

    void WinApplication::FreeDLL()
    {
        FreeLibrary(hDLL);
        std::filesystem::path target = dllSourceFile + L".temp.dll";
        try // If you want to avoid exception handling, then use the error code overload of the following functions.
        {
            std::filesystem::remove(target);
        }
        catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
        {
            throw std::runtime_error(e.what());
        }
    }

    BOOL WinApplication::InitInstance(HINSTANCE hInstance, int nCmdShow, LPCTSTR titleName, LPCTSTR windowClassName, POINT windowSize)
    {
        int x = GetSystemMetrics(SM_CXSCREEN) / 2;
        int y = GetSystemMetrics(SM_CYSCREEN) / 2;
        int halfWidth = windowSize.x / 2;
        int halfHeight = windowSize.y / 2;
        hWnd = CreateWindow(windowClassName, titleName, appDesc.HasTitleBar ? WS_OVERLAPPEDWINDOW : WS_POPUPWINDOW,
            x - halfWidth, y - halfHeight, windowSize.x, windowSize.y, nullptr, nullptr, hInstance, nullptr);

        UEngine::WinInput::Get()->AttachHandler(hWnd);

        if (!hWnd)
        {
            return FALSE;
        }

        SetWindowSize(x - halfWidth, y - halfHeight, windowSize.x, windowSize.y);
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

    const POINT WinApplication::GetClientPixelSize() const
    {
        RECT clientSize;
        GetClientSize(&clientSize);
        return POINT{ clientSize.right - clientSize.left, clientSize.bottom - clientSize.top };
    }

    void WinApplication::SetWindowSize(int x, int y, int width, int height)
    {
        //화면크기에 맞는 렉트
        RECT rc;
        rc.left = 0;
        rc.top = 0;
        rc.right = width;
        rc.bottom = height;

        //실제 윈도우 크기 조정
        AdjustWindowRect(&rc, appDesc.HasTitleBar ? WS_OVERLAPPEDWINDOW : WS_POPUPWINDOW, appDesc.Wcex->lpszMenuName != nullptr);
        //위 RECT 정보로 윈도우 사이즈 세팅
        SetWindowPos(hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
            SWP_NOZORDER | SWP_NOMOVE);
    }

    void WinApplication::ConfineCursor()
    {
        RECT r;
        GetWindowRect(hWnd, &r);
        ClipCursor(&r);
    }

    LRESULT CALLBACK WinApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_DESTROY: case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
}
