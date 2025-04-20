#include "FieldOfView.h"
#include "AstarScene.h"

void FieldOfView::Calculate(AstarTile map[][20], int tileIdX, int tileIdY, int checkRow, float startSlope, float endSlope, ScanDirection direction)
{
	// 한 분면 검사
	if (endSlope < startSlope)
		return;
	/*
	기울기
	(y1-y2) / (x1-x2)
	y값 증가량에 대한 x값을 알아야 하기 때문에 역수로 계산한다.(x = 1/m * y)
	*/
	float nextStartSlope = startSlope;

	// 시작점
	float oriCenterX = map[tileIdY][tileIdX].center.x;
	float oriCenterY = map[tileIdY][tileIdX].center.y;

	for (int dy = checkRow; dy <= ASTAR_TILE_COUNT; ++dy) // 탐색 단위
	{
		for (int dx = dy*nextStartSlope; dx >= 0; --dx) // 시작 기울기->끝기울기까지
		{
			// 타일 좌표
			int idX = tileIdX + dx + direction.horizonX + direction.horizonY;
			int idY = tileIdY + dy + direction.verticalX + direction.verticalY;

			// 접점 기울기
			float leftSlope = (oriCenterX - (idX * ASTAR_TILE_SIZE))/(oriCenterY-(idY *ASTAR_TILE_SIZE + ASTAR_TILE_SIZE));
			float rightSlope = (oriCenterX - (idX * ASTAR_TILE_SIZE+ ASTAR_TILE_SIZE / 2)) / (oriCenterY - (idY * ASTAR_TILE_SIZE));

			if (rightSlope > startSlope)
				continue;
			if (leftSlope < endSlope)
				break;

			AstarTile* tile = &map[idY][idX];

			if (!tile)
				continue;

			// 타일이 시야 내에 있는지 확인
			float centerSlope = (oriCenterX - tile->center.x) / (oriCenterY - tile->center.y);
			
			if (centerSlope > startSlope || centerSlope < endSlope)
				continue;

			// 타일이 벽이라면
			if (tile->GetType() == AstarTileType::Wall)
			{
				Calculate(map, tileIdX, tileIdY, checkRow + 1, nextStartSlope, leftSlope, direction);

				// 다음에도 벽이면 벽 아닐때까지 넘김
				while (tile->GetType() == AstarTileType::Wall)
				{
					--dx;
					tile = &map[idY][--idX];
				}

				// 벽이 아닌 타일로 새로운 시작 기울기를 구함.
				nextStartSlope = rightSlope;
			}

			// 벽은 처리 안됨
			tile->SetColor(RGB(0, 255, 0));
			tile->SetType(AstarTileType::Visible);
		}
	}
}
