#pragma once
#include "config.h"
#include "Entity.h"

class Map;
class Monster : public Entity
{
public:
    Monster(FPOINT pos, float speed);
    virtual ~Monster();

    virtual void Act(Game* game) override;
    virtual bool NeedsInput() override;
    bool IsBusy() override;

    void Move(Game* game);
};

