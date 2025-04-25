#include "LobbyBackground.h"
#include "D2DImageManager.h"
#include "D2DImage.h"
#include "TimerManager.h"
#include "CommonFunction.h"

void LobbyBackground::Init()
{
	//SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	currentGround = D2DImageManager::GetInstance()->AddImage("로비전경", L"Image/arcsForeground.png");
	nextGround = D2DImageManager::GetInstance()->FindImage("로비전경");
	currentUnderground = D2DImageManager::GetInstance()->AddImage("로비후경", L"Image/arcsBackground.png");
	nextUnderground = D2DImageManager::GetInstance()->FindImage("로비후경");

	groundMoveSpeed = 60.0f;
	undergroundMoveSpeed = 20.0f;

	groundCameraY = WINSIZE_Y;
	undergroundCameraY = WINSIZE_Y;
}

void LobbyBackground::Release()
{
}

void LobbyBackground::Update()
{
	time = TimerManager::GetInstance()->GetDeltaTime();
	groundCameraY -= groundMoveSpeed * time;
	undergroundCameraY -= undergroundMoveSpeed * time;	// test

	// 스왑
	if (groundCameraY < 0.0f)     // 첫번째 이미지 다 지나감, 두번째 이미지 WINSIZE_Y만큼 출력
	{
		groundCameraY = WINSIZE_Y;
		swap(currentGround, nextGround);
	}

	if (undergroundCameraY < 0.0f)     // 첫번째 이미지 다 지나감, 두번째 이미지 WINSIZE_Y만큼 출력
	{
		undergroundCameraY = WINSIZE_Y;
		swap(currentUnderground, nextUnderground);
	}
}

void LobbyBackground::Render(HDC hdc)
{ 
	currentUnderground->Middle_Render(0, undergroundCameraY - WINSIZE_Y);
	nextUnderground->Middle_Render(0, undergroundCameraY);

	currentGround->Middle_Render(0,groundCameraY - WINSIZE_Y);
	nextGround->Middle_Render(0, groundCameraY);
}
