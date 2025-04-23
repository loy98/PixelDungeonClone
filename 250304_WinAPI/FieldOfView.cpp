#include "FieldOfView.h"
#include <cmath>
#include "CommonFunction.h"

void FieldOfView::Calculate(Map(&map)[TILE_Y][TILE_X], int viewRange, int tileIdX, int tileIdY, int checkRow, float startSlope, float endSlope, ScanDirection direction)
{
	// 한 분면 검사
	/*
	기울기
	(y1-y2) / (x1-x2)
	y값 증가량에 대한 x값을 알아야 하기 때문에 역수로 계산한다.(x = 1/m * y)
	*/

	if (viewRange <= 0)
	return;

	RECT viewRc;
	RECT tileRc;

	if (endSlope > startSlope) return;

	for (int depth = checkRow; depth < TILE_Y; ++depth) {
		float nextStart = startSlope;
		bool blocked = false;
		int dy = -depth;

		for (int dx = -depth; dx <= 0; ++dx) {
			// 1) 슬로프 계산
			float leftSlope = (dx - 0.5f) / (dy + 0.5f);
			float rightSlope = (dx + 0.5f) / (dy - 0.5f);
			if (rightSlope > startSlope) continue;
			if (leftSlope < endSlope)      break;

			// 2) 월드 좌표 변환
			int idX = tileIdX + dx * direction.horizonX + dy * direction.horizonY;
			int idY = tileIdY + dx * direction.verticalX + dy * direction.verticalY;
			if (idX < 0 || idX >= TILE_X || idY < 0 || idY >= TILE_Y)
				continue;

			Map& tile = map[idY][idX];

			viewRc = GetRectAtCenter(tileIdX, tileIdY, viewRange * TILE_SIZE, viewRange * TILE_SIZE);
			tileRc = GetRectAtCenter(idX, idY, TILE_SIZE, TILE_SIZE);

			if (!RectInRect(viewRc, tileRc))
			{
				continue;
			}

			// 3) 비차단 타일 표시
			if (!blocked && tile.type != 0) {
				tile.visible = true;
				//tile.SetColor(RGB(0, 255, 0));
			}

			// 4) 벽 발견 시 재귀 분기
			if (!blocked && tile.type == 0) {
				blocked = true;
				Calculate(map, 0, tileIdX, tileIdY,
					depth + 1, startSlope, leftSlope, direction);
				nextStart = rightSlope;
			}
			// 5) 그림자 끝 지점 복귀
			else if (blocked && tile.type == 0) {
				blocked = false;
				startSlope = nextStart;
				tile.visible = true;
				//tile.SetColor(RGB(0, 255, 0));
			}
		}

		// 6) 행 전체가 차단되었으면 종료
		if (blocked) break;
	}



	//if (viewRange <= 0)
	//	return;

	//if (endSlope > startSlope)
	//	return;
	//
	//float nextStartSlope = startSlope;
	//bool isBlock = false;

	//RECT viewRc;

	//viewRc = GetRectAtCenter(tileIdX, tileIdY, viewRange, viewRange);

	//// dy 맵 크기를 벗어나면 나올 수 없는 범위의 좌표가 나옴->왼쪽부터 차례대로...
	//for (int dy = checkRow; dy < TILE_Y; dy++) // 탐색 단위
	//{
	//	for (int dx = ceil((float)dy * nextStartSlope); dx >= 0; dx--) // 시작 기울기->끝기울기까지
	//	{
	//		// 타일 좌표
	//		int idX = tileIdX + (-dx * direction.horizonX) + (dy * direction.horizonY);
	//		int idY = tileIdY + (-dx * direction.verticalX) + (dy * direction.verticalY);

	//		if (idX < 0 || idX >= TILE_X || idY < 0 || idY >= TILE_Y)
	//			continue;
	//
	//		Map* tile;
	//		tile = &map[idY][idX];

	//		if (!tile)
	//			continue;

	//		RECT tileRc;
	//		tileRc = GetRectAtCenter(idX, idY, TILE_SIZE, TILE_SIZE);

	//		// 타일이 시야 내에 있는지 확인
	//		float centerSlope = (float)dx / (float)dy;
	//		// 접점 기울기-dy에 -+0.5 left, right 바꾸면 다르게 나옴 
	//		float leftSlope = ((float)dx + 0.5f) / ((float)dy + 0.5f);
	//		float rightSlope = ((float)dx - 0.5f) / ((float)dy - 0.5f);

	//		if (rightSlope > startSlope)
	//		{
	//			continue;
	//		}
	//		else if (leftSlope < endSlope)
	//		{
	//			break;
	//		}
	//		

	//		if (centerSlope > startSlope)
	//		{
	//			continue;
	//		}

	//		if (centerSlope < endSlope)
	//		{
	//			break;
	//		}

	//		// 타일의 leftSlope은 dy+1 타일의 rightSlope과 같다.-시야처리하면 안됨 몰겟다
	//		if (endSlope == rightSlope)
	//		{
	//			//endSlope = leftSlope;
	//			endSlope = centerSlope;
	//			break;
	//		}
	//
	//		if (RectInRect(viewRc, tileRc))
	//		{
	//			tile->visible = true;

	//		}

	//		// 타일이 벽이라면
	//		if (isBlock)
	//		{
	//			//TODO: convert raw value to enum
	//			if (tile->type == 0)
	//			{
	//				if (dx == 0)
	//				{
	//					return;
	//				}
	//				nextStartSlope = rightSlope;
	//				continue;
	//			}
	//			else
	//			{
	//				// dx == 0이면 rightSlope이 마이너스. start가 end보다 작기 때문에 종료된다.
	//				if (dx == 0)
	//				{
	//					nextStartSlope = 0.0f;
	//				}
	//				else
	//				{
	//					nextStartSlope = rightSlope;
	//				}
	//				startSlope = nextStartSlope;
	//				isBlock = false;
	//			}
	//		}
	//		else
	//		{
	//			if (tile->type == 0)
	//			{	
	//				Calculate(map, viewRange, tileIdX, tileIdY, dy + 1, nextStartSlope, leftSlope, direction);

	//				if (dx == 0)
	//				{
	//					return;
	//				}
	//				isBlock = true;
	//				continue;
	//			}
	//		}
	//		// tile->SetColor(RGB(0, 255, 0));
	//	}
	//}
}