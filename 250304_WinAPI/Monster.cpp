#include "Monster.h"
#include "Game.h"
#include "TurnManager.h"

Monster::Monster(FPOINT pos)
{
    position = pos;
}

Monster::~Monster()
{
}

void Monster::Act(Game* game)
{
    TurnManager* tm = game->GetTurnManager();
    Map* map = game->GetMap();

    // 턴 이동 테스트용
    //Move({ TILE_SIZE, TILE_SIZE }, map);

    // 에이스타용
    Move({ 30, 30 }, map);

    // tm->EndTurn();
}

bool Monster::NeedsInput()
{
    return false;
}

void Monster::Move(FPOINT delta, Map* map)
{
    // 이동 순서 체크용
    //Sleep(100);

    position.x += delta.x;
    position.y += delta.y;
}
