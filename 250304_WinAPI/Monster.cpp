#include "Monster.h"
#include "Game.h"
#include "Map.h"
#include "TurnManager.h"

Monster::Monster(FPOINT pos, float speed, int hp, int attDmg, int defense)
{
    position = pos;
    this->speed = speed;
    this->hp = hp;
    this->maxHp = hp;
    this->attackDmg = attDmg;
    this->defense = defense;
    isMoving = false;
    isActive = true;

    type = EntityType::MONSTER;
    curState = EntityState::IDLE;

    targetPos = { position.x + TILE_SIZE, position.y + TILE_SIZE };
}

Monster::~Monster()
{
}

void Monster::Act(Game* game)
{
    switch (curState)
    {
    case EntityState::IDLE:
        ActIdle(game);
        return;
    case EntityState::MOVE:
        Move(game);
        return;
    case EntityState::ATTACK:
        Attack(game);
        return;
    case EntityState::DEAD:
        // 애니메이션 끝나면 actor 목록에서 지워야함
        return;
    }
}

void Monster::Attack(Game* game)
{
    if (target)
    {
        game->ProcessCombat(this, target);
        SetRandomTargetPos();
        curState = EntityState::IDLE;
    }
}

void Monster::ActIdle(Game* game)
{
    target = game->GetActorAt(targetPos);
    if (target)
    {
        curState = EntityState::ATTACK;
        return;
    }
    curState = EntityState::MOVE;
}

void Monster::Move(Game* game)
{
    // 이동 순서 체크용
    // Sleep(100);
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
        // 테스트용이라 도착지 정하는건 수정해야함
        SetRandomTargetPos();
        curState = EntityState::IDLE;
    }
}

void Monster::SetRandomTargetPos()
{
    // 랜덤으로 시드 생성
    random_device rd;

    // 고성능 엔진을 시드(rd())로 초기화
    mt19937_64 eng(rd());

    // 범위 설정
    uniform_int_distribution<int> dist(0, 3);
    FPOINT dir[] = { {-TILE_SIZE, 0}, {TILE_SIZE, 0}, {0, TILE_SIZE}, {0, -TILE_SIZE} };

    targetPos = position + dir[dist(eng)];
}
