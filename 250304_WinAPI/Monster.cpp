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

    tm->EndTurn();
}

bool Monster::NeedsInput()
{
    return false;
}

void Monster::Move(FPOINT delta, Map* map)
{
}
