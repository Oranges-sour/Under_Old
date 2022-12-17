#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"
USING_NS_CC;

#include "GameItem.h"

class basic_Weapon : public basic_GameItem {
public:
    basic_Weapon() {}
    virtual ~basic_Weapon() {}

    /**
     *使用物品
     *@param gameSprite: 使用武器的精灵
     *@param from: 起始点坐标
     *@param to: 终点坐标
     *@return 无
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to) = 0;

private:
    /**
     *最原始的使用物品,在weapon中被屏蔽
     *@param 略
     *@return 略
     */
    virtual void useItem(basic_GameSprite* gameSprite) final override {}
};

//近程武器
class NearWeapon : public basic_Weapon {
public:
    /**
     *使用武器
     *@param gameSprite: 使用武器的精灵
     *@param from: 起始点坐标
     *@param to: 终点坐标(仅仅表示一个使用的方向
     *@param range: 武器伤害半径
     *@param angle: 半圆的张角
     *@param holdTime: 蓄力时间
     *@return 无
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to, float range, float angle,
        float holdTime = 0.0f)
    {
    }

private:
    /**
     *屏蔽
     *@param 略
     *@return 略
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to) final override
    {
    }
};

//远程武器
class FarWeapon : public basic_Weapon {
public:
    FarWeapon() {}
    virtual ~FarWeapon() {}

    /**
     *使用物品
     *@param gameSprite: 使用武器的精灵
     *@param from: 起始点坐标
     *@param to: 终点坐标
     *@param holdTime: 蓄力时间
     *@return 无
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to, float holdTime = 0.0f)
    {
    }

private:
    /**
     *屏蔽
     *@param 略
     *@return 略
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to) final override
    {
    }
};

#endif