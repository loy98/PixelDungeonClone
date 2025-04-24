#pragma once
#include "config.h"

struct Clip {
    int   startFrame;
    int   endFrame;
    float frameTime;
    bool  loop;
    function<void()> onEndCallback; // 비루프 모드일 때 마지막 프레임 이후 호출
};
class Animator
{
private:
    float elapsedTime;
    int curFrame;
    string curClip;
    unordered_map<string, Clip> clips;
public:
    Animator();
    ~Animator();

    void AddClip(const string& animName, Clip clip) { clips[animName] = clip; }

    void Play(const string& animName);
    void Play(const string& animName, bool loop, function<void()> endFunc);
    void Update(float deltaTime);

    int GetCurFrame() { return curFrame; }
    bool isPlaying() const { return !curClip.empty(); }

};

