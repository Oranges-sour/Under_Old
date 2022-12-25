#include "MyEnemy0.h"

#include "GameManager.h"
#include "GameMap.h"
#include "GameRenderer.h"
#include "Hero.h"
#include "MyMath.h"
#include "MyResourcesDef.h"
#include "Particle.h"
#include "ResourcesManager.h"

//////////////////////////////////////////////////////////////////////////////////////////

MyEnemy0* MyEnemy0::create(const string& frame, const string& parOnKill,
                           const string& bframe, const string& bparOnKill) {
    auto e = new (std::nothrow) MyEnemy0();
    if (e && e->initGameSprite(frame, parOnKill, bframe, bparOnKill)) {
        e->autorelease();
        return e;
    }
    CC_SAFE_DELETE(e);
    return nullptr;
}

bool MyEnemy0::initGameSprite(const string& frame, const string& parOnKill,
                              const string& bframe, const string& bparOnKill) {
    this->cocosSchedule([&](float) { this->_update(); }, "upd");

    this->parOnKill = parOnKill;

    weapon = new MyEnemy0Weapon(bframe, bparOnKill);

    this->logicSchedule([&](float) { canShoot = true; }, 0.5f, "shh");

    basic_Enemy::initGameSprite();
    return this->initWithSpriteFrameName(frame);
}

void MyEnemy0::getContact(basic_GameSprite* contactFrom) {
    auto type = contactFrom->getGameSpriteType();
    if (type.type0 == GameSpriteType::Type0::bullet &&
        type.type1 == GameSpriteType::Type1::heroBullet) {
        auto bullet = dynamic_cast<basic_Bullet*>(contactFrom);
        this->setHitPoint(getHitPoint() - bullet->getDamage());

        auto p0 = bullet->speedVec;
        speedVec += p0 * 0.2f;
    }
}

void MyEnemy0::kill() {
    showParticleOnKill();
    auto m = GameManager::getInstance();
    m->removeGameSprite(this);
}

void MyEnemy0::_update() {
    runAI();
    if (getHitPoint() <= 0) {
        kill();
    }
}

void MyEnemy0::runAI() {
    auto m = GameManager::getInstance();
    auto hero = m->getHero();
    auto heroPos = hero->getPosition();
    auto myPos = this->getPosition();
    if (MyMath::distance(heroPos, myPos) < 2200) {
        if (canShoot) {
            weapon->useItem(this, myPos, heroPos, 0);
            canShoot = false;
        }
    }
}

void MyEnemy0::showParticleOnKill() {
    auto createPar = [&]() {
        auto p = basic_Particle::create(parOnKill, 0.3f, 5, 20);
        p->setScale(0.8f);
        p->setPosition(this->getPosition());
        auto gm = GameManager::getInstance();

        auto pp = gm->getGameMap()->convertInMap(p->getPosition());
        gm->addGameSprite(p, pp, GameRenderOrder::user0);
    };

    for (int x = 0; x < 15; ++x) {
        createPar();
    }
    auto gm = GameManager::getInstance();
    auto gr = gm->getGameRenderer();
    gr->shakeCamera(45, 15, 0.3f);
}

//////////////////////////////////////////////////////////////////////////////////////////

MyEnemy0Bullet* MyEnemy0Bullet::create(const string& frame,
                                       const string& parOnKill) {
    auto b = new (std::nothrow) MyEnemy0Bullet();
    if (b && b->initGameSprite(frame, parOnKill)) {
        b->autorelease();
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}

bool MyEnemy0Bullet::initGameSprite(const string& frame,
                                    const string& parOnKill) {
    this->parOnKill = parOnKill;
    this->cocosSchedule([&](float) { _update(); }, "upd");

    basic_Bullet::initGameSprite();
    return this->initWithSpriteFrameName(frame);
}

void MyEnemy0Bullet::onContact(basic_GameSprite* contactTarget) {
    auto type = contactTarget->getGameSpriteType();
    if ((type.type0 == GameSpriteType::Type0::mapTile ||
         type.type0 == GameSpriteType::Type0::hero) &&
        !isContactFinish()) {
        setContactFinish(true);
        kill();
    }
}

void MyEnemy0Bullet::shoot(const Vec2& start, const Vec2& to) {
    setPosition(start);
    setRotation(MyMath::getRotation(start, to));
    speedVec = MyMath::getPosOnLine(Vec2(0, 0), to - start, 20);
}

void MyEnemy0Bullet::kill() {
    showParOnKill();
    auto gameM = GameManager::getInstance();
    gameM->removeGameSprite(this);
}

void MyEnemy0Bullet::_update() { setRotation(getRotation() + 25); }

void MyEnemy0Bullet::showParOnKill() {
    auto createPar = [&]() {
        auto p = basic_Particle::create(parOnKill, 0.1f, 2, 20);
        p->setScale(0.5f);
        p->setPosition(this->getPosition());
        auto gm = GameManager::getInstance();

        auto pp = gm->getGameMap()->convertInMap(p->getPosition());
        gm->addGameSprite(p, pp, GameRenderOrder::user0);
    };
    for (int x = 0; x < 5; ++x) {
        createPar();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

MyEnemy0Weapon::MyEnemy0Weapon(const string& frame, const string& parOnKill)
    : frame(frame), parOnKill(parOnKill) {}

MyEnemy0Weapon::~MyEnemy0Weapon() {}

void MyEnemy0Weapon::useItem(basic_GameSprite* gameSprite, const Vec2& from,
                             const Vec2& to, float holdTime) {
    auto b = MyEnemy0Bullet::create(frame, parOnKill);

    auto gm = GameManager::getInstance();
    auto rm = gm->getResourcesManager();
    GameSpritePhysicsInfo info;
    info.activeContact = true;
    info.gravityEffect = false;
    info.needPhysics = true;
   /* info.contactWithWall = true;*/

    info.physicsShape = rm->getPhysicsBody(ResKey::Physics::Enemy0Bullet);

    b->setPhysicsInfo(info);
    b->shoot(from, to);

    auto pp = gm->getGameMap()->convertInMap(b->getPosition());
    gm->addGameSprite(b, pp, GameRenderOrder::user0);
}
