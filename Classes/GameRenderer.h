#ifndef __GAME_RENDERER_H__
#define __GAME_RENDERER_H__

#include "cocos2d.h"
USING_NS_CC;

#include <memory>
#include <vector>

#include "GameSpriteType.h"

class basic_GameSprite;

//一个帧动"包"
struct ShakeBag {
    //倾斜角度
    float angle = 0;
    //持续时间
    float time = 0;
    //振幅
    float amplitude = 0;
    //每次减少的振幅
    float delta = 0;
};

//游戏渲染器
class GameRenderer final : public Node {
public:
    static GameRenderer* create();

    /**
     *初始化渲染器
     *@param
     *@return
     */
    virtual bool init() override;

    /**
     *添加节点
     *@param 节点指针
     *@return 无
     */
    void addNode(Node* node, int zOrder);

    /**
     *移除节点
     *@param 节点指针
     *@return 无
     */
    void removeNode(Node* node);

    /**
     *更新渲染
     *@param 略
     *@return 无
     */
    void _update(const std::vector<basic_GameSprite*>& gameObjects,
                 const Vec2& heroPosition, const Vec2& mousePosition);

    /**
     *更新光源,需要每帧调用
     *@param gameObjects:所有在游戏中的对象.将由此类进行光源判断
     *@return 无
     */
    SpriteFrame* updateLight(const std::vector<basic_GameSprite*>& gameObjects);

    /**
     *更新摄像机帧动
     *@param 无
     *@return 无
     */
    void updateShake();

    /**
     *设置摄像机的左下角坐标
     *@param position:坐标
     *@return 无
     */
    void setGameCameraPosition(const Vec2& position);

    /**
     *获得摄像机当前的左下角坐标
     *@param 无
     *@return 左下角坐标
     */
    const Vec2& getGameCameraPosition();

    /**
     *坐标是否在摄像机区域内
     *@param pos: 坐标
     *@return 是否在区域内
     */
    bool isInCamera(const Vec2& pos);

    /**
     *矩形是否在摄像机区域内
     *@param rect: 矩形
     *@return 是否在区域内
     */
    bool isInCamera(const Rect& rect);

    /**
     *抖动摄像机,频率默认30Hz
     *@param angle: 倾斜角度
     *@param amplitude: 振幅(像素距离
     *@param time: 持续时间
     *@return 无
     */
    void shakeCamera(float angle, float amplitude, float time);

    /**
     *设置在完成所有对象的渲染之后的全图渲染shader
     *@param key:用户资源管理器中的shader键值
     *@param order:渲染的顺序(从小order到大)
     *@return 无
     */
    // void addAfterDrawShader(int key, int order);

    /**
     *移除全图渲染shader
     *@param key:用户资源管理器中的shader键值
     *@return 无
     */
    // void removeAfterDrawShader(int key);

    virtual void cleanup() override;

private:
    std::vector<std::shared_ptr<ShakeBag>> shakes;

    unsigned int shakeCount = 0;

    RenderTexture* lightRenderTexture = nullptr;

    //向屏幕显示图片的精灵
    Sprite* lightDraw = nullptr;
    Node* objectRenderer = nullptr;
    Node* finalDraw = nullptr;
    //相机真正需要到达的坐标
    Vec2 cameraPosNeed;
};

#endif