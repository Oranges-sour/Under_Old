#include "EnemyHitPointBar.h"

EnemyHitPointBar* EnemyHitPointBar::create(basic_Enemy* enemy)
{
	auto b = new (std::nothrow)EnemyHitPointBar();
	if (b && b->init(enemy)) {
		b->autorelease();
		return b;
	}
	CC_SAFE_DELETE(b);
	return nullptr;
}

bool EnemyHitPointBar::init(basic_Enemy* enemy)
{
	return false;
}
