#pragma once
#include "../Entity.h"
#include "Bar/UIMopBar.h"
#include "Util/UIHelper.h"

class UIMopHPManager
{
protected:
    unordered_map<Entity*, UIMopBar*> uiMopHPBars;

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
        for (auto& [entity, bar] : uiMopHPBars)
        {
            if (bar)
                bar->Update(dt);
        }
    }
        
    void Render(ID2D1HwndRenderTarget* rt) {
        for (auto& [entity, bar] : uiMopHPBars)
        {
            if (bar)
                bar->Render(rt);
        }
    }

    void Release()
    {
        for (auto& [entity, bar] : uiMopHPBars)
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
        if (!entity || !camera ) return nullptr;
        if (entity->GetType() != EntityType::MONSTER) return nullptr;
        
        auto hpBar = new UIMopBar();
        auto entityPos = entity->GetPosition();
        auto entityRect = {entity};
        hpBar->Init();
        hpBar->SetStyle(defulatMopHPBarBgStyle, defaultMopHPBarStyle);
        hpBar->SetMaxValue(entity->GetMaxHP());
        hpBar->SetValue(entity->GetHP());
        hpBar->SetTarget(entity, camera);
        hpBar->SetActive(false);
        uiMopHPBars.insert({entity, hpBar});
        return hpBar;
    }

    UIMopBar* FindMopBar(Entity* entity)
    {
        auto it = uiMopHPBars.find(entity);
        if (it != uiMopHPBars.end())
        {
            return it->second;
        }
    }

    bool RemoveMopBar(Entity* entity)
    {
        auto it = uiMopHPBars.find(entity);
        if (it != uiMopHPBars.end())
        {
            delete it->second;
            it = uiMopHPBars.erase(it);
            return true;
        }
        return false;
    }

    void DetachMopBar(Entity* entity)
    {
        auto bar = FindMopBar(entity);
        entity->GetEntityObserverHub().RemoveObserver(bar);
        RemoveMopBar(entity);
    }
    
    void ChangeZoomScale(float zoomScale)
    {
        for (auto& [entity, bar] : uiMopHPBars)
        {
            if (bar)
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
