#pragma once
#include "config.h"	// ScanDirection
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

class AstarTile;
class FieldOfView
{
public:

	// 2차원 배열을 넘기려면 이렇게 넘겨야 함... 마음에 들지 않음
	void Calculate(AstarTile(&map)[20][20], int tileIdX, int tileIdY, int checkRow, float startSlope, float endSlope, ScanDirection direction);
};

