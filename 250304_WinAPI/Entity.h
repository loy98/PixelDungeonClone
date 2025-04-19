#pragma once
#include "config.h"

class Game;
class Entity
{
protected:
    FPOINT position;
    float speed;
public:
    int graphicID;
    bool isActive;
public:
    Entity();
    virtual ~Entity();

    void Render(HDC hdc);
    virtual void Act(Game* game);
    virtual bool NeedsInput() = 0;

    inline void SetPosition(const float x, const float y) { this->position.x = x; this->position.y = y; }
    inline void SetPosition(FPOINT postion) { this->position.x = postion.x; this->position.y = postion.y; }
    inline FPOINT GetPosition() const { return position; }
};
