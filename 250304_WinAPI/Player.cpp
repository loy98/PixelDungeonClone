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
    isMoving = false;
}

Player::~Player()
{
}

void Player::Act(Game* game)
{
    KeyManager* km = KeyManager::GetInstance();

    if (isMoving)
    {
        Move(game);
        return;
    }

    //if (km->IsOnceKeyDown(VK_UP))
    //    Move({ 0, -TILE_SIZE }, map, game);
    //else if (km->IsOnceKeyDown(VK_DOWN))
    //    Move({ 0, TILE_SIZE }, map, game);
    //else if (km->IsOnceKeyDown(VK_LEFT))
    //    Move({ -TILE_SIZE, 0 }, map, game);
    //else if (km->IsOnceKeyDown(VK_RIGHT))
    //    Move({ TILE_SIZE, 0 }, map, game);

    // endturn을 move 안에서..? -> 인자에 turnManager 받아야함
    // 이동이 수행 됐을 때만 endturn

    // 에이스타용
    if (km->IsOnceKeyDown(VK_UP))
    //---
        Move({ 0, -30 }, map, game);
    else if (km->IsOnceKeyDown(VK_DOWN))
        Move({ 0, 30 }, map, game);
    else if (km->IsOnceKeyDown(VK_LEFT))
        Move({ -30, 0 }, map, game);
    else if (km->IsOnceKeyDown(VK_RIGHT))
        Move({ 30, 0 }, map, game);
  
    //TODO:MergeCheck
        targetPos = { position.x, position.y - TILE_SIZE };
    else if (km->IsOnceKeyDown(VK_DOWN))
        targetPos = { position.x, position.y + TILE_SIZE };
    else if (km->IsOnceKeyDown(VK_LEFT))
        targetPos = { position.x - TILE_SIZE, position.y };
    else if (km->IsOnceKeyDown(VK_RIGHT))
        targetPos = { position.x + TILE_SIZE, position.y };
    else return;

    isMoving = true;

}

bool Player::NeedsInput()
{
    return !isMoving;
}

bool Player::IsBusy()
{
    return isMoving;
}

void Player::Move(Game* game)
{
    if (!game->GetMap()->CanGo(targetPos)) return;

    FPOINT delta = targetPos - position;

    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
    delta.Normalize();

    position.x += speed * deltaTime * delta.x;
    position.y += speed * deltaTime * delta.y;

    delta = targetPos - position;

    // 매직넘버로,,, -> 변수로 dir 저장해두고 쓰면 Dot Product
    if (delta.Length() <= 0.5f)
    {
        position = targetPos;
        isMoving = false;
    }
}
