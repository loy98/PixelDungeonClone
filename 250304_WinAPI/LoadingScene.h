#pragma once
#include "GameObject.h"

// class Image;
class D2DImage;
class LoadingScene: public GameObject
{
private:
	D2DImage* bg;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual ~LoadingScene() {};
};

