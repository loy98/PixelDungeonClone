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

	Move({ 30, 30 }, map);
    //if (isMoving)
    //{
        //Move(level);
        // 에이스타용
	//}
    //else
    //{
        //isMoving = true;
    //}
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
    FPOINT delta = position - position;
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
