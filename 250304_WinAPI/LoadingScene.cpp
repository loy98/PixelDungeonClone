#include "LoadingScene.h"

#include "D2DImage.h"
#include "D2DImageManager.h"
// #include "Image.h"

HRESULT LoadingScene::Init()
{
    bg = D2DImageManager::GetInstance()->AddImage("·Îµù_1", L"Image/loading.bmp");
    
    
    return S_OK;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
}

void LoadingScene::Render(HDC hdc)
{
    if(bg)
    {
        bg->Render(0,0);
    }
}
