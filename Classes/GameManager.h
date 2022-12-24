#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

#include <array>
#include <map>
#include <set>
#include <tuple>
#include <vector>

#include "GameSpriteType.h"
#include "QuadTree.h"
#include "iVec2.h"

class basic_GameMapBuilding;
class PhysicsManager;
class ControlManager;
class GameRenderer;
class GameMap;
class GameMapTile;
class ResourcesManager;
class basic_Hero;
class basic_GameSprite;

enum class GameRenderOrder { map = 1, user0, user1, user2, user3 };

/*提示:
 * 1.不将UI等元素添加入GameManager的渲染中,凡是在GameManager的渲染都会收到光源的影响
 */

class GameManager final : public Node {
public:
    /**
     *创建实例
     *@param parent: 添加实例的节点
     *@return 无
     */
    static void createInstance();

    /**
     *销毁实例,并自动从添加实例的节点中移除自己和所有游戏对象
     *@param 无
     *@return 无
     */
    static void destroyInstance();

    /**
     *获得实例
     *@param 无
     *@return 实例指针
     */
    static GameManager* getInstance();

private:
    static GameManager* instance;

public:
    /**
     *在资源加载完成后调用
     *@param 无
     *@return 无
     */
    void initManager();

    /**
     *由cocos的scheduler每一帧调用,帧渲染之前调用
     *@param dt: 距离上一次调用的间隔时间
     */
    std::pair<QuadCoor /*left top*/, QuadCoor /*right bottom*/>
    mainUpdateBeforeRender(float dt);

    /**
     *在帧渲染之后调用
     *@param dt: 距离上一次调用的间隔时间
     */
    void mainUpdateAfterRender(float dt);

    /**
     *暂停游戏
     *@param 无
     *@return 无
     */
    void pauseGame();

    /**
     *开始游戏(结束暂停)
     *@param 无
     *@return 无
     */
    void resumeGame();

    /**
     *游戏是否暂停
     *@param 无
     *@return true游戏暂停 false游戏未暂停
     */
    bool isGamePause();

    /**
     *添加游戏对象,并给定渲染层
     *@param gameObject:对象的指针
     *@param layerOrder: 渲染层
     *@return 无
     */
    void addGameSprite(basic_GameSprite* gameObject, const iVec2& vec,
                       GameRenderOrder layerOrder);

    /**
     *添加普通节点,使用removeFromParent()移除
     *@param node:节点指针
     *@param layerOrder: 渲染层
     *@return 无
     */
    void addNode(Node* node, GameRenderOrder layerOrder);

    /**
     *移除游戏对象
     *@param 对象指针
     *@return 无
     */
    void removeGameSprite(basic_GameSprite* gameObject);

    /**
     *获取资源管理器
     *@param 无
     *@return 资源管理器的指针
     */
    ResourcesManager* getResourcesManager();

    /**
     *获得游戏渲染器
     *@return 渲染器的指针
     */
    GameRenderer* getGameRenderer();

    /**
     *获得游戏地图
     *@param 无
     *@return 游戏地图的指针
     */
    GameMap* getGameMap();

    /**
     *获得所有在更新矩形区域内的精灵
     *@param 无
     *@return 精灵
     */
    const Quad<basic_GameSprite*>& getAllSprites();

    /**
     *清理
     *@param 无
     *@return 无
     */
    virtual void cleanup() override;

    // 添加特殊的游戏对象(英雄,控制杆等等)

    // 设置英雄,并自动加入渲染
    void setHero(basic_Hero* hero);

    // 设置移动控制杆,不加入渲染(见"提示1")
    //  void setMoveJoystick(Joystick* joystick);

    // 获得英雄
    basic_Hero* getHero();

private:
    /**
     *初始化游戏管理器
     *@param 无
     *@return 无
     */
    virtual bool init() override;

    /**
     *更新英雄的移动
     *@param 无
     *@return 无
     */
    void updateHeroMove();

private:
    // 一些必要的数据

    /////////////////////////////////////////////////////////

    // 游戏英雄
    basic_Hero* hero = nullptr;

private:
    // 物理管理器
    PhysicsManager* physicsManager = nullptr;
    // 游戏渲染器
    GameRenderer* gameRenderer = nullptr;
    // 游戏地图
    GameMap* gameMap = nullptr;

    // 控制器
    ControlManager* controlManager = nullptr;
    // 游戏是否暂停
    bool gamePauseState = false;

    // 所有游戏对象
    std::shared_ptr<Quad<basic_GameSprite*>> gameObjects;

    // 这一帧需要删除的对象
    std::vector<basic_GameSprite*> needToErase;
    // 这一帧需要添加的对象
    std::vector<std::tuple<basic_GameSprite*, iVec2, GameRenderOrder>>
        needToAdd;

    // 资源管理器
    // 用户资源
    ResourcesManager* userResourcesManager = nullptr;
};

#endif
