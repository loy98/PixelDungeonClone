#pragma once
#include "config.h"
#include "Entity.h"

//enum class EntityState
//{
//    IDLE,
//    MOVE,
//    ATTACK,
//    DEAD
//};

class MonsterAI;
class Level;
class Monster : public Entity
{
public:
    Monster(FPOINT pos, float speed, int hp, int attDmg, int defense);
    virtual ~Monster();

    virtual void Act(Level* level) override;
    void Move(Level* level);
    virtual void Attack(Level* level) override;
    void ActIdle(Level* level);//AI

    bool NeedsInput() override { return false; }
    bool IsBusy() override { return curState != EntityState::IDLE; }

    // 랜덤한 위치로 이동-wandering-플레이어 있는지 계속 체크
    void SetRandomTargetPos();

    // monsterAI
    MonsterAI* monsterAi;
    void SetEntityState(EntityState state) { this->curState = state; }
};

