#include "Animator.h"

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::Play(const string& animName)
{
    auto it = clips.find(animName);
    if (it == clips.end()) return;
    
    Clip& c = it->second;
    curFrame = c.startFrame;

    curClip = animName;
    elapsedTime = 0.f;
}

void Animator::Play(const string& animName, bool loop, function<void()> endFunc)
{
    auto it = clips.find(animName);
    if (it == clips.end()) return;

    Clip& c = it->second;
    c.loop = loop;
    c.onEndCallback = endFunc;
    curClip = animName;
    curFrame = c.startFrame;
    elapsedTime = 0.f;
}

void Animator::Update(float deltaTime)
{
    if (curClip.empty()) return;
    Clip& c = clips.at(curClip);
    elapsedTime += deltaTime;
    if (elapsedTime >= c.frameTime) 
    {
        elapsedTime = 0;
        ++curFrame;
        if (curFrame > c.endFrame) 
        {
            if (c.loop) 
            {
                curFrame = c.startFrame;
            }
            else 
            {
                curFrame = c.endFrame;
                if (c.onEndCallback) 
                {
                    c.onEndCallback();
                    //c.onEndCallback = nullptr; // 한 번만
                }
            }
        }
    }
}
