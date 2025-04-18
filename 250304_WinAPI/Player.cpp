#include "Player.h"
#include "Map.h"
#include "Game.h"
#include "TurnManager.h"
#include "KeyManager.h"

Player::Player(FPOINT pos)
{
    position = pos;
}

Player::~Player()
{
}

void Player::Act(Game* game)
{
    TurnManager* tm = game->GetTurnManager();
    KeyManager* km = KeyManager::GetInstance();
    Map* map = game->GetMap();

    if (km->IsOnceKeyDown(VK_UP))
        Move({ 0, -TILE_HEIGHT }, map);
    else if (km->IsOnceKeyDown(VK_DOWN))
        Move({ 0, TILE_HEIGHT }, map);
    else if (km->IsOnceKeyDown(VK_LEFT))
        Move({ -TILE_WIDTH, 0 }, map);
    else if (km->IsOnceKeyDown(VK_RIGHT))
        Move({ TILE_WIDTH, 0 }, map);

    // endturn을 move 안에서..? -> 인자에 turnManager 받아야함
    tm->EndTurn();
}

bool Player::NeedsInput()
{
    return false;
}

void Player::Move(FPOINT delta, Map* map)
{
    // 다음 좌표
    float nextX = position.x + delta.x;
    float nextY = position.y + delta.y;

    if (!map->CanGo({ nextX, nextY })) return;

    position.x = nextX;
    position.x = nextY;
}
