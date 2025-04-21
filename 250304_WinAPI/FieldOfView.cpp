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
	float nextEndSlope = endSlope;

	// 시작점
	float oriCenterX = map[tileIdY][tileIdX].center.x;
	float oriCenterY = map[tileIdY][tileIdX].center.y;

	bool isBlock = false;

	// dy 맵 크기를 벗어나면 나올 수 없는 범위의 좌표가 나옴->왼쪽부터 차례대로...
	for (int dy = checkRow; dy < ASTAR_TILE_COUNT; dy++) // 탐색 단위
	{
		for (int dx = ceil((float)dy * nextStartSlope); dx >= 0; dx--) // 시작 기울기->끝기울기까지
		{
			// 타일 좌표
			int idX = tileIdX + (-dx * direction.horizonX) + (dy * direction.horizonY);
			int idY = tileIdY + (-dx * direction.verticalX) + (dy * direction.verticalY);

			if (idX < 0 || idX >= ASTAR_TILE_COUNT || idY < 0 || idY >= ASTAR_TILE_COUNT) {
				
				break;
			}
			AstarTile* tile = &map[idY][idX];
			//tile->SetColor(RGB(255, 255, 255));

			// 접점 기울기-dy에 -+0.5 left, right 바꾸면 다르게 나옴 
			float leftSlope = ((float)dx +0.5f) / ((float)dy + 0.5f);
			float rightSlope = ((float)dx - 0.5f) / ((float)dy - 0.5f);

			if (rightSlope - startSlope > FLT_EPSILON) {

				continue;
			}

			if (leftSlope - nextEndSlope < FLT_EPSILON){
				tile = &map[idY][idX];
				//tile->SetColor(RGB(0, 255, 255));
				break;
			}
			
			tile = &map[idY][idX];

			if (!tile)
				continue;

			// 타일이 시야 내에 있는지 확인
			float centerSlope = (float)dx / (float)dy;
			
			if (centerSlope - startSlope > FLT_EPSILON ) {
				if(centerSlope - endSlope < FLT_EPSILON){
					break;
				}
				else {
					continue;
				}
				//tile->SetColor(RGB(255, 255, 0));
				
			}
	

			tile->isVisible = true;

			// 타일이 벽이라면
			if (isBlock)
			{
				if (tile->GetType() == AstarTileType::Wall)
				{
					nextStartSlope = rightSlope;
					
					continue;
				}
				else
				{
					isBlock = false;
				}
			}
			else
			{
				if (tile->GetType() == AstarTileType::Wall)
				{
					if (dx > 0) {
						isBlock = true;
					}
					
					Calculate(map, tileIdX, tileIdY, dy + 1, nextStartSlope, leftSlope, direction);
					
					nextStartSlope = rightSlope;
					nextEndSlope = leftSlope;	// 기울기 0에 벽 있을 때 뒤에 안나오긴 하는데 부자연스러움.
					continue;
				}
			}

			tile->SetColor(RGB(0, 255, 0));
			int a = 0;
		}
	}
}
