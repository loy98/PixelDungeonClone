#include "Monster.h"
// #include "Game.h"
// #include "Map.h"
#include "Level.h"
#include "TurnManager.h"

Monster::Monster(FPOINT pos, float speed)
{
    position = pos;
    this->speed = speed;
    targetPos = { position.x + TILE_SIZE, position.y + TILE_SIZE };
    // targetPos = pos
}

Monster::~Monster()
{
}

void Monster::Act(Level* level)
{

    // 턴 이동 테스트용
    if (isMoving)
    {
        Move(level);
    }
    else
    {
        isMoving = true;
    }

    // tm->EndTurn();
}

bool Monster::NeedsInput()
{
    return !isMoving;
}

bool Monster::IsBusy()
{
    return isMoving;
}

void Monster::Move(Level* level)
{
    // 이동 순서 체크용
    // Sleep(100);
    auto index = level->GetMapIndex(targetPos.x, targetPos.y);
    int a = 0;
    auto map = level->GetMap(targetPos.x, targetPos.y);
    
    if (map)
    {
        if (!map->CanGo()) return;
    }
    // if (!level->GetMap(targetPos.x, targetPos.x)->CanGo()) return;

    FPOINT delta = position - position;

    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
    delta.Normalize();

    position.x += speed * deltaTime * delta.x;
    position.y += speed * deltaTime * delta.y;

    delta = position - position;

    // 매직넘버로,,, -> 변수로 dir 저장해두고 쓰면 Dot Product
    if (delta.Length() <= 0.5f)
    {
        position = targetPos;
        // 테스트용이라 도착지 정하는건 수정해야함
        // targetPos += { TILE_SIZE, TILE_SIZE };
        isMoving = false;
    }
}
