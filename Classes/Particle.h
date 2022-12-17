#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "cocos2d.h"
USING_NS_CC;

#include <string>
using namespace std;

#include "basic_GameSprite.h"

class basic_Particle : public basic_GameSprite {
public:
    /**
     *创建粒子
     *@param frame: 精灵帧
     *@param speedRate: 速度比率
     *@param rotaterate: 旋转比率
     *@return 粒子精灵
     */
    static basic_Particle* create(const string& frame, float liveTime,
                           float speedRate = 1.0f, float rotateRate = 0.0f);

    /**
     *初始化粒子
     *@param liveTime:粒子的存活时间
     *@return 是否初始化成功
     */
    virtual bool initGameSprite(const string& frame, float liveTime,
                                float speedRate, float rotateRate);

    void _update();

    virtual void kill()override;

private:
    Vec2 addVec;
    float addRotation;
};

#endif