#include "BattleScene.h"
#include "CommonFunction.h"
#include "config.h"
#include "D2DImage.h"
#include "D2DImageManager.h"

HRESULT BattleScene::Init()
{
    SetClientRect(g_hWnd, WINSIZE_X, TILEMAPTOOL_Y);
    sampleTile = D2DImageManager::GetInstance()->AddImage("sample_tile", L"Image/tiles_sewers.png", SAMPLE_TILE_X, SAMPLE_TILE_Y);
    Load();

    test = D2DImageManager::GetInstance()->AddImage("banner", L"Image/banners.png", 2, 4);


    return S_OK;
}

void BattleScene::Release()
{
}

void BattleScene::Update()
{
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
    {
        SceneManager::GetInstance()->ChangeScene("전투씬_1");
    }
}

void BattleScene::Render(HDC hdc)
{
    for (auto& info : tileInfo)
    {
        sampleTile->RenderFrame(static_cast<int>(info.rc.left),
                                static_cast<int>(info.rc.top), info.frameX,
                                info.frameY);
        sampleTile->DrawRect({ (float)info.rc.left, (float)info.rc.top},{(float)info.rc.right, (float)info.rc.bottom},0,1);
    }
    test->Middle_RenderFrame(WINSIZE_X / 2, WINSIZE_Y / 2, 0, 3, DEG_TO_RAD(135), false, false, 0.5f);
}

void BattleScene::Load()
{
    HANDLE hFile = CreateFile(
        L"TileMapData.dat", GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(g_hWnd, TEXT("파일 열기 실패"), TEXT("경고"), MB_OK);
        return;
    }
    DWORD dwByte = 0;
    if (!ReadFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL))
    {
        MessageBox(g_hWnd, TEXT("파일 읽기 실패"), TEXT("경고"), MB_OK);
    }
    CloseHandle(hFile);
}
