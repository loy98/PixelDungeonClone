#include "FieldOfView.h"
#include "AstarScene.h"
#include <cmath>

void FieldOfView::Calculate(AstarTile(&map)[20][20], int tileIdX, int tileIdY, int checkRow, float startSlope, float endSlope, ScanDirection direction)
{
	int b = 0;
	// 한 분면 검사
	if (endSlope > startSlope)
		return;
	int a = 0;
	/*
	기울기
	(y1-y2) / (x1-x2)
	y값 증가량에 대한 x값을 알아야 하기 때문에 역수로 계산한다.(x = 1/m * y)
	*/
	float nextStartSlope = startSlope;
	//float nextEndSlope = endSlope;

	// 시작점
	//float oriCenterX = map[tileIdY][tileIdX].center.x;
	//float oriCenterY = map[tileIdY][tileIdX].center.y;

	bool isBlock = false;

	// dy 맵 크기를 벗어나면 나올 수 없는 범위의 좌표가 나옴->왼쪽부터 차례대로...
	for (int dy = checkRow; dy < ASTAR_TILE_COUNT; dy++) // 탐색 단위
	{
		for (int dx = ceil((float)dy * nextStartSlope); dx >= 0; dx--) // 시작 기울기->끝기울기까지
		{
			// 타일 좌표
			int idX = tileIdX + (-dx * direction.horizonX) + (dy * direction.horizonY);
			int idY = tileIdY + (-dx * direction.verticalX) + (dy * direction.verticalY);

			if (idX < 0 || idX >= ASTAR_TILE_COUNT || idY < 0 || idY >= ASTAR_TILE_COUNT)
				continue;
	
			// 타일이 시야 내에 있는지 확인
			float centerSlope = (float)dx / (float)dy;
			// 접점 기울기-dy에 -+0.5 left, right 바꾸면 다르게 나옴 
			float leftSlope = ((float)dx + 0.5f) / ((float)dy + 0.5f);
			float rightSlope = ((float)dx - 0.5f) / ((float)dy - 0.5f);

			// 시작기울기보다 크거나 같다면-startslope이 고정되어있음;; 수정완료
			if (rightSlope - startSlope >= FLT_EPSILON)
			{
				// 센터가 시작기울기보다 크다면
				if (centerSlope - startSlope > FLT_EPSILON)
				{
					continue;
				}
			}
	
			if (leftSlope - endSlope <= FLT_EPSILON)
			{
				if (centerSlope - endSlope <= FLT_EPSILON)
				{
					break;
				}
			}
				

			// centerSlope이 시작기울기와 끝기울기에 겹쳤거나 벗어났다면
			/*if ((centerSlope - startSlope >= FLT_EPSILON) || (centerSlope - endSlope <= FLT_EPSILON)) {
				if (rightSlope - startSlope >= FLT_EPSILON)
					continue;

				if (leftSlope - endSlope <= FLT_EPSILON)
					break;
			*/

			AstarTile* tile ;
			tile = &map[idY][idX];

			if (!tile)
				continue;
	
			tile->isVisible = true;

			// 타일이 벽이라면
			if (isBlock)
			{
				if (tile->GetType() == AstarTileType::Wall)
				{
					nextStartSlope = rightSlope;

					if (dx == 0)
					{
						return;
					}
					continue;
				}
				else
				{
					// dx == 0이면 rightSlope이 마이너스. start가 end보다 작기 때문에 종료된다.
					if (dx == 0)
					{
						nextStartSlope = 0.0f;
					}
					else
					{
						nextStartSlope = rightSlope;
					}
					startSlope = nextStartSlope;
					isBlock = false;
				}
			}
			else
			{
				if (tile->GetType() == AstarTileType::Wall)
				{	

					Calculate(map, tileIdX, tileIdY, dy + 1, nextStartSlope, leftSlope, direction);
					
					if (dx == 0)
					{
						return;
					}

					/*매 타일마다 초기화해야 다음 행에서 재귀탐색한 영역까지 재탐색하지 않음*/
					nextStartSlope = rightSlope;
					isBlock = true;
					continue;
				}
			}
			tile->SetColor(RGB(0, 255, 0));
		}
	}
}
