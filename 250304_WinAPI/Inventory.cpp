#include "Inventory.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::AddItem(Item* item)
{
	for (int i = 0; i < items.size(); ++i)
	{
		if (items[i] == nullptr)
		{
			items[i] = item;
			return;
		}
	}
}

Item* Inventory::GetItemAt(int index)
{
	if (index < 0 || index >= items.size()) return nullptr;
	return items[index];
}

Item* Inventory::RemoveItemAt(int index)
{
	if (index < 0 || index >= items.size()) return nullptr;
	Item* item = items[index];
	items[index] = nullptr;
	return item;
}
