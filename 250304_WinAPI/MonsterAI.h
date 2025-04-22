#pragma once
#include "config.h"
#include "CommonFunction.h"
#include "Entity.h"
enum class MonsterState
{
    IDLE,
    SLEEP,
    WANDER,
    ATTACK,
    HUNT
};

class Level;
class Monster;
class Entity;
class MonsterAI
{
private:
    MonsterState currMonsterState;
    vector<FPOINT> fovList;
public:
    MonsterAI() { currMonsterState = MonsterState::IDLE; };
    ~MonsterAI() {};
    void Act(EntityState entityState, Level* level, Monster *monster, bool isAlert= false);
    //void Move(Level* level, Monster* monster);
    void Attack(Level* level, Monster* monster);
    void Follow(Level* level, Monster* monster);
    void Wandering(Level* level, Monster* monster);;
    void SetFov(Level* level, Monster* monster);
    //bool CanSee(Level* level, Monster* monster);

    bool UpdateFovInfo(Level* level, Monster* monster);
    FPOINT SetRandomTargetPos(Level* level, Monster* monster);
};

