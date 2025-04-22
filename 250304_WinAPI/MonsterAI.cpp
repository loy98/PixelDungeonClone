#include "MonsterAI.h"
#include "Monster.h"
#include "Level.h"
#include "CombatSyetem.h"

void MonsterAI::Act(EntityState entityState, Level* level, Monster* monster, bool isAlert)
{
	// 시야에 보이는지-시야 범위 있어야 함.
    // 시야 얻음
    UpdateFovInfo(level, monster);

    if (isAlert)
    {
        currMonsterState = MonsterState::HUNT;
    }

    switch (currMonsterState)
    {
    case MonsterState::IDLE:
        // 시야 정보 업데이트
        break;
    case MonsterState::SLEEP:
        // 턴 소모
        break;
    case MonsterState::HUNT:
        // 타겟위치로 도착지 설정
        Follow(level, monster);
        break;
    case MonsterState::ATTACK:
        monster->SetEntityState(EntityState::ATTACK);
        break;
    case MonsterState::WANDER:
        Wandering(level, monster);
        break;
    }
}
//
//void MonsterAI::Move(Level* level, Monster* monster)
//{
//    switch (currMonsterState)
//    {
//    case MonsterState::HUNT:
//        Follow(level, monster);
//        break;
//    case MonsterState::WANDER:
//        Wandering(level, monster);
//        break;
//    }
//}

void MonsterAI::Follow(Level* level, Monster* monster)
{
    // 공격가능?
    // 목표 위치에 액터?
    Entity* actor = level->GetActorAt(monster->GetTargetPos());

    if (actor == monster->GetTarget())
    {
        monster->SetEntityState(EntityState::ATTACK);
    }

    monster->SetEntityState(EntityState::MOVE);
    currMonsterState = MonsterState::HUNT;
}

void MonsterAI::Attack(Level* level, Monster* monster)
{
    // 한번더 가능이면 attack, 아니면 follow
    if (monster->GetTarget())
    {
        CombatSyetem::GetInstance()->ProcessAttack(monster, monster->GetTarget());
        
        currMonsterState = MonsterState::HUNT;
        monster->SetEntityState(EntityState::MOVE);

    }
}

void MonsterAI::Wandering(Level* level, Monster* monster)
{
    SetRandomTargetPos(level, monster);

    // move;
    auto map = level->GetMap(monster->GetTargetPos().x, monster->GetTargetPos().y);
    Entity* actor = level->GetActorAt(monster->GetTargetPos());
    
    if ((map && !map->CanGo()) || actor)
    {
        currMonsterState = MonsterState::SLEEP;
    }
}

// 임시
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
            float nx = monster->GetPosition().x + TILE_SIZE * dx[j];
            float ny = monster->GetPosition().y + TILE_SIZE * dy[i];

            fovList.push_back({nx, ny});
        }
    }
}

bool MonsterAI::UpdateFovInfo(Level* level, Monster* monster)
{
    // 시야 업데이트
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
                monster->SetTarget(actor);
                monster->SetTargetPos(fovList[i]);
                currMonsterState = MonsterState::HUNT;
                return true;
            }
            else if (actor->GetType() == EntityType::MONSTER)
            {
                // 주변 몬스터 상태 정보(피격 상태면 사냥모드로 돌입)
                break;
            }
        }

        currMonsterState = MonsterState::IDLE;
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
    FPOINT dir[] = { {-TILE_SIZE, 0}, {TILE_SIZE, 0}, {0, TILE_SIZE}, {0, -TILE_SIZE} };

    monster->SetTargetPos(monster->GetPosition() + dir[dist(eng)]);

    return monster->GetPosition() + dir[dist(eng)];
}