#include "Player.h"
// #include "Game.h"
#include "TurnManager.h"
#include "KeyManager.h"
#include "Level.h"
#include "TimerManager.h"

Player::Player(FPOINT pos, float speed, int hp, int attDmg, int defense)
{
    position = pos;
    this->speed = speed;
    this->hp = hp;
    this->maxHp = hp;
    this->attackDmg = attDmg;
    this->defense = defense;
    isMoving = false;
    isActive = true;

    type = EntityType::PLAYER;
    curState = EntityState::IDLE;

    // 에너지 test
    actionCost = 10.f;
    energyPerTurn = 10.0f;
}

Player::~Player()
{
}

void Player::Act(Level* level)
{
    switch (curState)
    {
    case EntityState::IDLE:
        ActIdle(level);
        return;
    case EntityState::MOVE:
        Move(level);
        return;
    case EntityState::ATTACK:
        Attack(level);
        return;
    case EntityState::DEAD:
        // player는 죽은채로 계속 애니메이션 돼야함
        return;
    }
}

void Player::Attack(Level* level)
{
    if (target)
    {
        //level->ProcessCombat(this, target);
        curState = EntityState::IDLE;
    }
}

void Player::ActIdle(Level* level)
{
    KeyManager* km = KeyManager::GetInstance();
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
    target = level->GetActorAt(targetPos);
    if (target)
    {
        curState = EntityState::ATTACK;
        return;
    }    
    if (!level->GetMap(targetPos.x, targetPos.y)->CanGo()) return;

    curState = EntityState::MOVE;
}

void Player::Move(Level* level)
{
    //if (!level->GetMap(targetPos.x, targetPos.y)->CanGo())
    //{
    //    curState = EntityState::IDLE;
    //    return;
    //}
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
        curState = EntityState::IDLE;
    }
}
