#pragma once
#include "config.h"
#include "Entity.h"

class Map;
class Monster : public Entity
{
public:
    Monster(FPOINT pos, float speed, int hp, int attDmg, int defense);
    virtual ~Monster();

    virtual void Act(Game* game) override;
    virtual void Attack(Game* game) override;
    void ActIdle(Game* game);
    void Move(Game* game);

    bool NeedsInput() override { return false; }
    bool IsBusy() override { return curState != EntityState::IDLE; }

    // 테스트용 다음 위치 설정
    void SetRandomTargetPos();
};

