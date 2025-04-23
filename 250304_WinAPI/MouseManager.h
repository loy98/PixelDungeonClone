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

    POINT dragStartP[3];
    POINT dragEndP[3];
    POINT prevP[3];
    POINT clickedP[3];

    float dragElapsedTime;

    long deltaX;
    long deltaY;

    bool isDragging[3]{ false, false, false };

    bool dragEndPUsed[3];
    bool clickedPUsed[3];

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
        return isDragging[BtoInt(button)];
    }

    inline POINT GetDragStartP(int button = MOUSE_LEFT) {
        return dragStartP[BtoInt(button)];
    }
    inline POINT GetDragEndP(int button = MOUSE_LEFT) {
        return dragEndP[BtoInt(button)];
    }
    inline POINT GetClickP(int button = MOUSE_LEFT) {
        return clickedP[BtoInt(button)];
    }
    inline POINT GetPrevP(int button = MOUSE_LEFT) {
        return prevP[BtoInt(button)];
    }

    inline bool GetDragValueUsed(int button = MOUSE_LEFT) {
        return dragEndPUsed[BtoInt(button)];
    }
    inline void AlreadyDragUsed(int button = MOUSE_LEFT) {
        dragEndPUsed[BtoInt(button)] = true; }

    inline bool GetClickValueUsed(int button = MOUSE_LEFT) {
        return clickedPUsed[BtoInt(button)];
    }
    inline void AlreadyClickUsed(int button = MOUSE_LEFT) {
        clickedPUsed[BtoInt(button)] = true; }

    inline long GetDeltaX() { return deltaX; }
    inline long GetDeltaY() { return deltaY; }

    inline int BtoInt(int button){ return button == MOUSE_LEFT ? 0 : (button == MOUSE_RIGHT ? 1 : 2);}
};
