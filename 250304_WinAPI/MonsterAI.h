#pragma once
#include "config.h"
#include "CommonFunction.h"

enum class MonsterState
{
    IDLE,
    SLEEP,
    MOVE,
    ATTACK,
    FOLLOW
};

class Level;
class Monster;
class MonsterAI
{
private:
    MonsterState currMonsterState;
    vector<POINT> fovList;
public:
    MonsterAI() { currMonsterState = MonsterState::IDLE; };
    ~MonsterAI() {};
    void Act(Level* level, Monster *monster);
    void SetFov(Level* level, Monster* monster);
};

