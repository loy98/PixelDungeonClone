#include "CombatSyetem.h"
#include "Entity.h"

int CombatSyetem::CalculateDmg(Entity* attacker, Entity* defender)
{
	// 랜덤으로 시드 생성
	random_device rd;
	mt19937_64 eng(rd());
	uniform_int_distribution<int> dist(attacker->GetAttDmg().min, attacker->GetAttDmg().max);

	int applyDmg = dist(eng) - defender->GetDefense();
	return max(applyDmg, 0);
}

void CombatSyetem::ProcessAttack(Entity* attacker, Entity* defender)
{
	if (!attacker || !defender)	return;
	if (attacker->GetType() == defender->GetType())	return;

	int dmg = CalculateDmg(attacker, defender);
	defender->TakeDamage(dmg);
	if (defender->GetType() == EntityType::MONSTER && !defender->IsAlive())
		attacker->TakeExp(defender->GetExp());
}
