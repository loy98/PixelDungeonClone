#pragma once
#include "config.h"
#include "Entity.h"

class Level;
class Monster : public Entity
{
public:
    Monster() {};
    Monster(FPOINT pos, float speed);

    virtual ~Monster();

    virtual void Act(Level* level) override;
    virtual bool NeedsInput() override;
    bool IsBusy() override;

    void Move(Level* level);
};

