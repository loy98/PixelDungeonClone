#include "CombatSyetem.h"
#include "Entity.h"
#include "UIManager.h"

int CombatSyetem::CalculateDmg(Entity* attacker, Entity* defender)
{
	// 공격자 데미지 min ~ max 사이 랜덤값으로 설정
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
	// 10% 확률로 고정 회피
	if (IsAttackEvaded())
	{
		wstring eng = defender->GetImage()->imgName + L" is Evaded.";
		wstring kor = defender->GetImage()->imgName + L" 가공격을 피했습니다";
		UIManager::GetInstance()->SendLog(eng, D2D1::ColorF(D2D1::ColorF::Orange));
		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Orange));
		return;
	}

	int dmg = CalculateDmg(attacker, defender);

	defender->TakeDamage(dmg);
	if (dmg == 0)
	{
		wstring eng = defender->GetImage()->imgName + L" is Blocking.";
		wstring kor = defender->GetImage()->imgName + L" 가 공격을 막았습니다.";
		UIManager::GetInstance()->SendLog(eng, D2D1::ColorF(D2D1::ColorF::Blue));
		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Blue));
	}
	else if (defender->GetType() == EntityType::PLAYER)
	{
		wstring eng = attacker->GetImage()->imgName +L" hit you.";
		wstring kor = attacker->GetImage()->imgName +L" 에게 맞았습니다.";
		UIManager::GetInstance()->SendLog(eng, D2D1::ColorF(D2D1::ColorF::White));
		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Yellow));

		if (defender->GetHP() < dmg)
		{
			wstring eng = attacker->GetImage()->imgName + L" killed you...";
			wstring kor = L"당신은 " + attacker->GetImage()->imgName + L" 에 의해 죽었습니다....";
			UIManager::GetInstance()->SendLog(eng, D2D1::ColorF(D2D1::ColorF::Red));
			UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Red));
		}
	}
	else if (defender->GetType() == EntityType::MONSTER)
	{
		wstring eng = L"You attack " + attacker->GetImage()->imgName;
		wstring kor = attacker->GetImage()->imgName +L" 를 공격했습니다.";
		UIManager::GetInstance()->SendLog(eng, D2D1::ColorF(D2D1::ColorF::White));
		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Yellow));
	}
}

bool CombatSyetem::IsAttackEvaded()
{
	int evadeChancePercent = 10;  // 10% 확률 회피
	int roll = rand() % 100;      // 0 ~ 99
	return roll < evadeChancePercent;
}