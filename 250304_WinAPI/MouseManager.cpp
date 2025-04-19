// MouseManager.cpp
#include "MouseManager.h"

HRESULT MouseManager::Init()
{
    mouseDown.reset();
    mouseUp.set();
    wheelDelta = 0;
    return S_OK;
}

void MouseManager::Update()
{
    // 마우스 위치 추적
    GetCursorPos(&mousePos);
    ScreenToClient(g_hWnd, &mousePos); // g_hWnd는 전역 HWND

    //레이어 갱신
    SetLayer();

    // 마우스 상태 갱신
    const int mouseVK[3] = { MOUSE_LEFT ,MOUSE_RIGHT, MOUSE_MIDDLE };
    for (int i = 0; i < 3; ++i)
    {

        if (GetAsyncKeyState(mouseVK[i]) & 0x8000)
        {
            if (!mouseDown[i]) mouseDown[i] = true;
        }
        else
        {
            mouseDown[i] = false;
        }
    }

    wheelDelta = 0;
}

bool MouseManager::IsOnceMouseDown(int button)
{
    if (GetAsyncKeyState(button) & 0x8000)
    {
        int idx = (button == MOUSE_LEFT) ? 0 : (button == MOUSE_RIGHT ? 1 : 2);
        if (!mouseDown[idx])
        {
            mouseDown[idx] = true;
            return true;
        }
    }
    return false;
}

bool MouseManager::IsOnceMouseUp(int button)
{
    int idx = (button == MOUSE_LEFT) ? 0 : (button == MOUSE_RIGHT ? 1 : 2);
    if (!(GetAsyncKeyState(button) & 0x8000))
    {
        if (!mouseUp[idx])
        {
            mouseUp[idx] = true;
            return true;
        }
    }
    else
    {
        mouseUp[idx] = false;
    }
    return false;
}

bool MouseManager::IsOnceMouseDown(int button, Layer thisLayer)
{
    if (thisLayer == currLayer) {
        if (GetAsyncKeyState(button) & 0x8000)
        {
            int idx = (button == MOUSE_LEFT) ? 0 : (button == MOUSE_RIGHT ? 1 : 2);
            if (!mouseDown[idx])
            {
                mouseDown[idx] = true;
                return true;
            }
        }
        return false;
    }
    else return false;
    
}

bool MouseManager::IsStayMouseDown(int button, Layer thisLayer)
{
    if (thisLayer == currLayer) {
        return (GetAsyncKeyState(button) & 0x8000) != 0;
    }
    else return false;
}

bool MouseManager::IsOnceMouseUp(int button, Layer thisLayer)
{
    if (thisLayer == currLayer) {
        int idx = (button == MOUSE_LEFT) ? 0 : (button == MOUSE_RIGHT ? 1 : 2);
        if (!(GetAsyncKeyState(button) & 0x8000))
        {
            if (!mouseUp[idx])
            {
                mouseUp[idx] = true;
                return true;
            }
        }
        else
        {
            mouseUp[idx] = false;
        }
        return false;
    }
    else return false;
}

void MouseManager::SetLayer()
{
    //// 조건에 따라 레이어를 선택
}

bool MouseManager::IsStayMouseDown(int button)
{
    return (GetAsyncKeyState(button) & 0x8000) != 0;
}

void MouseManager::Release()
{
    ReleaseInstance();
}
