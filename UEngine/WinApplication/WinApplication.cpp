#include <functional>
#include "uengine_winapp.h"
#include "WinApplication.h"

namespace UEngine
{
    WinApplication WinApplication::instance;

    WinApplication::WinApplication()
        : hWnd(NULL)
        , appDesc({ 0 })
        , isDefaultDesc(false)
    {
    }

    void WinApplication::Create(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = nullptr;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = _T("WIN_APPLICATION");
        wcex.hIconSm = nullptr;

        UEngine::WINDOWS_APPLICATION_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        
        desc.HInstance = hInstance;
        desc.HasTitleBar = true;
        desc.InitUTime = true;
        desc.InitWinInput = true;
        desc.TitleName = _T("Window Application");
        desc.NCmdShow = 10;
        desc.WindowSize = { 800, 600 };
        desc.wcex = &wcex;

        Create(desc);
        isDefaultDesc = true;
    }

    void WinApplication::Create(const WINDOWS_APPLICATION_DESC& desc)
    {
        appDesc = desc;
        if (appDesc.wcex->lpfnWndProc == nullptr)
            appDesc.wcex->lpfnWndProc = WndProc;

        RegisterClassEx(desc.wcex);
        // Perform application initialization:
        InitInstance(desc.HInstance, desc.NCmdShow, desc.TitleName, desc.wcex->lpszClassName, desc.WindowSize);
    }

    void WinApplication::Close()
    {
        UnregisterClass(isDefaultDesc ? _T("WIN_APPLICATION") : appDesc.wcex->lpszClassName, appDesc.HInstance);
        PostQuitMessage(0);
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

    void WinApplication::SetWindowSize(int x, int y, int width, int height)
    {
        //ȭ��ũ�⿡ �´� ��Ʈ
        RECT rc;
        rc.left = 0;
        rc.top = 0;
        rc.right = width;
        rc.bottom = height;

        //���� ������ ũ�� ����
        AdjustWindowRect(&rc, appDesc.HasTitleBar ? WS_OVERLAPPEDWINDOW : WS_POPUPWINDOW, appDesc.wcex->lpszMenuName != nullptr);
        //�� RECT ������ ������ ������ ����
        SetWindowPos(hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
            SWP_NOZORDER | SWP_NOMOVE);
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