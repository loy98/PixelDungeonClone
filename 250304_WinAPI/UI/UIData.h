#pragma once

class D2DImage;

struct ItemData
{
    string name = "";
    string description = "";
    D2DImage* origin = nullptr;
    int frameX = 0;
    int frameY = 0;
};

struct UIInventorySlotData
{
    //int itemId = 0;
    string name = "";
    D2DImage* icon = nullptr;
    int quantity = 0;
    int enhancement = 0;
    const ItemData* info;
};

struct PlayerStatData {
    std::wstring title{L"스탯창"};
    int attack = 0;
    int hp = 0;
    int maxHp = 0;
    int exp = 0;
    int maxExp = 0;
    int def = 0;
    int level = 0;
};
