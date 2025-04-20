#pragma once
#include "Entity.h"

class Map;
class Player : public Entity
{
public:
    Player(FPOINT pos, float speed, int hp, int attDmg, int defense);
    virtual ~Player();

    virtual void Act(Game* game) override;
    virtual void Attack(Game* game) override;
    void ActIdle(Game* game);
    void Move(Game* game);

    bool NeedsInput() override { return true; }
    bool IsBusy() override { return curState != EntityState::IDLE; }
};
