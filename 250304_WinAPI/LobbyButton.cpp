#include "LobbyButton.h"
#include "D2DImageManager.h"
#include "D2DImage.h"
#include "FModSoundPlayer.h"
#include "MouseManager.h"
#include "CommonFunction.h"
#include "KeyManager.h"

HRESULT MapButton::Init(int posX, int posY)
{
	state = ButtonState::None;
	image = D2DImageManager::GetInstance()->AddImage("대쉬보드", L"Image/dashboard.png", 4, 1);
	if (image == nullptr)
	{
		return E_FAIL;
	}

	pos.x = posX;
	pos.y = posY;

	rc.left = pos.x;
	rc.top = pos.y;
	rc.right = rc.left + 96;
	rc.bottom = rc.top + 96;

	//mainRc.left = dashboardRc.left;
	//mainRc.top = dashboardRc.top;
	//mainRc.right = mainRc.left + (32 * 3);
	//mainRc.bottom = mainRc.top + (32 * 3);

	buttonFunc = nullptr;

	return S_OK;
}

void MapButton::Release()
{
}

void MapButton::Update()
{
	if (PtInRect(&rc, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
		{
			state = ButtonState::Down;
		}
		else if (state == ButtonState::Down &&
			KeyManager::GetInstance()->IsOnceKeyUp(VK_LBUTTON))
		{
			state = ButtonState::Up;
			FModSoundPlayer::GetInstance()->Play("click", 0.3f);
			// 기능 수행
			// TilemapTool::Save()
			//if (buttonFunc && obj)	obj->buttonFunc();
			if (buttonFunc) buttonFunc();
		}
	}
	else
	{
		state = ButtonState::None;
	}
}

void MapButton::Render(HDC hdc)
{
	//RenderRect(hdc, rc);

	switch (state)
	{
	case ButtonState::None:

	case ButtonState::Up:
		image->RenderFrameScale(pos.x, pos.y, 3, 3, 0, 0);
		
		break;
	case ButtonState::Down:
		image->RenderFrameScale(pos.x, pos.y, 3, 3, 0, 0, 0, 0, 0., 0.8f);
		break;
	}
}

HRESULT ExitButton::Init(int posX, int posY)
{
	return E_NOTIMPL;
}

void ExitButton::Release()
{
}

void ExitButton::Update()
{
}

void ExitButton::Render(HDC hdc)
{
}
