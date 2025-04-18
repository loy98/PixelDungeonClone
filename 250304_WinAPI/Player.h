#pragma once
#include "Entity.h"

class Map;
class Player : public Entity
{
public:
    void Move(FPOINT delta, Map* map);
};
