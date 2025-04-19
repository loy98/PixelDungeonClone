#include "Player.h"
#include "Map.h"
#include "Game.h"
#include "TurnManager.h"
#include "KeyManager.h"
#include "TimerManager.h"

Player::Player(FPOINT pos, float speed)
{
    position = pos;
    this->speed = speed;
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
        Move({ 0, -TILE_SIZE }, map, game);
    else if (km->IsOnceKeyDown(VK_DOWN))
        Move({ 0, TILE_SIZE }, map, game);
    else if (km->IsOnceKeyDown(VK_LEFT))
        Move({ -TILE_SIZE, 0 }, map, game);
    else if (km->IsOnceKeyDown(VK_RIGHT))
        Move({ TILE_SIZE, 0 }, map, game);

    // endturn을 move 안에서..? -> 인자에 turnManager 받아야함
    // 이동이 수행 됐을 때만 endturn
}

bool Player::NeedsInput()
{
    return true;
}

void Player::Move(FPOINT delta, Map* map, Game* game)
{
    // 다음 좌표
    float nextX = position.x + delta.x;
    float nextY = position.y + delta.y;

    if (!map->CanGo({ nextX, nextY })) return;

    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
    delta.Normalize();

    position.x += speed * deltaTime * delta.x;
    position.y += speed * deltaTime * delta.y;
    
    if (position == FPOINT{ nextX, nextY })
        game->GetTurnManager()->EndTurn();
    else
        Move(delta, map, game);
}
