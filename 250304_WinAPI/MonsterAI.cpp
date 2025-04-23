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
        Hunting(level, monster);
    }
    else
    {
        if (UpdateFovInfo(level, monster))  // 시야에 있는지 체크-있으면 안에서 타겟설정 및 갈 위치 설정함
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

    // test
    //monster->SetTargetPos(fovList[0]);
    //monster->SetEntityState(EntityState::MOVE);
}

void MonsterAI::Hunting(Level* level, Monster* monster)
{
    if (CanAttack(level, monster))
    {
        monster->SetEntityState(EntityState::ATTACK);
    }
    else
    {
        // destPos를 타겟의 targetPos로 설정
        monster->SetDestPos(target->GetTargetPos());
        currMonsterState = MonsterState::FOLLOW;
    }
}

bool MonsterAI::CanAttack(Level* level, Monster* monster)
{
    // 타겟 pos로 확인
    Entity* actor = level->GetActorAt(monster->GetTargetPos());

    if(actor)
    {
        if (actor == target)
        {
            return true;
        }
        return false;
    }
}

void MonsterAI::Attack(Level* level, Monster* monster)
{
    // 한번더 가능이면 attack, 아니면 follow
    if (monster->GetTarget())
    {
        //CombatSyetem::GetInstance()->ProcessAttack(monster, monster->GetTarget());
        if (level->GetActorAt(monster->GetTargetPos()) == target)
        {
            currMonsterState = MonsterState::ATTACK;
            monster->SetEntityState(EntityState::ATTACK);
        }
        else
        {
            // 타겟 위치로 이동?-어떻게 이동할 건지.
            currMonsterState = MonsterState::FOLLOW;
            monster->SetEntityState(EntityState::MOVE);
        }
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

// 임시-
void MonsterAI::SetFov(Level* level, Monster* monster)
{
    // 적의 시야를 가져오는 함수
    if (!fovList.empty())
    {
        fovList.clear();
    }

    int dx[5] = { -2, -1, 0, 1, 2 };
    int dy[5] = { -2, -1, 0, 1, 2 };
    // 주변 24방향 탐색

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            float nx = monster->GetPosition().x + TILE_SIZE* dx[j];
            float ny = monster->GetPosition().y + TILE_SIZE * dy[i];

            fovList.push_back({nx, ny});
        }
    }
}

bool MonsterAI::UpdateFovInfo(Level* level, Monster* monster)
{
    // 시야 얻음
    SetFov(level, monster);

    // 플레이어 위치 타일 찾기
    Entity* actor;

    for (int i = 0; i < fovList.size(); i++)
    {
        actor = level->GetActorAt(fovList[i]);
        if(actor)
        {
            if (actor->GetType() == EntityType::PLAYER)
            {
                target = actor;
                monster->SetTarget(target);
                //monster->SetTargetPos(fovList[i]);
                return true;
            }
            else if (actor->GetType() == EntityType::MONSTER)
            {
                // 주변 몬스터 상태 정보(피격 상태면 사냥모드로 돌입)
                break;
            }
        }

      
    }

   // currMonsterState = MonsterState::IDLE;
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