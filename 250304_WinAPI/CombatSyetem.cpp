#include "CombatSyetem.h"
#include "Entity.h"
#include "UIManager.h"
#include "FModSoundPlayer.h"


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

	// 공격소리
	FModSoundPlayer::GetInstance()->Play("hit", 0.3f);

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

		if (defender->GetHP() + dmg < dmg)
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

	//	// 시스템 로그-공격
	//	if (attacker->GetType() == EntityType::PLAYER)
	//	{
	//		// 플레이어 공격
	//		wstring monster = L"몬스터";
	//		// 영어
	//		//wstring eng = L"You hit " + monster + L".";
	//		// 한글-임시
	//		wstring kor = monster + L"을(를) 공격했다.";
	//		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::White));
	//	}
	//	else if (attacker->GetType() == EntityType::MONSTER)
	//	{
	//		// 플레이어 피격
	//		wstring monster = L"몬스터";
	//		// 영어
	//		//wstring eng = monster + L" hit you.";
	//		// 한글-임시
	//		wstring kor = monster + L"가 당신을 공격했다.";
	//		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::White));
	//	}

	//	if (defender->GetType() == EntityType::MONSTER && !defender->IsAlive())
	//	{
	//		attacker->TakeExp(defender->GetExp());
			
	//		// 시스템 로그-몬스터 사망
	//		wstring monster = L"몬스터";
	//		// 한글
	//		wstring kor = L"당신은 " + monster + L"을(를) 쓰러뜨렸다.";
	//		// 영어
	//		//wstring eng = L"You defeated " + monster + L".";

		//		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::White));

		// }
}

bool CombatSyetem::IsAttackEvaded()
{
	int evadeChancePercent = 10;  // 10% 확률 회피
	int roll = rand() % 100;      // 0 ~ 99
	return roll < evadeChancePercent;
}