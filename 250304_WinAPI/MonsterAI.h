#pragma once
#include "config.h"
#include "CommonFunction.h"
#include "Entity.h"
enum class MonsterState
{
    SLEEP,
    WANDER,
    ATTACK,
    FOLLOW
};

class Level;
class Monster;
class Entity;
class MonsterAI
{
private:
    MonsterState currMonsterState;
    MonsterState prevMonsterState;
    vector<FPOINT> fovList;
    Entity* target;

public:
    MonsterAI();
    ~MonsterAI() {};
    void Act(Level* level, Monster *monster, bool isAlert= false);
    //void Move(Level* level, Monster* monster);
    bool CanAttack(Level* level, Monster* monster);
    void Attack(Level* level, Monster* monster);
    void Hunting(Level* level, Monster* monster);
    void Wandering(Level* level, Monster* monster);;
    void SetFov(Level* level, Monster* monster);
    //bool CanSee(Level* level, Monster* monster);

    bool UpdateFovInfo(Level* level, Monster* monster);
    FPOINT SetRandomTargetPos(Level* level, Monster* monster);
};

