#pragma once
#include "config.h"

class Entity
{
protected:
    FPOINT position;
public:
    int graphicID;
    inline void SetPosition(const float x, const float y) { this->position.x = x; this->position.y = y; }
    inline void SetPosition(FPOINT postion) { this->position.x = postion.x; this->position.y = postion.y; }
    inline FPOINT GetPosition() const { return position; }
};
