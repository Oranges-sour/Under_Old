#ifndef __PHYSICS_MANAGER_H__
#define __PHYSICS_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

#include <tuple>
#include <vector>

#include "iVec2.h"

class basic_GameSprite;

class PhysicsManager final : public Node {
public:
    /**
     *创建物理管理器
     *@param 无
     *@return 管理器指针
     */
    static PhysicsManager* create();

    /**
     *更新游戏精灵的物理状态,如重力等
     *@param gameObjects:所有在游戏中的对象.将由此类进行判断.
     *@return 无
     */
    void updatePhysics(const std::vector<basic_GameSprite*>& gameObjects);

private:
    /**
     *处理精灵与墙的碰撞(卡住精灵)
     *@param sprite: 精灵
     *@return 无
     */
    void contactWithWall(basic_GameSprite* sprite);

    /**
     *精灵水平移动
     *@param speedVec: 速度矢量
     *@param moveSpeed: 移动速度
     *@param checkPoint: 精灵的四角在地图中的坐标,
                        顺序:左下, 左上, 右下, 右上
     *@param heroBoundingBox
     *@return 精灵坐标需要的增加量
     */
    Vec2 horizonMove(Vec2& speedVec,
                     const std::tuple<const iVec2&, const iVec2&, const iVec2&,
                                      const iVec2&>& checkPoint,
                     const Rect& boundingBox);

    /**
     *精灵竖直移动
     *@param speedVec: 速度矢量
     *@param moveSpeed: 真正的移动速度
     *@param checkPoint: 精灵的四角在地图中的坐标,
                        顺序:左下, 左上, 右下, 右上
     *@param heroBoundingBox
     *@return 精灵坐标需要的增加量
     */
    Vec2 verticalMove(Vec2& speedVec,
                      const std::tuple<const iVec2&, const iVec2&, const iVec2&,
                                       const iVec2&>& checkPoint,
                      const Rect& boundingBox);

    /**
     *初始化物理管理器
     *@param 无
     *@return 无
     */
    virtual bool init() override;

    /**
     *处理物理碰撞
     *@param
     *@return
     */
    void processContact(PhysicsContact& conatct);
};

#endif