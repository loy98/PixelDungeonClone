#pragma once
#include "Singleton.h"
#include "config.h"
#include <bitset>

#define MOUSE_LEFT    VK_LBUTTON
#define MOUSE_RIGHT   VK_RBUTTON
#define MOUSE_MIDDLE  VK_MBUTTON

typedef enum class InputLayer {
    LEVEL,
    UI,
    ALARM,
    ESC,
    COUNT
} Layer;

class MouseManager : public Singleton<MouseManager>
{
private:
    // 상태 관리용 배열
    bool prevMouseDown[3]{ false, false, false };
    bool currMouseDown[3]{ false, false, false };

    POINT mousePos;
    Layer currLayer;

    POINT dragStartP;
    POINT dragEndP;
    POINT prevP;

    long deltaX;
    long deltaY;

    bool isDragging[3]{ false, false, false };

    bool valueUsed;

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

    inline bool GetIsDragging(int button) {
        int idx = (button == MOUSE_LEFT) ? 0 : (button == MOUSE_RIGHT ? 1 : 2);
        return isDragging[idx];
    }

    inline POINT GetDragEndP() { return dragEndP; }

    inline bool GetValueUsed() { return valueUsed; }
    inline void AlreadyUsed() { valueUsed = true; }

    inline long GetDeltaX() { return deltaX; }
    inline long GetDeltaY() { return deltaY; }
};
