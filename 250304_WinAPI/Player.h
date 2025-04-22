#pragma once
#include "Entity.h"

class Level;
class Player : public Entity
{
public:
    Player() {};
    Player(FPOINT pos, float speed);
    virtual ~Player();

    void Render(HDC hdc) override;
    virtual void Act(Level* level) override;
    virtual bool NeedsInput() override;
    bool IsBusy() override;

    void Move(Level* level);
};
