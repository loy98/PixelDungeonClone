#include "Inventory.h"

#include "CommonFunction.h"
#include "Item.h"
#include "Equipment.h"
#include "Entity.h"

#include "UIManager.h"
#include "FModSoundPlayer.h"

Inventory::Inventory(Entity* owner) : owner(owner)
{
}

Inventory::~Inventory()
{
}

void Inventory::AddItem(Item* item)
{
	string key = item->GetName();

    // key 값에 해당하는 인덱스가 있으면 인벤토리에서
    // 해당 인덱스 count 증가
	auto it = indexMap.find(key);

    // 장비... 같은 이름이면 indexMap 사용 불가,,, 같은 장비 못먹는다?
    if (it != indexMap.end() && indexMap[key] != -1)
    {
        if (item->GetItemType() == ItemType::CONSUMABLE)
        {
            items[it->second].count++;
        }
        return;
    }

    // 없으면 빈 슬롯 찾아 할당
    for (int i = 3; i < (int)items.size(); ++i) 
    {
        if (items[i].count == 0) 
        {
            items[i].item = item;
            items[i].count = 1;
            indexMap[key] = i;
            return;
        }
    }
}

slot Inventory::GetSlot(int idx)
{
    return items[idx];
}
Item* Inventory::GetItem(string name)
{
    if (indexMap[name] == -1) return nullptr;
    int index = indexMap[name];
	if (index < 0 || index >= items.size()) return nullptr;

	return items[index].item;
}

void Inventory::UseItem(int idx)
{
    if (idx > 0 && idx < 2) return;

    if (items[idx].item == nullptr) return;
    UIManager::GetInstance()->SendLog((L"Item 사용 " + to_wstring(idx)), D2D1::ColorF(D2D1::ColorF::Blue));
    UseItem(items[idx].item->GetName());
    
}
void Inventory::UseItem(string name)
{
    if (indexMap[name] == -1) return;
    int index = indexMap[name];
	if (index < 0 || index >= items.size()) return;
    if (items[index].count <= 0) return;

    Item* item = items[index].item;
    switch (item->GetItemType())
    {
    case ItemType::CONSUMABLE:
        item->Use(owner, nullptr); // 텔포 스크롤떄문에 Use함수 수정중,,
        items[index].count--;
        owner->SetState(EntityState::USING_ITEM);
        FModSoundPlayer::GetInstance()->Play("drink", 0.7f);
        if (items[index].count <= 0)
        {
            // 인벤토리에서 제거
            items[index].count = 0;
            delete items[index].item;
            items[index].item = nullptr;

            // 인덱스 정보 -1로 초기화
            indexMap[name] = -1;
        }
        return;
    case ItemType::EQUIPMENT:
        EquipType type = static_cast<Equipment*>(item)->GetEqType();
        Swap(index, type);
        UpdateEqStats();
        if (type == EquipType::ARMOR)
            owner->SetFrameY(item->GetLevel());
        return;
    }
	return;
}

void Inventory::Swap(int src, int dest)
{
    swap(items[src], items[dest]);
    if (items[src].item)
        indexMap[items[src].item->GetName()] = src;
    if (items[dest].item)
        indexMap[items[dest].item->GetName()] = dest;
}

void Inventory::UpdateEqStats()
{
    EqStat bonus{ {0,0}, 0, 0 };
    if (items[EquipType::WEAPON].item)
    {
        bonus.bonusDmg.min = static_cast<Equipment*>(items[EquipType::WEAPON].item)->GetBonusValue();
        bonus.bonusDmg.max = static_cast<Equipment*>(items[EquipType::WEAPON].item)->GetBonusValue();
    }
    if (items[EquipType::ARMOR].item)
        bonus.bonusHp = static_cast<Equipment*>(items[EquipType::ARMOR].item)->GetBonusValue();
    if (items[EquipType::SHIELD].item)
        bonus.bonusDf = static_cast<Equipment*>(items[EquipType::SHIELD].item)->GetBonusValue();

    owner->SetBonusStats(bonus);
    owner->UpdateStat();
}
