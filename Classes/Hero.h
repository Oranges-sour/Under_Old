#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
USING_NS_CC;

#include <map>
#include <string>
#include <vector>

#include "GameItem.h"
#include "basic_GameSprite.h"

class HeroItemBag final {
public:
    using ItemMap = std::map<GameItemType, std::vector<basic_GameItem*>>;

    HeroItemBag();
    ~HeroItemBag();
    /**
     *添加一个物品
     *@param item:物品
     *@return 无
     */
    void addItem(basic_GameItem* item);

    /**
     *移除物品
     *@param item:要移除的物品
     *@return 无
     */
    void removeItem(basic_GameItem* item);

    /**
     *得到所有物品
     *@param 无
     *@return 所有物品
     */
    const ItemMap& getAllItems();

private:
    ItemMap items;
};

enum class HeroActionType {
    LWalk,
    RWalk,
    LRun,
    RRun,
};

class basic_Hero : public basic_GameSprite {
public:
    /**
     *获取血量
     *@param 无
     *@return 血量
     */
    virtual int getHitPoint() final;

    /**
     *设置血量
     *@param hitPoint:血量
     *@return 无
     */
    virtual void setHitPoint(int hitPoint) final;

    /**
     *获取蓝量
     *@param 无
     *@return 蓝量
     */
    virtual int getEnergy() final;

    /**
     *设置蓝量
     *@param energy:蓝量
     *@return 无
     */
    virtual void setEnergy(int energy) final;

    /**
     *获得物品包
     *@param 无
     *@return 物品包
     */
    virtual HeroItemBag& getItemBag() final;

    /**
     *设置英雄动作状态
     *@param type:状态
     *@return 无
     */
    virtual void setActionType(HeroActionType type);

    /**
     *获取英雄动作状态
     *@param
     *@return 状态
     */
    virtual HeroActionType getActionType();

    /**
     *被碰撞
     *@param contactTarget: 撞击目标
     *@return 无
     */
    virtual void onContact(basic_GameSprite* contactTarget) override;

    /**
     *发动近程攻击
     *@param pos: 鼠标指引的攻击位置
     *@return 无
     */
    virtual void attackNear(const Vec2& pos) = 0;

    /**
     *发动远程攻击
     *@param pos: 鼠标指引的攻击位置
     *@param holdTime: 蓄力的时间,秒
     *@return 无
     */
    virtual void attackFar(const Vec2& pos, float holdTime) = 0;

private:
    HeroActionType actionType;
    HeroItemBag itemBag;
    int hitPoint = 0;
    int energy = 0;
};

#endif
