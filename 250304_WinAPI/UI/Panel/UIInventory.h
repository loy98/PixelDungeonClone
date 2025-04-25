#pragma once
#include "../Core/UIContainer.h"
#include "../Text/UITextBox.h"
#include "../Button/UIImageTextButton.h"
#include "../Core/UILayout.h"
#include "../VisualStyle.h"
#include "../Util/UIResourceSubManager.h"
#include "../Test/UITestEffectManager.h"
#include "../Util/UIHelper.h"
#include "../Image/UI9PatchImage.h"
#include "../UIData.h"
#include "../../Player.h"
#include "../../Inventory.h"
#include "../../Item.h"
#include "../../UIManager.h"
#include "../../CommonFunction.h"

class UIInventory : public UIContainer, public IEntityObserver {
private:
    UITextBox* titleText = nullptr;
    UIContainer* gridArea = nullptr;
    UIContainer* bottomSection = nullptr;
    // UIItemPanel* uiItemPanel = nullptr;

    vector<UIImageTextButton*> itemSlots;
    
    const int numCols = 5;
    const int numRows = 5;
    const D2D1_SIZE_F slotSize = {60.f,60.f};
    const float gridSpacing = 10.0f;
    const D2D1_RECT_F padding = {20,0,21, 0 };

public:
    void Init(const D2D1_RECT_F& area = { 200, 100, 578, 569 }) {
        SetRect(area);
        UIResourceSubManager::Preload_InventoryUI();

        AddBackGround();
        AddTitleSection();
        AddGridSection();
        AddBottomSection();
    }

    void UpdateItemSlot(int idx,
                         const UIInventorySlotData& data, const std::function<void()>& onClick)
    {
        UIHelper::UpdateInventorySlot(*itemSlots[idx], data, onClick);
    }

    int UseItemSlot(int idx)
    {
        UIManager::GetInstance()->UseUIItem(idx);
        return idx;
    }

    void OnChangePlayerInven(Player* entity) {
        if (!entity) return;

        auto inven = entity->GetInven();
        if (inven)
        {
            // for (int idx = 0; idx < 3; ++idx)
            // {
            //     auto slot = inven->GetSlot(idx);
            //
            //     if (slot.item)
            //     {
            //         
            //     }
            // }
            for (int idx = 0; idx < 25; ++idx)
            {
                auto slot = inven->GetSlot(idx);

                if (slot.item)
                {
                    UIManager::GetInstance()->SendLog(cp949_to_wstring(slot.item->GetName()), D2D1::ColorF(D2D1::ColorF::White));
                    auto* info = UIResourceSubManager::GetItemData(slot.item->GetName());
                    UpdateItemSlot(
                        idx,
                        UIInventorySlotData{ slot.item->GetName(), slot.item->GetImage(), slot.count, 0, info},
                        [this, idx]() {UseItemSlot(idx); }
                    );
                }
                else
                {
                    UpdateItemSlot(
                        idx,
                        UIInventorySlotData{ "", nullptr, 0, 0, nullptr},
                        [this, idx]() {UseItemSlot(idx); }
                    );
                }
            }
        }
    }

private:
    void AddBackGround() {
        UIResourceSubManager::Preload_NinePatch();
        NinePatchStyle defaultNinePatchStyle =
            UIHelper::CreateNinePatchFromSheet("chrome", "chrome", {6.0f, 6.0f});
        
        auto* inventoryBox = UIHelper::ApplyNinePathStyle(this, this->GetSizeRect(), defaultNinePatchStyle);
    }
    
    void AddTitleSection() {
        UIButtonStyle titleStyle = {
            {},  // 배경 없음
            { L"pixel", 18.0f, D2D1::ColorF::White, true,
              DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER }
        };

        titleText = new UITextBox();
        titleText->Init(titleStyle, L"배낭", { padding.left, 5, GetWidth(), 50 });
        AddChild(titleText);

        float imgSize = 30;
        D2D1_RECT_F dummyRect = { GetWidth() - padding.right - imgSize, 5, GetWidth() - padding.right, 5 + imgSize};
        UIIconStyle defaultIconStyle =
        {
        { D2DImageManager::GetInstance()->FindImage("exit_btn")},
        { D2DImageManager::GetInstance()->FindImage("")}
        };

        auto* exitBtn = UIHelper::ApplyIconStyle(this, dummyRect, defaultIconStyle);
        exitBtn->SetOnClick([this, exitBtn]() {

            this->SetActive(false);
            });
    }

    void AddGridSection() {
        auto* gridAreaTop = new UIContainer();
        gridAreaTop->SetRect({0, 50, GetWidth(), 110});
        gridAreaTop->SetLayout(new UIGridLayout(numCols, gridSpacing, gridSpacing, padding.left, padding.top));
        AddChild(gridAreaTop);
        int idx = 0;
        for (idx = 0; idx < 5; ++idx)
        {
            D2D1_RECT_F dummyRect = { 0, 0, slotSize.width, slotSize.height };
            UIInventorySlotStyle inventorySlotStyle =
                {
                ImageStyle { D2DImageManager::GetInstance()->FindImage("inventory_slot"), {0,0,0,0},1.0f, {0.5f,0.5f,0.5f,1.0f}},
                ImageStyle { nullptr },
                TextStyle { L"pixel", 24.0f, D2D1::ColorF(D2D1::ColorF::White), true, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR},
                TextStyle { L"pixel", 24.0f, D2D1::ColorF(D2D1::ColorF::White), true, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_FAR }
                };  // 예시 스타일
            auto* slot = UIHelper::ApplyInventorySlotStyle(gridAreaTop, dummyRect, inventorySlotStyle);

            slot->SetOnClick([this, idx, slot]()
            {
                UseItemSlot(idx);
            });

            // Magic Num (icon 기본 16px, slot 기본 64px이므로 x4 적용
            slot->GetImages()[1]->SetRect({ 0,0,16,16 });
            slot->GetImages()[1]->SetScale({4.f, 4.f});
            itemSlots.push_back(slot);
        }
        gridAreaTop->UpdateLayout();

        auto* weaponData = UIResourceSubManager::GetItemData("weaponIcon");
        auto* armorData = UIResourceSubManager::GetItemData("armorIcon");
        auto* accData = UIResourceSubManager::GetItemData("accIcon");
        itemSlots[0]->GetImages()[1]->SetImage(weaponData->origin, weaponData->frameX, weaponData->frameY);
        itemSlots[1]->GetImages()[1]->SetImage(armorData->origin, armorData->frameX, armorData->frameY);
        itemSlots[2]->GetImages()[1]->SetImage(accData->origin, accData->frameX, accData->frameY);
        
        gridArea = new UIContainer();
        gridArea->SetRect({ 0, 110, GetWidth(), 50 + (slotSize.width + gridSpacing) * numRows } );
        gridArea->SetLayout(new UIGridLayout(numCols, gridSpacing, gridSpacing, padding.left, padding.top));
        AddChild(gridArea);

        for (; idx < numCols * numRows; ++idx) {
            D2D1_RECT_F dummyRect = { 0, 0, slotSize.width, slotSize.height };
            UIInventorySlotStyle inventorySlotStyle =
                {
                    ImageStyle { D2DImageManager::GetInstance()->FindImage("inventory_slot"), },
                    ImageStyle { nullptr },
                    TextStyle { L"pixel", 24.0f, D2D1::ColorF(D2D1::ColorF::White), true, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR},
                    TextStyle { L"pixel", 24.0f, D2D1::ColorF(D2D1::ColorF::White), true, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_FAR }
                };  // 예시 스타일
            auto* slot = UIHelper::ApplyInventorySlotStyle(gridArea, dummyRect, inventorySlotStyle);

            slot->SetOnClick([this, idx, slot]()
            {
                UseItemSlot(idx);
            });

            // Magic Num (icon 기본 16px, slot 기본 64px이므로 x4 적용
            slot->GetImages()[1]->SetRect({ 0,0,16,16 });
            slot->GetImages()[1]->SetScale({4.f, 4.f});

            itemSlots.push_back(slot);
        }

        gridArea->UpdateLayout();
    }

    void AddBottomSection() {
        // const int col = 2;
        // const float padding = 10.0f;
        // const float width = (GetWidth() - padding * (col + 1)) / col;
        //
        // UIIconStyle defaultIconStyle =
        //     {
        //     { D2DImageManager::GetInstance()->FindImage("default_btn")},
        //     { D2DImageManager::GetInstance()->FindImage(""), {62,0,width-62,0 }}
        //     };
        //
        // bottomSection = new UIContainer();
        // bottomSection->SetRect({0, GetHeight() - 60, GetWidth(), GetHeight() });
        // bottomSection->SetLayout(new UIHorizontalLayout(9, 20));
        // AddChild(bottomSection);
        //
        //
        // D2D1_RECT_F dummyRect = { 0, 0, 164, 40 };
        //
        // auto* btn1 = UIHelper::ApplyIconStyle(bottomSection, dummyRect, defaultIconStyle);
        // btn1->SetOnClick([this, btn1]() {
        //     wstring debugString = (L"클릭 [BTN1] 클릭\n");
        //     UITestEffectManager::GetInstance()->AddEffect(debugString, btn1->GetWorldRect());
        //     });
        //
        // auto* btn2 = UIHelper::ApplyIconStyle(bottomSection, dummyRect, defaultIconStyle);
        // btn2->SetOnClick([this, btn2]() {
        //     wstring debugString = (L"클릭 [BTN2] 클릭\n");
        //     UITestEffectManager::GetInstance()->AddEffect(debugString, btn2->GetWorldRect());
        //     });
        //
        // bottomSection->UpdateLayout();
    }
};
