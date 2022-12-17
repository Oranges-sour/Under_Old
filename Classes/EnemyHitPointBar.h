#ifndef __ENEMY_HITPOINT_BAR_H__
#define __ENEMY_HITPOINT_BAR_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Enemy.h"

class EnemyHitPointBar : public Node {
public:
    static EnemyHitPointBar* create(basic_Enemy* enemy);
    bool init(basic_Enemy* enemy);
    //EnemyHitPointBar();
    virtual ~EnemyHitPointBar() {}

private:
    DrawNode* drawNode = nullptr;
    basic_Enemy* enemy = nullptr;
};

#endif