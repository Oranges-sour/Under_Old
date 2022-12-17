#ifndef __MY_ENEMY_0_H__
#define __MY_ENEMY_0_H__

#include "cocos2d.h"
USING_NS_CC;

#include <string>
using namespace std;

#include "Bullet.h"
#include "Enemy.h"
#include "Weapon.h"

class MyEnemy0Weapon;

class MyEnemy0 : public basic_Enemy {
public:
    static MyEnemy0* create(const string& frame, const string& parOnKill,
                            const string& bframe, const string& bpaoOnKill);
    virtual bool initGameSprite(const string& frame, const string& parOnKill,
                                const string& bframe, const string& bparOnKill);

    virtual void getContact(basic_GameSprite* contactFrom);
    virtual void kill() override;
    virtual GameSpriteType getGameSpriteType() override
    {
        return {GameSpriteType::Type0::enemy, GameSpriteType::Type1::unknow};
    }

    void _update();

    void runAI();

private:
    bool canShoot = false;
    MyEnemy0Weapon* weapon = nullptr;
    void showParticleOnKill();

private:
    string parOnKill;
};

class MyEnemy0Bullet : public basic_Bullet {
public:
    static MyEnemy0Bullet* create(const string& frame, const string& parOnKill);
    virtual bool initGameSprite(const string& frame, const string& parOnKill);

    virtual void onContact(basic_GameSprite* contactTarget) override;

    virtual void shoot(const Vec2& start, const Vec2& to);

    virtual void kill() override;
    virtual GameSpriteType getGameSpriteType() override
    {
        return {GameSpriteType::Type0::bullet,
                GameSpriteType::Type1::enemyBullet};
    }

    void _update();

private:
    virtual void shoot(const Vec2& start) override {}
    void showParOnKill();

private:
    string parOnKill;
};

class MyEnemy0Weapon : public FarWeapon {
public:
    MyEnemy0Weapon(const string& frame, const string& parOnKill);
    virtual ~MyEnemy0Weapon();

    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
                         const Vec2& to, float holdTime);

private:
    string frame;
    string parOnKill;
};

#endif