#include "FieldOfView.h"
#include "AstarScene.h"

void FieldOfView::Calculate(AstarTile(&map)[20][20], int tileIdX, int tileIdY, int checkRow, float startSlope, float endSlope, ScanDirection direction)
{
	int b = 0;
	// 한 분면 검사
	if (endSlope > abs(startSlope))
		return;
	int a = 0;
	/*
	기울기
	(y1-y2) / (x1-x2)
	y값 증가량에 대한 x값을 알아야 하기 때문에 역수로 계산한다.(x = 1/m * y)
	*/
	float nextStartSlope = startSlope;

	// 시작점
	float oriCenterX = map[tileIdY][tileIdX].center.x;
	float oriCenterY = map[tileIdY][tileIdX].center.y;

	bool isBlock = false;

	// dy 범위 매직넘버- 맵 크기를 벗어나면 나올 수 없는 범위의 좌표가 나옴->왼쪽부터 차례대로...
	for (int dy = checkRow; dy < 10; dy++) // 탐색 단위
	{
		for (int dx = dy * nextStartSlope; dx >= 0; dx--) // 시작 기울기->끝기울기까지
		{
			// 타일 좌표
			int idX = tileIdX + (-dx * direction.horizonX) + (dy * direction.horizonY);
			int idY = tileIdY + (-dx * direction.verticalX) + (dy * direction.verticalY);

			// 접점 기울기
	/*	float leftSlope = abs((oriCenterX - (idX * ASTAR_TILE_SIZE)) / (oriCenterY - (idY * ASTAR_TILE_SIZE + ASTAR_TILE_SIZE)));
		float rightSlope = abs((oriCenterX - (idX * ASTAR_TILE_SIZE + ASTAR_TILE_SIZE / 2)) / (oriCenterY - (idY * ASTAR_TILE_SIZE)));*/
		float leftSlope = (dx -0.5f) / (dy + 0.5f);
		float rightSlope = (dx + 0.5f) / (dy - 0.5f);

			if (rightSlope > startSlope)
				continue;
			if (leftSlope < endSlope)
				break;
			
			AstarTile* tile = &map[idY][idX];

			if (!tile)
				continue;

			// 타일이 시야 내에 있는지 확인
			float centerSlope = dx/dy;
			
			if (centerSlope > startSlope || centerSlope < endSlope)
				continue;

			tile->isVisible = true;

			// 타일이 벽이라면
			if (tile->GetType() == AstarTileType::Wall)
			{
				Calculate(map, tileIdX, tileIdY, checkRow + 1, nextStartSlope, centerSlope, direction);
				isBlock = true;
				// 다음에도 벽이면 벽 아닐때까지 넘김
			}

			if (isBlock && tile->GetType() == AstarTileType::Wall)
				continue;
			else if (isBlock && tile->GetType() != AstarTileType::Wall)
			{
				isBlock = false;

				// 벽이 아닌 타일로 새로운 시작 기울기를 구함.
				nextStartSlope = centerSlope;
			}

			tile->SetColor(RGB(0, 255, 0));
			int a = 0;
		}
	}
}
