#include "UIResourceSubManager.h"
#include "../../D2DImageManager.h"
#include "../UIData.h"

std::unordered_map<std::string, ItemData*> UIResourceSubManager::itemDatas;

void UIResourceSubManager::Preload_StatusToolbar() {
    auto mgr = D2DImageManager::GetInstance();
    mgr->AddImage("status_character_ico", L"assets/interfaces/status_ico.png", 1, 1);
    mgr->AddImage("status_character_bg", L"assets/interfaces/status_bg.png", 1, 1);
    mgr->AddImage("status_hp_bar", L"assets/interfaces/HPBar.png", 1, 1);
    mgr->AddImage("status_hp_bg", L"assets/interfaces/player_hp_barbg.png", 1, 1);
    mgr->AddImage("status_exp_bar", L"assets/interfaces/ExpBar.png", 1, 1);
    mgr->AddImage("status_exp_bg", L"assets/interfaces/player_exp_barbg.png", 1, 1);
    mgr->AddImage("status_level", L"assets/interfaces/level_box.png", 1, 1);
}

void UIResourceSubManager::Preload_QuickSlotToolbar() {
    auto mgr = D2DImageManager::GetInstance();
    mgr->AddImage("item_quick", L"assets/interfaces/item_quick.png", 1, 1);
    mgr->AddImage("search_quick", L"assets/interfaces/search_quick.png", 1, 1);
    mgr->AddImage("sleep_quick", L"assets/interfaces/sleep_quick.png", 1, 1);
    mgr->AddImage("inven_quick", L"assets/interfaces/inven_quick.png", 1, 1);
}

void UIResourceSubManager::Preload_TopRightUI() {
    auto mgr = D2DImageManager::GetInstance();
    mgr->AddImage("stair_ico", L"assets/interfaces/stair_ico.png", 1, 1);
    mgr->AddImage("guidebook_btn", L"assets/interfaces/guidebook_btn.png", 1, 1);
    mgr->AddImage("menu_btn", L"assets/interfaces/menu_btn.png", 1, 1);
}

void UIResourceSubManager::Preload_InventoryUI()
{
    auto mgr = D2DImageManager::GetInstance();
    mgr->AddImage("inventory_bg", L"assets/interfaces/inventory_bg.png", 1, 1);
    mgr->AddImage("inventory_slot", L"assets/interfaces/inventory_slot.png", 1, 1);
    mgr->AddImage("default_btn", L"assets/interfaces/default_btn.png", 1, 1);
    mgr->AddImage("exit_btn", L"assets/interfaces/exit_btn.png", 1, 1);

}

void UIResourceSubManager::Preload_NinePatch()
{
    auto mgr = D2DImageManager::GetInstance();
    mgr->AddImage("chrome", L"assets/interfaces/chrome.png", 1, 1);
    mgr->RegisterFrame("chrome_tl", "chrome", { 0, 0, 6, 6 });
    mgr->RegisterFrame("chrome_t",  "chrome", { 6, 0, 14, 6 });
    mgr->RegisterFrame("chrome_tr", "chrome", { 14, 0, 20, 6 });
    
    mgr->RegisterFrame("chrome_l",  "chrome", { 0, 6, 6, 14 });
    mgr->RegisterFrame("chrome_c",  "chrome", { 6, 6, 14, 14 });
    mgr->RegisterFrame("chrome_r",  "chrome", { 14, 6, 20, 14 });
    
    mgr->RegisterFrame("chrome_bl", "chrome", { 0, 14, 6, 20 });
    mgr->RegisterFrame("chrome_b",  "chrome", { 6, 14, 14, 20 });
    mgr->RegisterFrame("chrome_br", "chrome", { 14, 14, 20, 20 });

    mgr->RegisterFrame("quest_tl", "chrome", {32, 32, 38, 43});
    mgr->RegisterFrame("quest_t", "chrome", {38, 32, 58, 43});
    mgr->RegisterFrame("quest_tr", "chrome", {58, 32, 64, 43});

    mgr->RegisterFrame("quest_l", "chrome", {32, 38, 38, 48});
    mgr->RegisterFrame("quest_c", "chrome", {38, 38, 58, 48});
    mgr->RegisterFrame("quest_r", "chrome", {58, 38, 64, 48});
    
    mgr->RegisterFrame("quest_bl", "chrome", {32, 48, 38, 59});
    mgr->RegisterFrame("quest_b", "chrome", {38, 48, 58, 59});
    mgr->RegisterFrame("quest_br", "chrome", {58, 48, 64, 59});
}

void UIResourceSubManager::Preload_MopHPUI()
{
    auto mgr = D2DImageManager::GetInstance();
    mgr->AddImage("mop_hpbar_bg", L"assets/interfaces/mop_hpbar_bg.png", 1, 1);
    mgr->AddImage("mop_hpbar_fill", L"assets/interfaces/mop_hpbar_fill.png", 1, 1);
}

void UIResourceSubManager::Preload_ItemData()
{
    auto mgr = D2DImageManager::GetInstance();
    auto img = mgr->AddImage("아이템", L"Image\\Items.png", 16, 32);
    itemDatas["체력포션"] = new ItemData {"체력포션", "HP를 소량 회복시켜주는 기능을 가진 아이템입니다.", img, 0, 21};
    itemDatas["weaponIcon"] = new ItemData {"무기아이콘", "", img, 1, 0};
    itemDatas["armorIcon"] = new ItemData {"갑옷아이콘", "", img, 2, 0};
    itemDatas["accIcon"] = new ItemData {"악세아이콘", "", img, 4, 0};
    
}

// ✅ 전체 리소스 초기화
void UIResourceSubManager::PreloadAll() {
    InitFontResources();                 // ⬅ 폰트 먼저
    Preload_StatusToolbar();
    Preload_QuickSlotToolbar();
    Preload_TopRightUI();
    Preload_InventoryUI();
    Preload_NinePatch();
    Preload_MopHPUI();
    Preload_ItemData();
}

const ItemData* UIResourceSubManager::GetItemData(std::string name)
{
    auto it = itemDatas.find(name);
    if (it != itemDatas.end())
    {
        return it->second;
    }
    return nullptr;
}
