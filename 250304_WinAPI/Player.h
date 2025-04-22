#pragma once
#include "Entity.h"

class Map;
class Player : public Entity
{
public:
    Player() {};
    Player(FPOINT pos, float speed);
    virtual ~Player();

    virtual void Act(Game* game) override;
    virtual bool NeedsInput() override;
    bool IsBusy() override;

    void Move(Game* game);
};
