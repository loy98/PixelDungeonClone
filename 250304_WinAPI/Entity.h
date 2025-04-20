#pragma once
#include "config.h"

class Level;
class D2DImage;

enum class EntityState
{
    IDLE,
    MOVE,
    ATTACK,
    DEAD
};
enum class EntityType
{
    PLAYER,
    MONSTER,
    NPC,
    OBJECT
};

class Entity
{
protected:
    //test
    D2DImage* image;
	FPOINT position;

    // 공통 속성
    EntityType type;
    EntityState curState;
    FPOINT position;
    FPOINT targetPos;
    float speed;
    bool isMoving;

    // 전투 속성
    int hp, maxHp;
    int attackDmg, defense;
    Entity* target;
public:
    int graphicID;
    bool isActive;

    wchar_t szText[128]; // 디버깅용
public:
    Entity();
    virtual ~Entity();

    void Render(HDC hdc);
    virtual void Act(Level* level);
    virtual void Attack(Level* level) {};

    virtual bool NeedsInput() = 0;
    virtual bool IsBusy() = 0;
    bool IsAlive() { return curState == EntityState::DEAD; }

    void TakeDamage(int dmg);

    inline void SetPosition(const float x, const float y) { this->position.x = x; this->position.y = y; }
    inline void SetPosition(FPOINT postion) { this->position.x = postion.x; this->position.y = postion.y; }
    inline FPOINT GetPosition() const { return position; }
    inline int GetAttDmg() const { return attackDmg; }
    inline int GetDefense() const { return defense; }
    inline EntityState GetState() const { return curState; }

};
