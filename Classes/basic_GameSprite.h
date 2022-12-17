#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "cocos2d.h"
USING_NS_CC;

//#include "GameManager.h"
#include "MySchedule.h"

class GameMapTile;

#include <map>
#include <string>
#include <vector>

#include "GameSpriteType.h"

struct GameSpritePhysicsInfo {
    //物体的形状以及碰撞(PhysicsEditor中的数据)
    std::string physicsShape;

    //是否受到重力影响
    bool gravityEffect = false;

    //是否会主动撞击他人 true:会主动撞击他人 flase:只会被动接受撞击
    bool activeContact = false;

    //是否需要物理模拟
    bool needPhysics = false;

    //是否会与地图碰撞
    bool contactWithWall = false;

    //精灵是否站在地面(只有检测了与地图的碰撞此值才有用
    bool onGround = false;

    //物体质量(kg),计算地面摩擦力使用
    float mass = 1.0f;
};

/*游戏的基对象,游戏中出现的任何元素(如子弹,英雄,敌人,甚至特殊物品)
都要从此对象继承而来.
-----
物理碰撞的说明:
        对于一个对象,虽然精灵帧改变,但碰撞边界不改变.对于这种设计进行如下说明:
           一般情况下,精灵帧的改变只是小范围的改变,不需要重新设置碰撞边界.如果有大范围
           的改变,预测是攻击的特效,可以使用另外的方式清除被攻击的物体,不应使用对性能影响较大的
           碰撞边界(每次重新设置边界都要重新计算三角形).
*/
class basic_GameSprite : public Sprite,
                         public CocosSchedule,
                         public LogicSchedule {
public:
    basic_GameSprite() {}
    virtual ~basic_GameSprite() {}

    /**
     *精灵撞击到了其他精灵,此精灵是撞击者
     *@param contactTarget: 被撞击的物体
     *@return 无
     */
    virtual void onContact(basic_GameSprite* contactTarget);

    /**
     *精灵受到撞击,不是撞击发出者
     *@param contactFrom:撞击发出者
     *@return 无
     */
    virtual void getContact(basic_GameSprite* contactFrom);

    /**
    *杀死此精灵,并产生粒子特效(依据实际情况).
    *如果不想产生特效等,可以直接调用 void removeFromParent()
    *@param 无

    *@return 无
    */
    virtual void kill();

    /**
     *获得对象的类型
     *@param 无
     *@return 对象类型
     */
    virtual GameSpriteType getGameSpriteType();

    /**
     *添加Buff,并给定持续时间,如果已经获得此buff时间会叠加
     *@param objectBuffType: buff的类型
     *@param time: buff的持续时间
     *@return 无
     */
    // virtual void addBuff(GameObjectBuffType objectBuffType, float time)final;

    /**
     *减少Buff,给定减少的时间,时间如果小于0则直接移除buff
     *@param objectBuffType: buff的类型
     *@param time: buff的减少时间
     *@return 无
     */
    // virtual void subBuff(GameObjectBuffType objectBuffType, float time)final;

    /**
     *获得当前受到的所有buff以及剩余时间
     *@param 无
     *@return buff的类型以及对应的时间
     */
    // virtual const std::map<GameObjectBuffType, float>& getBuff()final;

    /**
     *是否受到了param所给定的buff,并且剩余的时间是否超过了所给定的阈值(可选的)
     *@param objectBuffType: buff的种类
     *@param time: 剩余时间的阈值(可选的)
     *@return 受到:ture  不受到:false
     */
    // virtual bool isBuff(GameObjectBuffType objectBuffType, float time =
    // -1.0f)final;

    /**
     *设置精灵的物理信息,如碰撞体积或质量
     *@param physicsInfo: 物理信息
     *@return 无
     */
    virtual void setPhysicsInfo(const GameSpritePhysicsInfo& physicsInfo) final;

    /**
     *获得精灵的物理信息
     *@param 无
     *@return 对象的物理信息
     */
    virtual GameSpritePhysicsInfo& getPhysicsInfo() final;

    /**
     *加载物理碰撞
     */
    virtual void uploadPhysicsBody() final;

    /**
     *卸载物理碰撞
     */
    virtual void unloadPhysicsBody() final;

    /**
     *获得所有光源渲染精灵
     *@param 无
     *@return 光源精灵
     */
    virtual const std::map<int, Sprite*>& getAllLightSprites() final;

    /**
     *获得光源渲染精灵
     *@param key: 键值
     *@return 光源精灵
     */
    virtual Sprite* getLightSprite(int key) final;

    /**
     *添加光源精灵
     *@param 光源精灵
     *@return 无
     */
    virtual void addLightSprite(Sprite* lightSprite, int key) final;

    /**
     *移除光源精灵
     *@param 光源精灵
     *@return 无
     */
    virtual void removeLightSprite(int key) final;

    /**
     *初始化
     *@param 无
     *@return 是否初始化成功
     */
    virtual bool initGameSprite();

    /**
     *清理对象
     *@param 无
     *@return 无
     */
    virtual void cleanup() override;

public:
    //每一帧的运动矢量(当前帧过后就会被清除)
    Vec2 moveVec;
    //运动的速度
    Vec2 speedVec;
    //加速度,每一帧加速度都会影响速度
    Vec2 accVec;

private:
    // buff的种类与时间的映射表
    // std::map<GameObjectBuffType, float> buffs;

    //物理碰撞
    PhysicsBody* physicsBody = nullptr;
    //光源渲染精灵
    std::map<int, Sprite*> lightSprites;
    //物理信息
    GameSpritePhysicsInfo physicsInfo;
};

#endif
