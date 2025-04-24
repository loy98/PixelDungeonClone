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

	int dmg = CalculateDmg(attacker, defender);
	defender->TakeDamage(dmg);

	// 시스템 로그-공격
	if (attacker->GetType() == EntityType::PLAYER)
	{
		// 플레이어 공격
		wstring monster = L"몬스터";
		// 영어
		//wstring eng = L"You hit " + monster + L".";
		// 한글-임시
		wstring kor = monster + L"을(를) 공격했다.";
		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::White));
	}
	else if (attacker->GetType() == EntityType::MONSTER)
	{
		// 플레이어 피격
		wstring monster = L"몬스터";
		// 영어
		//wstring eng = monster + L" hit you.";
		// 한글-임시
		wstring kor = monster + L"가 당신을 공격했다.";
		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::White));
	}

	if (defender->GetType() == EntityType::MONSTER && !defender->IsAlive())
	{
		attacker->TakeExp(defender->GetExp());
		
		// 시스템 로그-몬스터 사망
		wstring monster = L"몬스터";
		// 한글
		wstring kor = L"당신은 " + monster + L"을(를) 쓰러뜨렸다.";
		// 영어
		//wstring eng = L"You defeated " + monster + L".";

		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::White));
	}
}
