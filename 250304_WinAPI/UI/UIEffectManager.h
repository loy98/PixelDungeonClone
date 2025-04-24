#pragma once
#include "UI/Text/UITextEffect.h"
#include <vector>

#include "../Observer/IEntityObserver.h"

class UIEffectManager : public IEntityObserver
{
private:
    Camera* camera{nullptr};
    
    std::vector<UITextEffect*> effects;
    D2D1_SIZE_F offset = {-15.f, -30.f};

    TextStyle defaultTextStyle = {
        L"pixel", 16.0f, D2D1::ColorF(D2D1::ColorF::White), true,
         DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER
    };

    EffectStyle defaultEffectStyle =
    {
        2.0f, // duration
        -20.0f // y movespeed value
    };

public:
    ~UIEffectManager() override;

    void AddEffect(const std::wstring& text, const D2D1_RECT_F& rect)
    {
        auto* fx = new UITextEffect();
        fx->SetWorldRect(rect);
        fx->Init(text, defaultTextStyle, defaultEffectStyle);
        effects.push_back(fx);
    }

    void AddEffect(const std::wstring& text, const TextStyle& style, const D2D1_RECT_F& rect, const EffectStyle& effectStyle) {
        auto* fx = new UITextEffect();
        fx->SetWorldRect(rect);
        fx->Init(text, style, effectStyle);
        effects.push_back(fx);
    }

    void Update(float dt) {
        for (auto it = effects.begin(); it != effects.end(); ) {
            if ((*it)->IsDead()) {
                delete *it;
                it = effects.erase(it);
            }
            else {
                (*it)->Update(dt);
                ++it;
            }
        }
    }

    void Render(ID2D1HwndRenderTarget* rt) {
        for (auto* fx : effects) {
            if (fx)
                fx->Render(rt);
        }
    }

    void Clear() {
        for (auto* fx : effects) {
            delete fx;
        }
        effects.clear();
    }

    void SetCamera(Camera* camera)
    {
        if (camera)
        {
            this->camera = camera;
        }
    }

    
    /* Observer */
    void OnEntityDamageTaken(DamageTakenData data) override
    {
        if (!camera)
        {
            OutputDebugStringA("카메라없어요~~~, by 이펙트매니저");
            return;
        }
        
        if (data.sendText == L"") return;

        if (!data.textStyle)
        {
            data.textStyle = &defaultTextStyle;
        }
        if (!data.effectStyle)
        {
            data.effectStyle = &defaultEffectStyle;
        }

        float width = 30.0f;
        float height = 30.0f;
        FPOINT scrPos = {camera->ConvertToRendererX(data.pos.x), camera->ConvertToRendererY(data.pos.y)};
        D2D1_RECT_F setRect = {scrPos.x + offset.width, scrPos.y + offset.height,
            scrPos.x + width + offset.width, scrPos.y + height + offset.height};
        
        AddEffect(data.sendText, *data.textStyle, setRect, *data.effectStyle);
    }
};

inline UIEffectManager::~UIEffectManager()
{
    for (auto* fx : effects) {
        delete fx;
    }
    effects.clear();
}
