#include <functional>
#include <Windows.h>
#include "WinInput.h"
#include "WinApplication.h"

namespace UEngine
{
    WinApplication WinApplication::instance;
    LRESULT DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    WinApplication::WinApplication()
        : hInstance(nullptr)
        , hWnd(nullptr)
        , hAccelTable(nullptr)
        , className(nullptr)
        , customWndProc(nullptr)
    {
    }

    void WinApplication::Create(const WINDOWS_APPLICATION_DESC& desc)
    {
        className = desc.WindowClassName;
        hAccelTable = desc.HAccelTable;
        customWndProc = desc.WndProc;
        if (desc.WndProc == nullptr)
            customWndProc = DefaultWndProc;

        MyRegisterClass(desc.HInstance, desc.Icon, desc.SmallIcon, desc.Cursor, desc.WindowClassName);
        // Perform application initialization:
        InitInstance(desc.HInstance, desc.NCmdShow, desc.TitleName, desc.WindowClassName, desc.WindowSize, desc.Resizable);
    }

    void WinApplication::Create
    (
        HINSTANCE hInstance, 
        int nCmdShow, 
        POINT windowSize,
        bool resizable, 
        const LPCTSTR titleName, 
        const LPCTSTR className,
        LRESULT(*WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    )
    {
        WINDOWS_APPLICATION_DESC desc;
        ZeroMemory(&desc, sizeof(WINDOWS_APPLICATION_DESC));

        desc.Cursor = LoadCursor(NULL, IDC_ARROW);
        desc.HInstance = hInstance;
        desc.Icon = LoadIcon(NULL, IDI_APPLICATION);
        desc.TitleName = titleName;
        desc.WindowClassName = className;
        desc.NCmdShow = nCmdShow;
        desc.WindowSize = windowSize;
        desc.Resizable = resizable;
        desc.WndProc = WndProc;

        Create(desc);
    }

    void WinApplication::Close()
    {
        UnregisterClass(className, hInstance);
        PostQuitMessage(0);
    }

    ATOM WinApplication::MyRegisterClass(HINSTANCE hInstance, HICON icon, HICON smallIcon, HCURSOR cursor, LPCTSTR windowClassName)
    {
        WNDCLASSEX wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = icon;
        wcex.hCursor = cursor;
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = windowClassName;
        wcex.hIconSm = smallIcon;

        return RegisterClassEx(&wcex);

    }

    BOOL WinApplication::InitInstance(HINSTANCE hInstance, int nCmdShow, LPCTSTR titleName, LPCTSTR windowClassName, POINT windowSize, bool resizable)
    {
        this->hInstance = hInstance; // Store instance handle in our global variable

        int x = GetSystemMetrics(SM_CXSCREEN) / 2;
        int y = GetSystemMetrics(SM_CYSCREEN) / 2;
        int halfWidth = windowSize.x / 2;
        int halfHeight = windowSize.y / 2;
        hWnd = CreateWindow(windowClassName, titleName, WS_OVERLAPPEDWINDOW,
            x - halfWidth, y - halfHeight, windowSize.x, windowSize.y, nullptr, nullptr, hInstance, nullptr);

        if (!hWnd)
        {
            return FALSE;
        }

        SetWindowSize(x - halfWidth, y - halfHeight, windowSize.x, windowSize.y);
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
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
        AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU, false);
        //위 RECT 정보로 윈도우 사이즈 세팅
        SetWindowPos(hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
            SWP_NOZORDER | SWP_NOMOVE);
    }

    LRESULT CALLBACK WinApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (WinApplication::Get()->customWndProc != nullptr)
        {
            auto ok = WinApplication::Get()->customWndProc(hWnd, message, wParam, lParam);
            if (ok) return true;
        }

        //어떤 메세지가 발생되었는가를 통해 처리할 조건문
        if (message == WM_DESTROY || message == WM_CLOSE)
        {
            PostQuitMessage(0);
            return (DefWindowProc(hWnd, message, wParam, lParam));
        }

        return (DefWindowProc(hWnd, message, wParam, lParam));
    }


    LRESULT DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_MOUSEMOVE:
            UEngine::WinInput::Get()->SetMousePos(UEngine::Definition::Coordinate2D{ (float)LOWORD(lParam), (float)HIWORD(lParam) });
            break;
        case WM_KEYDOWN:
            UEngine::WinInput::Get()->SetKeyPress(true, wParam);
            break;
        case WM_KEYUP:
            UEngine::WinInput::Get()->SetKeyPress(false, wParam);
            break;
        case WM_LBUTTONDOWN:
            UEngine::WinInput::Get()->SetMousePress(true, UEngine::WinInput::MouseType::LEFT);
            break;
        case WM_LBUTTONUP:
            UEngine::WinInput::Get()->SetMousePress(false, UEngine::WinInput::MouseType::LEFT);
            break;
        case WM_RBUTTONDOWN:
            UEngine::WinInput::Get()->SetMousePress(true, UEngine::WinInput::MouseType::RIGHT);
            break;
        case WM_RBUTTONUP:
            UEngine::WinInput::Get()->SetMousePress(false, UEngine::WinInput::MouseType::RIGHT);
            break;
        case WM_MOUSEWHEEL:
            UEngine::WinInput::Get()->SetMousePress(true, UEngine::WinInput::MouseType::SCROLL);
            UEngine::WinInput::Get()->scroll = (int)wParam;
            break;
        case WM_TOUCH:
            break;
        case WM_MBUTTONDOWN:
            UEngine::WinInput::Get()->SetMousePress(true, UEngine::WinInput::MouseType::MIDDLE);
            break;
        case WM_MBUTTONUP:
            UEngine::WinInput::Get()->SetMousePress(false, UEngine::WinInput::MouseType::MIDDLE);
            break;
        }

        return (DefWindowProc(hWnd, message, wParam, lParam));
    }

}
