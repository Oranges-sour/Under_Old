#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
USING_NS_CC;

#include "basic_GameSprite.h"

class basic_Bullet : public basic_GameSprite {
public:
    /**
     *发射子弹
     *@param startPosition:子弹的起始坐标
     *@return 无
     */
    virtual void shoot(const Vec2& startPosition) = 0;

    /**
     *设置子弹是否完成碰撞
     *@param finish:是否
     *@return 无
     */
    void setContactFinish(bool finish) noexcept { contactFinish = finish; }

    /**
     *是否碰撞了
     *@param 无
     *@return 是否碰撞了
     */
    bool isContactFinish() noexcept { return contactFinish; }

    /**
     *设置攻击力
     *@param damage: 攻击力
     *@return 无
     */
    void setDamage(int damage) { this->damage = damage; }

    /**
     *获得子弹攻击力
     *@param 无
     *@return 攻击力
     */
    int getDamage() { return damage; }

private:
    //子弹的伤害
    int damage = 0;
    //防止多次受到物理碰撞造成多次伤害
    bool contactFinish = false;
};

#endif
