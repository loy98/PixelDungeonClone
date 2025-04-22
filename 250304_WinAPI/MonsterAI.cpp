#include "MonsterAI.h"
#include "Monster.h"

void MonsterAI::Act(Level* level, Monster* monster)
{
	// Idle일 때만 받음
	// 시야에 보이는지-시야 범위 있어야 함.
}


// 임시
void MonsterAI::SetFov(Level* level, Monster* monster)
{
    // 적의 시야를 가져오는 함수
    if (!fovList.empty())
    {
        fovList.clear();
    }

    int dx[5] = { -2, -1, 0, 1, 2 };
    int dy[5] = { -2, -1, 0, 1, 2 };
    // 주변 24방향 탐색

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int nx = monster->GetPosition().x + TILE_SIZE * dx[j];
            int ny = monster->GetPosition().x + TILE_SIZE * dy[i];

            fovList.push_back({nx, ny});
        }
    }
}