#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "ObserverEventData.h"
#include "../Entity.h"

class Player;

// 공통 엔티티 이벤트 옵저버
class IEntityObserver {
public:
    
    virtual void OnEntityDamageTaken(DamageTakenData data) {}
    virtual void OnEntityDied(Entity* entity) {}
    virtual ~IEntityObserver() = default;
};
