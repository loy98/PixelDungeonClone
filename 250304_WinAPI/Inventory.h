#pragma once
#include "config.h"

class Item;
class Inventory
{
private:
	// 슬롯 25개에 갯수 중첩 -> 이중벡터? or 수량 따로 관리
	vector<Item*> items{ 25, nullptr };
public:
	Inventory();
	~Inventory();

	void AddItem(Item* item);
	Item* GetItemAt(int index);
	// 외부에서 remove 하고 그 친구를 처리 후 delete?
	Item* RemoveItemAt(int index);
};

