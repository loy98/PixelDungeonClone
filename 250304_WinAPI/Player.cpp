#include "Player.h"
#include "Map.h"

void Player::Move(FPOINT delta, Map* map)
{
    if (delta.x < 0 || delta.x > map->GetWidth() || delta.y < 0 || delta.y > map->GetHeight())
    {
        position.x = delta.x;
        position.y = delta.y;
    }
}
