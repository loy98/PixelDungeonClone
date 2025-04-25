#include "Entity.h"
#include "Level.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "TimerManager.h"
#include "UIManager.h"
#include "Animator.h"
#include "FModSoundPlayer.h"

Entity::Entity()
{
	image = D2DImageManager::GetInstance()->AddImage("warrior", L"Image/warrior.png", 21, 7);
	//image = D2DImageManager::GetInstance()->AddImage("entityTest", L"Image/rat.png", 16, 3);
	isActive = true;
	animator = new Animator;
}

Entity::~Entity()
{
	entityObserver.NotifySystemRelese(this);
}

void Entity::Update()
{
	float delta = TimerManager::GetInstance()->GetDeltaTime();
	animator->Update(delta);
	curAnimFrame = animator->GetCurFrame();
}

void Entity::Render(HDC hdc)
{
	image->Middle_RenderFrameScale(position.x, position.y, 2, 2, curAnimFrame, 1 );
}

void Entity::Act(Level* level)
{
}

void Entity::TakeDamage(int dmg)
{
	hp -= dmg;
	Stop();
	if (type == EntityType::PLAYER)
	{
		entityObserver.NotifyDamageTaken(this, dmg, D2D1::ColorF(D2D1::ColorF::Red));
	}
	else
	{
		entityObserver.NotifyDamageTaken(this, dmg, D2D1::ColorF(D2D1::ColorF::White));
	}
	if (hp <= 0)
	{
		hp = 0;
		//isActive = false;
		curState = EntityState::DEAD; // 몬스터 죽은거 확인용
		SetState(EntityState::DEAD);
		entityObserver.NotifyDeath(this);
	}
}

void Entity::TakeExp(int exp)
{
	this->exp += exp;
	if (this->exp / level == 20)
	{
		this->exp = this->exp % level;
		LevelUp();
	}
}

void Entity::LevelUp()
{
	level++;
	baseDmg.min += 1;
	baseDmg.min += 1;
	hp += 5;
	baseDf += 1;
	UpdateStat();
}

void Entity::UpdateStat()
{
	attackDmg = baseDmg + eqStat.bonusDmg;
	maxHp = baseMaxHp + eqStat.bonusHp;
	defense = baseDf + eqStat.bonusDf;
	entityObserver.NotifyHeal(this, 0);
}

void Entity::SetState(EntityState state)
{
	switch (state)
	{
	case EntityState::IDLE:
		curState = EntityState::IDLE;
		animator->Play("Idle");
		break;
	case EntityState::MOVE:
		curState = EntityState::MOVE;
		animator->Play("Move");
		FModSoundPlayer::GetInstance()->Play("step", 0.3f);
		break;
	case EntityState::ATTACK:
		curState = EntityState::ATTACK;
		animator->Play("Attack");
		FModSoundPlayer::GetInstance()->Play("hit", 0.3f);
		break;
	case EntityState::DEAD:
		curState = EntityState::DEAD;
		animator->Play("Dead");
		break;
	case EntityState::WAIT:
		curState = EntityState::WAIT;
		break;
	case EntityState::SLEEP:
		curState = EntityState::SLEEP;
		break;
	case EntityState::GET_ITEM:
		curState = EntityState::GET_ITEM;
		animator->Play("GetItem");
		FModSoundPlayer::GetInstance()->Play("get_item", 0.3f);
		break;
	case EntityState::USING_ITEM:
		curState = EntityState::USING_ITEM;
		animator->Play("UsingItem");
		break;
	}

}
