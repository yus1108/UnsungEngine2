#include "uengine_winapp.h"
#include "WinInput.h"
#include <iostream>

UEngine::WinInput* UEngine::WinInput::instance = nullptr;

const bool UEngine::WinInput::GetKeyDown(const unsigned& _key)
{
    if (GetAsyncKeyState(_key) & 0x8000)
    {
        //배열에 bool값이 false라면
        if (keys[_key] == false)
        {
            keys[_key] = true;
            return true;
        };
    }
    //현재는 안누르고 있다면
    else
    {
        keys[_key] = false;
    }
    return false;
}

const bool UEngine::WinInput::GetKey(const unsigned& _key)
{
    if (GetAsyncKeyState(_key) & 0x8000)
    {
        PrevKey = _key;
        return true;
    }
    return false;
}

const bool UEngine::WinInput::GetKeyUp(const unsigned& _key)
{
    //누르고있지 않을때
    if (GetKey(_key) == false)
    {
        if (PrevKey == _key)
        {
            PrevKey = NULL;
            return true;
        }
    }
    return false;
}


const UEngine::Utility::Coordinate2D UEngine::WinInput::GetMousePos() const
{
    RECT clientSize, windowSize, rc;
    POINT point;

    GetCursorPos(&point);
    GetClientRect(handler, &clientSize);
    GetWindowRect(handler, &windowSize);

    rc.left = 0;
    rc.top = 0;
    rc.right = clientSize.right;
    rc.bottom = clientSize.bottom;
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

    point.x = point.x - windowSize.left + rc.left;
    point.y = point.y - windowSize.top + rc.top;

    return { static_cast<float>(point.x), static_cast<float>(point.y) };
}