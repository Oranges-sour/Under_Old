#ifndef __GAME_ITEM_H__
#define __GAME_ITEM_H__

#include "cocos2d.h"
USING_NS_CC;

#include "basic_GameSprite.h"
class basic_Hero;

enum class GameItemType { unknow, nearWeapon, farWeapon };

/*
游戏中那些可以(只)被英雄捡起或使用的物品
在地图上显示的精灵
*/
class basic_GameItemSprite : public basic_GameSprite {
public:
    basic_GameItemSprite() {}
    virtual ~basic_GameItemSprite() {}

    /**
     *物品被捡起了,实例化此函数以对英雄进行一些操作
     *@param hero:捡起此物品的英雄
     *@return 无
     */
    virtual void itemPickedUp(basic_Hero* hero) = 0;

    /**
     *获得精灵种类
     *@param 无
     *@return 种类
     */
    virtual GameSpriteType getGameSpriteType() final override;

    // item不需要一般的碰撞检测
    virtual void onContact(basic_GameSprite* contactTarget) final override{};
    virtual void getContact(basic_GameSprite* contactFrom) final override{};
};

class basic_GameItem {
public:
    basic_GameItem() {}
    virtual ~basic_GameItem() {}

    /**
     *获得物品种类
     *@param 无
     *@return 物品种类
     */
    virtual GameItemType getItemType() const { return GameItemType::unknow; }

    /**
     *使用物品
     *@param gameSprite: 使用物品的精灵
     *@return 无
     */
    virtual void useItem(basic_GameSprite* gameSprite) = 0;
};

#endif
