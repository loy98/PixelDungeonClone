#pragma once
#include "config.h"

class Item;
class Entity;
struct slot
{
	int count;
	Item* item;
};

class Inventory
{
private:
	// 슬롯 25개에 갯수 중첩 -> 이중벡터? or 수량 따로 관리
	vector<slot> items{ 25, {0, nullptr} };
	unordered_map<string, int> indexMap;

	Entity* owner;
public:
	Inventory();
	~Inventory();

	void AddItem(Item* item);
	Item* GetItem(string name);
	// 외부에서 remove 하고 그 친구를 처리 후 delete?
	void UseItem(string name);
};

