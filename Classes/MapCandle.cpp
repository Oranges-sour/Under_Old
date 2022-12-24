#include "MapCandle.h"

#include "GameManager.h"
#include "GameMap.h"
#include "GameRenderer.h"
#include "Hero.h"
#include "MyMath.h"
#include "Particle.h"
#include "Random.h"

MapCandle* MapCandle::create(const string& frame, const string& star,
                             const string& par, const string& candleLight,
                             const string& starLight) {
    auto c = new (std::nothrow) MapCandle();
    if (c && c->initGameSprite(frame, star, par, candleLight, starLight)) {
        c->autorelease();
        return c;
    }
    CC_SAFE_DELETE(c);
    return nullptr;
}

bool MapCandle::initGameSprite(const string& frame, const string& star,
                               const string& par, const string& candleLight,
                               const string& starLight) {
    starFrame = star;
    particle = par;
    this->starLight = starLight;

    auto lightSp = Sprite::createWithSpriteFrameName(candleLight);
    this->addLightSprite(lightSp, 1);

    this->cocosSchedule([&](float) { _update(); }, "upd");
    this->cocosSchedule([&](float) { canCreate = true; }, 1.5f, "ss");

    basic_GameSprite::initGameSprite();
    return this->initWithSpriteFrameName(frame);
}

void MapCandle::removeStar(CandleStar* star) { needToErase.push_back(star); }

void MapCandle::_update() {
    auto gm = GameManager::getInstance();
    auto hero = gm->getHero();
    float d1 = MyMath::distance(hero->getPosition(), getPosition());
    if (stars.size() < 3 && canCreate && d1 < 1700) {
        canCreate = false;
        createStar();
    }

    for (auto& s : stars) {
        float dis = MyMath::distance(getPosition(), s->getPosition());
        if (dis < 50 && s->canKill()) {
            s->kill();
        }
    }
    for (auto& s : needToErase) {
        stars.erase(s);
    }
    needToErase.clear();

    auto sp = getLightSprite(1);
    sp->setPosition(getPosition());
}

void MapCandle::createStar() {
    auto s = CandleStar::create(this, starFrame, starLight, particle);

    rand_float r0(4, 4.5f);
    rand_float r1(0, 259);
    rand_int r2(1, 2);

    float speed = r0();
    float speed1 = 0.8f * speed;
    float a = r1();
    float b = a + 90 * pow(-1, r2());
    s->speedVec = Vec2(speed * DEG::cos(a), speed * DEG::sin(a)) +
                  Vec2(speed1 * DEG::cos(b), speed1 * DEG::sin(b));
    s->setPosition(getPosition() + Vec2(100 * DEG::cos(a), 100 * DEG::sin(a)));
    s->setOpacity(0);

    auto gm = GameManager::getInstance();
    auto gr = gm->getGameRenderer();

    auto p = gm->getGameMap()->convertInMap(s->getPosition());
    gm->addGameSprite(s, p, GameRenderOrder::user0);
    stars.insert(s);

    // ´´½¨Á£×Ó
    if (gr->isInCamera(getPosition())) {
        for (int x = 0; x < 5; ++x) {
            auto p = basic_Particle::create(particle, 0.1f, 5, 25);
            p->setScale(0.5f);
            p->setPosition(getPosition());

            auto pp = gm->getGameMap()->convertInMap(p->getPosition());
            gm->addGameSprite(p, pp, GameRenderOrder::user0);
        }
    }
}

CandleStar* CandleStar::create(MapCandle* candle, const string& frame,
                               const string& light, const string& par) {
    auto s = new (std::nothrow) CandleStar();
    if (s && s->initGameSprite(candle, frame, light, par)) {
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

bool CandleStar::initGameSprite(MapCandle* candle, const string& frame,
                                const string& light, const string& par) {
    this->candle = candle;
    particle = par;

    auto lightSp = Sprite::createWithSpriteFrameName(light);
    lightSp->setScale(0.2f);
    lightSp->setOpacity(255 * 0.2f);
    auto lact0 = ScaleTo::create(0.2f, 0.7f);
    auto lact1 = FadeIn::create(0.2f);

    lightSp->runAction(lact0);
    lightSp->runAction(lact1);

    this->addLightSprite(lightSp, 1);

    this->logicSchedule([&](float) { _update(); }, "upd");
    this->cocosScheduleOnce([&](float) { _canKill = true; }, 1.5f, "dk");
    this->cocosSchedule([&](float) { createPar(); }, 0.2f, "cp");

    auto act0 = FadeIn::create(0.2f);
    this->runAction(act0);

    basic_GameSprite::initGameSprite();
    return this->initWithSpriteFrameName(frame);
}

void CandleStar::_update() {
    auto center = candle->getPosition();
    auto myPos = getPosition();
    const float mass = 2000;
    float dis = max<float>(1, MyMath::distance(center, myPos));

    float forceG = (mass * mass) / (POT(dis));
    float acc = forceG / mass;
    Vec2 vecAcc = MyMath::getPosOnLine(Vec2(0, 0), center - myPos, acc);
    this->accVec = vecAcc;

    this->setRotation(getRotation() +
                      3.5f * MyMath::distance(Vec2(0, 0), speedVec));

    auto sp = getLightSprite(1);
    sp->setPosition(getPosition());
}

void CandleStar::createPar() {
    auto gm = GameManager::getInstance();
    auto gr = gm->getGameRenderer();
    if (!gr->isInCamera(getPosition())) {
        return;
    }
    auto p = basic_Particle::create(particle, 0.2f, 0.5f, 15);
    p->setScale(0.3f);
    p->setPosition(getPosition());

    auto pp = gm->getGameMap()->convertInMap(p->getPosition());
    gm->addGameSprite(p, pp, GameRenderOrder::user0);
}

bool CandleStar::canKill() { return _canKill; }

void CandleStar::kill() {
    auto gm = GameManager::getInstance();
    candle->removeStar(this);

    gm->removeGameSprite(this);
}
