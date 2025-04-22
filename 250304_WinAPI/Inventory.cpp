#include "Inventory.h"
#include "Item.h"

Inventory::Inventory()
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

    if (it != indexMap.end()) 
    {
        items[it->second].count++;
        return;
    }

    // 없으면 빈 슬롯 찾아 할당
    for (int i = 0; i < (int)items.size(); ++i) 
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

Item* Inventory::GetItem(string name)
{
    if (indexMap[name] == -1) return nullptr;
    int index = indexMap[name];
	if (index < 0 || index >= items.size()) return nullptr;

	return items[index].item;
}

void Inventory::UseItem(string name)
{
    if (indexMap[name] == -1) return;
    int index = indexMap[name];
	if (index < 0 || index >= items.size()) return;
    if (items[index].count <= 0) return;

    Item* item = items[index].item;
    item->Use(owner);
    items[index].count--;

	return;
}
