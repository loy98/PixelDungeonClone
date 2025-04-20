#pragma once
#include "config.h"

class Level;
class D2DImage;
class Game;
class Entity
{
protected:
    //test
    D2DImage* image;
	FPOINT position;
    FPOINT targetPos;
    float speed;
    bool isMoving;

public:
    int graphicID;
    bool isActive;
public:
    Entity();
    virtual ~Entity();

    void Render(HDC hdc);
    virtual void Act(Level* level);
    virtual bool NeedsInput() = 0;
    virtual bool IsBusy() = 0;

    inline void SetPosition(const float x, const float y) { this->position.x = x; this->position.y = y; }
    inline void SetPosition(FPOINT postion) { this->position.x = postion.x; this->position.y = postion.y; }
    inline FPOINT GetPosition() const { return position; }
};
