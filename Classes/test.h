#ifndef __TEST_H__
#define __TEST_H__

#include <string>
#include <vector>

#include "Bullet.h"
#include "Enemy.h"
#include "GameManager.h"
#include "Hero.h"
#include "Particle.h"
#include "Weapon.h"
#include "basic_GameSprite.h"
using namespace std;

class TestHero : public basic_Hero {
public:
    static TestHero* create(const string& hero, const string& center,
                            const string& out);
    virtual bool initGameSprite(const string& hero, const string& center,
                                const string& out);

    virtual void onContact(basic_GameSprite* contactTarget) override;
    virtual void getContact(basic_GameSprite* contactTarget) override {}
    virtual void kill() override { this->removeFromParent(); }
    virtual GameSpriteType getGameSpriteType() override
    {
        return {GameSpriteType::Type0::hero, GameSpriteType::Type1::unknow};
    }
    virtual void _update();
    virtual void attackNear(const Vec2& pos) override;
    virtual void attackFar(const Vec2& pos, float holdTime) override;

private:
    Sprite* center = nullptr;
    Sprite* out = nullptr;
};

class TestItemSprite : public basic_GameItemSprite {
public:
    static TestItemSprite* create(const std::string& p);
    virtual bool initGameSprite(const std::string& p);

    virtual void kill() override;

    virtual void itemPickedUp(basic_Hero* hero) override;
};

class HeroBullet : public basic_Bullet {
public:
    static HeroBullet* create(const string& p);

    virtual bool initGameSprite(const string& p);

    void delayShoot(const Vec2& startPosition, const Vec2& to, float time);

    virtual void shoot(const Vec2& startPosition, const Vec2& to);

    void update1();

    virtual void getContact(basic_GameSprite* contactTarget) override {}

    virtual void kill() override;

    virtual void onContact(basic_GameSprite* contactTarget) override;

    virtual GameSpriteType getGameSpriteType() override;

private:
    virtual void shoot(const Vec2& startPositon) final override {}
    Vec2 deltaPos;
    float x = 0;
};

class TestWeapon : public FarWeapon {
public:
    virtual GameItemType getItemType() const override
    {
        return GameItemType::farWeapon;
    };

    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
                         const Vec2& to, float holdTime) override;
};

#endif
