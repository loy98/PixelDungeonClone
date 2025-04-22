#pragma once
#include "config.h"
#include "Entity.h"

enum class MonsterState
{
    SLEEP,
    MOVE,
    ATTACK,
    FOLLOW
};

//enum class EntityState
//{
//    IDLE,
//    MOVE,
//    ATTACK,
//    DEAD
//};

class Level;
class Monster : public Entity
{
public:
    Monster(FPOINT pos, float speed, int hp, int attDmg, int defense);
    virtual ~Monster();

    virtual void Act(Level* level) override;
    void Move(Level* level);
    virtual void Attack(Level* level) override;
    void ActIdle(Level* level);

    bool NeedsInput() override { return false; }
    bool IsBusy() override { return curState != EntityState::IDLE; }

    // 랜덤한 위치로 이동
    void SetRandomTargetPos();
};

