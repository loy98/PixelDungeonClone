// MouseManager.h
#pragma once
#include "Singleton.h"
#include "config.h"

#define MOUSE_LEFT    VK_LBUTTON
#define MOUSE_RIGHT   VK_RBUTTON
#define MOUSE_MIDDLE  VK_MBUTTON

typedef enum class InputLayer {
    LEVEL,
    UI,
    ALARM,
    ESC,
    COUNT
}Layer;


class MouseManager : public Singleton<MouseManager>
{
private:
    bitset<3> mouseDown; // [0]: Left, [1]: Right, [2]: Middle
    bitset<3> mouseUp;

    POINT mousePos;
    Layer currLayer;

    POINT dragStartP;
    POINT dragEndP;
    POINT prevP;

    long deltaX;
    long deltaY;

    bool isDragging[4];

    short wheelDelta;

public:
    HRESULT Init();
    void Update();
    void Release();

    bool IsOnceMouseDown(int button);
    bool IsStayMouseDown(int button);
    bool IsOnceMouseUp(int button);

    bool IsOnceMouseDown(int button, Layer thisLayer);
    bool IsStayMouseDown(int button, Layer thisLayer);
    bool IsOnceMouseUp(int button, Layer thisLayer);

    void SetLayer();

    void InitPoints();

    inline POINT GetMousePos() const { return mousePos; }
    inline void SetMousePos(const POINT& pt) { mousePos = pt; }

    inline void SetWheelDelta(short delta) { wheelDelta = delta; }
    inline short GetWheelDelta() const { return wheelDelta; }

    inline bool GetIsDragging(int button) { return isDragging[button]; }

    inline long GetDeltaX() { return deltaX; }
    inline long GetDeltaY() { return deltaY; }
};
