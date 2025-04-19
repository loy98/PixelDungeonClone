#pragma once
#include "Entity.h"

class Map;
class Player : public Entity
{
public:
    Player(FPOINT pos);
    virtual ~Player();

    virtual void Act(Game* game) override;
    virtual bool NeedsInput() override;
    void Move(FPOINT delta, Map* map, Game* game);
};
