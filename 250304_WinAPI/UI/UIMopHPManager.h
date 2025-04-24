#pragma once
#include "../Entity.h"
#include "Bar/UIMopBar.h"
#include "Util/UIHelper.h"

class UIMopHPManager
{
protected:
    vector<UIMopBar*> uiMopHPBars;

    ImageStyle defulatMopHPBarBgStyle;
    BarStyle defaultMopHPBarStyle;
    
public:
    UIMopHPManager() = default;
    ~UIMopHPManager();

    void Init()
    {
        SetDefaultStyle();
    }
    void Update(float dt)
    {
        for (auto* bar : uiMopHPBars) {
            if (bar)
                bar->Update(dt);
        }
    }
        
    void Render(ID2D1HwndRenderTarget* rt) {
        for (auto* bar : uiMopHPBars) {
            if (bar)
                bar->Render(rt);
        }
    }

    void Release()
    {
        for (auto* bar : uiMopHPBars)
        {
            if (bar)
            {
                delete bar;
                bar = nullptr;
            }
        }
    }
    
    UIMopBar* CreateMopHPBar(Entity* entity, Camera* camera)
    {
        auto hpBar = new UIMopBar();
        auto entityPos = entity->GetPosition();
        auto entityRect = {entity};
        hpBar->Init();
        hpBar->SetStyle(defulatMopHPBarBgStyle, defaultMopHPBarStyle);
        hpBar->SetMaxValue(entity->GetMaxHP());
        hpBar->SetValue(entity->GetHP());
        hpBar->SetTarget(entity, camera);
        uiMopHPBars.push_back(hpBar);
        return hpBar;
    }
    
    void ChangeZoomScale(float zoomScale)
    {
        for (auto bar : uiMopHPBars)
        {
            if (!bar) continue;
            bar->SetScale({zoomScale, zoomScale});
        }
    }

protected:
    void SetDefaultStyle()
    {
        defulatMopHPBarBgStyle = ImageStyle
        {
            D2DImageManager::GetInstance()->FindImage(("mop_hpbar_bg")),
            {0,0,0,0},
            1.0f
        };
        defaultMopHPBarStyle = BarStyle
        {
            ImageStyle {D2DImageManager::GetInstance()->FindImage("mop_hpbar_fill")},
            ImageStyle{ nullptr }
        };
    }
};
