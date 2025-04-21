#include "Player.h"
// #include "Game.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "TurnManager.h"
#include "KeyManager.h"
#include "Level.h"
#include "TimerManager.h"

Player::Player(FPOINT pos, float speed)
{
    position = pos;
    this->speed = speed;
    isMoving = false;
    image = D2DImageManager::GetInstance()->AddImage("player", L"Image/warrior.png", 21, 7); 
}

Player::~Player()
{
}

void Player::Render(HDC hdc)
{
    if (image)
        image->Middle_RenderFrameScale(position.x, position.y, 2.f, 2.f, 1, 1);
}

void Player::Act(Level* level)
{
    KeyManager* km = KeyManager::GetInstance();

    if (isMoving)
    {
        Move(level);
        return;
    }

    if (km->IsOnceKeyDown(VK_UP))
        targetPos = { position.x, position.y - TILE_SIZE };
    else if (km->IsOnceKeyDown(VK_DOWN))
        targetPos = { position.x, position.y + TILE_SIZE };
    else if (km->IsOnceKeyDown(VK_LEFT))
        targetPos = { position.x - TILE_SIZE, position.y };
    else if (km->IsOnceKeyDown(VK_RIGHT))
        targetPos = { position.x + TILE_SIZE, position.y };
    else return;

    isMoving = level->GetMap(targetPos.x, targetPos.y)->CanGo();
}

bool Player::NeedsInput()
{
    return !isMoving;
}

bool Player::IsBusy()
{
    return isMoving;
}

void Player::Move(Level* level)
{
    if (!level->GetMap(targetPos.x, targetPos.y)->CanGo()) return;

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
