#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "cocos2d.h"
USING_NS_CC;

#include <string>
#include <utility>
#include <vector>

#include "GameDefault.h"
#include "basic_GameSprite.h"
#include "iVec2.h"

class GameMap;
class GameMapStruct;

class Random;

// 地图块的种类
enum class GameMapTileType {
    // 未知
    unknow,
    // 空气
    air,
    // 草地
    grass,
    // 泥土
    dirt,
    // 石块
    rock
};

class basic_GameMapBuilding;

// 一个地图的瓦片
class GameMapTile {
public:
    GameMapTileType type;

    const Rect& getRect();

private:
    friend class GameMapStruct;
    // 用来碰撞检测的矩形
    Rect tileRect;
};

class GameMapPhysicsObject : public basic_GameSprite {
public:
    /**
     *创建带物理信息的地图块
     *@param 无
     *@return 地图块
     */
    static GameMapPhysicsObject* create();

    virtual void onContact(basic_GameSprite* contactTarget) override {}
    virtual void getContact(basic_GameSprite* contactFrom) override {}
    virtual void kill() override {}
    virtual GameSpriteType getGameSpriteType() override {
        return {GameSpriteType::Type0::mapTile, GameSpriteType::Type1::unknow};
    }

private:
    /**
     *初始化地图块
     *@param 无
     *@return 是否初始化成功
     */
    virtual bool initGameSprite();
};

class GameMapStruct {
public:
    /**
     *初始化地图,并设置地图的大小,横竖的瓦片数量
     *@param width_tile:地图的宽度,瓦片数量
     *@param height_tile:地图的高度,瓦片数量
     *@param tilePixleSize:地图块像素大小
     *@return 无
     */
    void init(UINT width, UINT height, float tilePixelSize, unsigned int seed);

    /**
     *获取给定位置的瓦片
     *@param x: 横坐标
     *@param y: 纵坐标
     *@return 无
     */
    GameMapTile& at(UINT x, UINT y);

private:
    /**
     *初始化地图块
     *@param 无
     *@return 无
     */
    void initMapBlock(unsigned int seed);

    /**
     *初始化地图块, 使用柏林噪声
     *@param 无
     *@return 无
     */
    void initMapBlock_Prelin(Random& random);

    /**
     *生成噪声
     *@param 无
     *@return 无
     */
    std::vector<float> perlinNoise2D(int width, int height, int octaves,
                                     float bias, std::vector<float>& noiseSeed);

    std::vector<std::vector<GameMapTile>> _map;
    int width = 0;
    int height = 0;
};

class GameMapPhysicsTile {
public:
    GameMapPhysicsObject* physicsTile = nullptr;
    bool needPhysics = false;
    /*使用计数,每一帧减少一次
     *减少到0就自动消除这个物理模拟块
     */
    int count = 0;
    bool isInDirty = false;
    iVec2 posInMap;
};

class GameMap final : public Node {
public:
    /**
     *创建地图对象,不会调用GameMap::initMap(...)
     *@param 无
     *@return 地图对象指针
     */
    static GameMap* create();

    /**
     *初始化地图,并设置地图的大小(横竖的瓦片数量)
     *@param width_tile:地图的宽度(瓦片数量)
     *@param height_tile:地图的高度(瓦片数量)
     *@param tilePixelSize:一个瓦片的像素大小(瓦片是正方形,不需单独设定长宽)
     *@param physicsBody:每一个地图块的物理信息
     *@return 无
     */
    void initMap(UINT width_tile, UINT height_tile, float tilePixelSize,
                 unsigned int seed, const std::string& physicsBody);

    /**
     *获得地图的像素大小
     *@param 无
     *@return 地图的像素大小
     */
    Size getMapPixleSize();

    /**
     *获得地图块的大小
     *@param 无
     *@return 大小
     */
    Size getMapTileSize();

    /**
     *将真实像素坐标映射到地图网格坐标
     *@param position: 真实像素坐标
     *@return 地图网格坐标
     */
    iVec2 convertInMap(const Vec2& position);

    /**
     *将地图网格坐标映射到像素坐标
     *@param position: 地图网格坐标
     *@return 一个pair,当前网格的左下角和右上角坐标
     * (first:左下角, second:右上角)
     */
    std::pair<Vec2, Vec2> convertInPixel(const iVec2& position);

    /**
     *获得地图网格坐标的地图块
     *@param position: 地图网格坐标
     *@return 地图块
     */
    GameMapTile& at(const iVec2& position);

    /**
     *这一区域内的所有地图块是否都和参数种类相同
     *@param from: 左下角起始点
     *@param to: 右上角终点
     *@param type: 种类
     *@return 地图块
     */
    bool tileSameWith(const iVec2& from, const iVec2& to, GameMapTileType type);

    /**
     *精灵移动时使用,找到精灵所走的方向前面是否有方块
     *@param 无
     *@return first:是否有方块 second:发现的方块
     */
    std::pair<bool, GameMapTile*> findMapTile(const iVec2& from,
                                              const iVec2& to);

    /**
     *每一帧更新
     *@param 无
     *@return 无
     */
    void _update(const QuadCoor& left_top, const QuadCoor& right_bottom);

    /**
     *获得地图中的所有建筑物
     *@param 无
     *@return 建筑物
     */
    const std::vector<basic_GameMapBuilding*>& getMapBuildings();

    /**
     *地图是否初始化完成(包括预渲染的完成)
     *@param 无
     *@return 略
     */
    bool isInitFinish();

    /**
     *获得当前初始化的进度
     *@param 无
     *@return 第一个模板数的x和y代表当前
     */
    std::pair<iVec2, iVec2> getInitProgress();

private:
    /**
     *创建瓦片精灵
     *@param 无
     *@return 无
     */
    void createTileSprite();

    /**
     *设置瓦片的物理碰撞
     *@param physicsBody:名称
     *@return 无
     */
    void setTilePhysicsBody(const std::string& physicsBody);

    /**
     *更新物理块
     *@param 无
     *@return 无
     */
    void updatePhysicsTile(const QuadCoor& left_top,
                           const QuadCoor& right_bottom);

    // 地图材质块
    using TileTextureBlock = std::pair<Sprite*, std::array<Sprite*, 4>>;

    /**
     *获得对应地图块的精灵(仅仅地图预渲染时使用)
     *@param position: 地图块的位置
     *@return 精灵
     */
    TileTextureBlock getTileTextureBlock(const iVec2& position);

    /**
     *渲染地图
     *@param 无
     *@return 无
     */
    void renderMap(unsigned int scaleRate, int mapBlockTileSize);

private:
    GameMapStruct mapStruct;
    std::vector<std::vector<GameMapPhysicsTile>> physicsTiles;
    // 需要更新的物理块
    std::vector<GameMapPhysicsTile*> dirtyPhysicsTiles;
    std::vector<std::vector<Sprite*>> mapBlocks;
    int mapWidth = 0;
    int mapHeight = 0;
    float tilePixelSize = 0;

    // 一个渲染大图数组的宽度
    int mapBlockWidth = 0;
    // 一个渲染大图数组的高度
    int mapBlockHeight = 0;
    // 一个渲染大图的像素大小
    float mapBlockSize = 0;

    // 地图左下角坐标
    //  Vec2 mapPosition;
    // 物理信息
    std::string physicsBody;

    // 地图建筑物
    std::vector<basic_GameMapBuilding*> buildings;

    // 延迟渲染地图时,标记渲染到第几个块

    int init_Width = 0;
    int init_Height = 0;
    bool initFinish = false;
};

// 游戏中的建筑物
class basic_GameMapBuilding : public basic_GameSprite {
    // TODO:完善
};

#endif
