#include "Player.h"
// #include "Game.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "TurnManager.h"
#include "KeyManager.h"
#include "Level.h"
#include "TimerManager.h"
#include "PathFinder.h"
#include "CombatSyetem.h"
#include "Item.h"
#include "Inventory.h"

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
    level = 1;
    exp = 0;


    type = EntityType::PLAYER;
    curState = EntityState::IDLE;

    // 에너지 test
    actionCost = 10.f;
    energyPerTurn = 10.0f;

    //길찾기
    finder = new PathFinder();
    destPos = position;

    //image = D2DImageManager::GetInstance()->AddImage("player", L"Image/warrior.png", 21, 7); 

    inven = new Inventory(this);
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
        CombatSyetem::GetInstance()->ProcessAttack(this, target);
        Stop();
        curState = EntityState::IDLE;
    }
}

void Player::ActIdle(Level* level)
{
    // 테스트용
    if (KeyManager::GetInstance()->IsOnceKeyDown('P'))
    {
        if (inven)
        {
            inven->UseItem("체력포션");
        }
    }

    if (finder->FindPath(position, destPos, level, OUT path))
        targetPos = path[1];
    if (position == destPos) return;

    target = level->GetActorAt(targetPos);
    if (target)
    {
        curState = EntityState::ATTACK;
        return;
    }    
    if (!level->GetMap(targetPos.x, targetPos.y)->CanGo()) return;

    curState = EntityState::MOVE;
}

void Player::GetItem(Item* item)
{
    if (inven)
    {
        inven->AddItem(item);
    }
}

void Player::Heal(int healAmount)
{
    hp += healAmount;
    if (hp >= maxHp) hp = maxHp;
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
    if (delta.Length() <= 10.f)
    {
        position = targetPos;
        curState = EntityState::IDLE;
    }
}
