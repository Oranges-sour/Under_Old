#include "Particle.h"

#include <cmath>

#include "GameManager.h"
#include "MyMath.h"
#include "Random.h"

basic_Particle* basic_Particle::create(const string& frame, float liveTime,
                                       float speedRate, float rotateRate)
{
    auto par = new (std::nothrow) basic_Particle();
    if (par && par->initGameSprite(frame, liveTime, speedRate, rotateRate)) {
        par->autorelease();
        return par;
    }
    CC_SAFE_DELETE(par);
    return nullptr;
}

bool basic_Particle::initGameSprite(const string& frame, float liveTime,
                                    float speedRate, float rotateRate)
{
    rand_int r(1, 2);
    rand_float r0(0, 360);
    float a = r0();
    //使用平方来随机正负
    speedVec = Vec2(DEG::cos(a), DEG::sin(a)) * speedRate;
    addRotation = rotateRate;

    this->cocosSchedule([&](float) { _update(); }, "_update");
    this->cocosScheduleOnce([&](float) { kill(); }, liveTime, "live");

    basic_GameSprite::initGameSprite();
    return this->initWithSpriteFrameName(frame);
}

void basic_Particle::_update()
{
    speedVec.y -= 0.8f;
    setRotation(getRotation() + addRotation);
}

void basic_Particle::kill()
{
    auto act0 = FadeOut::create(0.2f);
    auto act1 = ScaleTo::create(0.2f, 0.0f);
    this->runAction(act0);
    this->runAction(act1);

    this->cocosScheduleOnce(
        [&](float) {
            auto m = GameManager::getInstance();
            m->removeGameSprite(this);
        },
        0.2f, "kill");
}
