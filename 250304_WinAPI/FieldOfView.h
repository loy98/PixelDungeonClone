#pragma once

/* 검사진행 방향
  --->				   <---
  ㅡㅡㅡㅡㅡㅡ|ㅡㅡㅡㅡㅡㅡ
| |\     <1>  |  <2>     \| |
| |  \        |        \  | |
| |    \      |      \    | |
V |      \    |    \      | V
  |  <8>   \  |  \   <3>  |
  ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
  |  <7>     \|\     <4>  |
^ |        \  |  \        | ^
| |      \    |    \      | |
| |    \      |      \    | |
| |  \   <6>  |  <5>   \  |	|
  ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
  --->				   <---
*/

struct ScanDirection
{
	int horizonX;
	int verticalX;
	int horizonY;
	int verticalY;

	ScanDirection(int hX, int vX, int hY, int vY)
		: horizonX(hX), verticalX(vX), horizonY(hY), verticalY(vY) {}
};

class AstarScene;
class FieldOfView
{
public:
	ScanDirection scanDirections[8] =
	{
		{1, 0, 0, 1},
		{-1, 0, 0, 1},
		{0, 1, 1, 0},
		{0, 1, -1, 0},
		{-1, 0, 0, -1},
		{1, 0, 0, -1},
		{0, -1, 1, 0},
		{0, -1, -1, 0}
	};

	void Calculate(AstarScene& map, int row, float startSlope, float endSlope, ScanDirection direction);
};

