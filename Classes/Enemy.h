#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
USING_NS_CC;

#include "basic_GameSprite.h"
#include "Weapon.h"

class basic_Enemy : public basic_GameSprite
{
public:
	/**
	*设置血量
	*@param hitpoint: 血量
	*@return 无
	*/
	void setHitPoint(int hitpoint);

	/**
	*获得血量
	*@param 无
	*@return 血量
	*/
	int getHitPoint();


private:
	int hitpoint = 0;
};

#endif