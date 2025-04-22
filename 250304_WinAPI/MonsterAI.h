#pragma once

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
public:
    MonsterAI() { currMonsterState = MonsterState::IDLE; };
    ~MonsterAI() {};
    void Act(Level* level, Monster *monster);
};

