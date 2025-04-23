#include "MonsterAI.h"
#include "Monster.h"
#include "Level.h"
#include "CombatSyetem.h"

MonsterAI::MonsterAI()
{
    currMonsterState = MonsterState::SLEEP;
    target = nullptr;
}

void MonsterAI::Act(Level* level, Monster* monster, bool isAlert)
{

    prevMonsterState = currMonsterState;
	// 시야에 보이는지-시야 범위 있어야 함.

    // 몬스터 destPos(hunting시) 바꿔줌
    // 타겟위치로 도착지 설정(findpath destPos 설정)
   
    if (target)
    {
        Hunting(level, monster);
    }
    else if (isAlert)
    {
        // 타겟 위치 전달 필요
        target = level->GetActorAt(level->GetPlayerPos());
        monster->SetTarget(target);
        Hunting(level, monster);
    }
    else
    {
        if (InFov(level, monster))  // 시야에 있는지 체크-있으면 안에서 타겟설정 및 갈 위치 설정함
        {
            Hunting(level, monster); // monsterAIState: Follow of Attack
        }
        else
        {
            Wandering(level, monster);  // monsterAIState: wander or sleep
        }
    }

    // 몬스터 상태 바꿔줌
    switch (currMonsterState)
    {
    case MonsterState::SLEEP:
        if (prevMonsterState == MonsterState::SLEEP)
        {
            monster->SetEntityState(EntityState::WAIT);
        }
        else
        {
            monster->SetEntityState(EntityState::SLEEP);
        }
        break;
    case MonsterState::FOLLOW:
        monster->SetEntityState(EntityState::MOVE);
        break;
    case MonsterState::ATTACK:
        monster->SetEntityState(EntityState::ATTACK);
        break;
    case MonsterState::WANDER:
        monster->SetEntityState(EntityState::MOVE);
        break;
    }
}

void MonsterAI::Hunting(Level* level, Monster* monster)
{
    if (CanAttack(level, monster))
    {
        /*Attack(level, monster);*/

        currMonsterState = MonsterState::ATTACK;
        //monster->SetEntityState(EntityState::ATTACK);
    }
    else
    {
        monster->SetDestPos(level->GetPlayerPos());
        currMonsterState = MonsterState::FOLLOW;
    }
}

bool MonsterAI::CanAttack(Level* level, Monster* monster)
{
    if(target)
    {
        if (GetDistance(monster->GetPosition(), target->GetPosition()) <= TILE_SIZE * sqrtf(2))
        {
            monster->SetDestPos(monster->GetPosition());
            return true;
        }
        else
            return false;
    }
}

void MonsterAI::Wandering(Level* level, Monster* monster)
{
    SetRandomTargetPos(level, monster);

    if (monster->GetTargetPos() == monster->GetPosition())
    {
        currMonsterState = MonsterState::SLEEP;
        return;
    }

    // move;
    auto map = level->GetMap(monster->GetTargetPos().x, monster->GetTargetPos().y);
    Entity* actor = level->GetActorAt(monster->GetTargetPos());
    
    if ((map && !map->CanGo()) || actor)
    {
        currMonsterState = MonsterState::SLEEP;
    }

    currMonsterState = MonsterState::WANDER;
}

bool MonsterAI::InFov(Level* level, Monster* monster)
{
    int startIndex = level->GetMapIndex((int)monster->GetPosition().x, (int)monster->GetPosition().y);
    int endIndex = level->GetMapIndex((int)level->GetPlayerPos().x, (int)level->GetPlayerPos().y);

    int startPosX = startIndex % TILE_X;
    int startPosY = startIndex / TILE_X;

    int endPosX = endIndex % TILE_X;
    int endPosY = endIndex / TILE_X;

    int w = endPosX - startPosX;
    int h = endPosY - startPosY;

    // 직선이 완만한지 판단-true면 완만, false면 급경사
    bool slopeFlag = abs(w) > abs(h);

    // x값의 변화량. w가 양수면 양의 방향으로 진행
    int dx = (w >= 0) ? 1 : -1;
    int dy = (h >= 0) ? 1 : -1;

    // 판별식에 사용되는 w, h 길이
    int fw = abs(w);
    int fh = abs(h);

    // 판별식
    int f = slopeFlag ? 2 * fh - fw : 2 * fw - fh;
    //판별식의 변화량
    int t = slopeFlag ? 2 * fh : 2 * fw;
    int s = slopeFlag ? -2 * fw : -2 * fh;

    int curX = startPosX;
    int curY = startPosY;

    if (slopeFlag)
    {
        for (int i = 0; i< 20 && curX != endPosX; ++i)
        {
            if (level->IsSolid(curX, curY))
                return false;

            if (f >= 0)
            {
                // 아래로 이동, 판별식 수정
                curY += dy;
                f += s;
            }
            curX += dx;
            f += t;
        }
        if (curX == endPosX)
        {
            target = level->GetActorAt(level->GetPlayerPos());
            monster->SetTarget(target);  // 이게 맞나..
            return true;
        }

    }
    else
    {
        for (int i = 0; i < 20 && curY != endPosY; ++i)
        {
            if (level->IsSolid(curX, curY))
                return false;

            if (f >= 0)
            {
                curX += dx;
                f += s;
            }

            curY += dy;
            f += t;
        }
        if (curY == endPosY)
        {
            target = level->GetActorAt(level->GetPlayerPos());
            monster->SetTarget(target);  // 이게 맞나..
            return true;
        }
    }

    return false;
}

FPOINT MonsterAI::SetRandomTargetPos(Level* level, Monster* monster)
{
    // 랜덤으로 시드 생성
    random_device rd;

    // 고성능 엔진을 시드(rd())로 초기화
    mt19937_64 eng(rd());

    // 범위 설정
    uniform_int_distribution<int> dist(0, 3);
    FPOINT dir[] = { {-TILE_SIZE, 0}, {TILE_SIZE, 0}, {0, TILE_SIZE}, {0, -TILE_SIZE}};

    monster->SetTargetPos(monster->GetPosition() + dir[dist(eng)]);

    return monster->GetPosition() + dir[dist(eng)];
}