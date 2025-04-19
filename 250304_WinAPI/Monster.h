#pragma once
#include "config.h"
#include "Entity.h"

class Map;
class Monster : public Entity
{
public:
    Monster() {};
    Monster(FPOINT pos);
    virtual ~Monster();

    virtual void Act(Game* game) override;
    virtual bool NeedsInput() override;
    void Move(FPOINT delta, Map* map);
};

