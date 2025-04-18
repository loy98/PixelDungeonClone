// MouseManager.h
#pragma once
#include "Singleton.h"
#include "config.h"

#define MOUSE_LEFT    VK_LBUTTON
#define MOUSE_RIGHT   VK_RBUTTON
#define MOUSE_MIDDLE  VK_MBUTTON

class MouseManager : public Singleton<MouseManager>
{
private:
    bitset<3> mouseDown; // [0]: Left, [1]: Right, [2]: Middle
    bitset<3> mouseUp;

    POINT mousePos;

    short wheelDelta;

public:
    HRESULT Init();
    void Update();
    void Release();

    bool IsOnceMouseDown(int button);
    bool IsStayMouseDown(int button);
    bool IsOnceMouseUp(int button);

    POINT GetMousePos() const { return mousePos; }
    void SetMousePos(const POINT& pt) { mousePos = pt; }

    void SetWheelDelta(short delta) { wheelDelta = delta; }
    short GetWheelDelta() const { return wheelDelta; }
};
