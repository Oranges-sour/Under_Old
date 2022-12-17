#include "GamePage.h"

#include "GameDefault.h"
#include "GameManager.h"
#include "GameMap.h"
#include "GameRenderer.h"
#include "Joystick.h"
#include "MapCandle.h"
#include "MyEnemy0.h"
#include "MyResourcesDef.h"
#include "Random.h"
#include "ResourcesManager.h"
#include "SimpleAudioEngine.h"
#include "test.h"

GamePage* GamePage::create() {
    auto s = new (std::nothrow) GamePage;
    if (s && s->init(true)) {
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

GamePage* GamePage::createScene() {
    auto scene = GamePage::create();
    return scene;
}

bool GamePage::init(bool isGameScene) {
    if (!Scene::initWithPhysics() || !MyBasicScene::init(isGameScene)) {
        return false;
    }

    auto phyw = this->getPhysicsWorld();
    phyw->setGravity(Vec2::ZERO);
    // phyw->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    phyw->setUpdateRate(1.0);

    auto layer0 = GamePageLayer0::create();
    this->addChild(layer0, 1);

    return true;
}

bool GamePageLayer0::init() {
    if (!Layer::init()) {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();

    this->bk = Sprite::create("allPageBk.png");
    bk->setPosition(visibleSize / 2);
    this->addChild(bk, 2);

    this->spAct0 = Sprite::createWithSpriteFrameName("mapDrawWaitAct0.png");
    spAct0->setPosition(visibleSize / 2);
    this->addChild(spAct0, 3);

    for (int x = 0; x < 9; ++x) {
        auto sp = Sprite::createWithSpriteFrameName("mapDrawWaitAct1.png");
        loadingBlocks[x] = sp;
        auto pos = getLoadingBlocksPos(x);
        sp->setPosition(pos);
        sp->setScale(0);
        this->addChild(sp, 3);
    }

    this->schedule(
        [&](float) {
            static int x = 0;
            auto sp = loadingBlocks[x];

            auto a0 = ScaleTo::create(0.3f, 1.2f);
            auto e0 = EaseIn::create(a0, 2.0f);
            auto a1 = ScaleTo::create(0.15f, 1.0f);
            auto e1 = EaseOut::create(a1, 2.0f);

            auto a2 = ScaleTo::create(0.3f, 0.0f);
            auto e2 = EaseIn::create(a2, 2.0f);

            auto a3 = DelayTime::create(0.8f);

            auto s0 = Sequence::create({e0, e1, a3, e2});
            sp->runAction(RepeatForever::create(s0));
            ++x;
            if (x == 9) {
                this->unschedule("startBlocks");
            }
        },
        0.1f, "startBlocks");

    gameInit();

    this->schedule(
        [&](float) {
            auto gameManager = GameManager::getInstance();
            auto map = gameManager->getGameMap();
            if (map->isInitFinish()) {
                init0();
                fadeOutLoading();
                this->unschedule("checkInit");
            }
        },
        "checkInit");

    return true;
}

void GamePageLayer0::cleanup() {
    GameManager::destroyInstance();
    Layer::cleanup();
}

void GamePageLayer0::addEnemy() {
    // 加入敌人
    auto gm = GameManager::getInstance();
    auto rm = gm->getResourcesManager();

    auto f0 = rm->getSpriteFrames(ResKey::SpFrame::Enemy0)[0];
    auto par = rm->getSpriteFrames(ResKey::SpFrame::Particle)[0];
    auto f1 = rm->getSpriteFrames(ResKey::SpFrame::Enemy0)[1];

    auto gameMap = gm->getGameMap();
    // 随机坐标
    auto heroPos = gm->getHero()->getPosition();
    auto p0 = gameMap->convertInMap(heroPos);
    rand_int r0(-5, 5);
    Vec2 pos;
    bool canCreate = false;
    for (int x = 0; x < 100; ++x) {
        auto p1 = p0 + iVec2(r0(), r0());
        p1.x = min<int>(250, max<int>(3, p1.x));
        p1.y = min<int>(250, max<int>(3, p1.y));
        if (gameMap->tileSameWith(p1 + iVec2(-1, -1), p1 + iVec2(1, 1),
                                  GameMapTileType::air)) {
            pos = gameMap->convertInPixel(p1).first;
            canCreate = true;
            break;
        }
    }
    if (canCreate) {
        auto e = MyEnemy0::create(f0, par, f1, par);
        e->setScale(1.5f);
        e->setPosition(pos);
        e->setHitPoint(200);

        GameSpritePhysicsInfo info;
        info.activeContact = true;
        info.gravityEffect = true;
        info.needPhysics = true;
        info.contactWithWall = true;

        info.physicsShape = rm->getPhysicsBody(ResKey::Physics::Enemy0);
        e->setPhysicsInfo(info);

        gm->addGameSprite(e, GameRenderOrder::user0);
    }
}

void GamePageLayer0::gameInit() {
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    GameManager::createInstance();
    auto gameManager = GameManager::getInstance();
    this->addChild(gameManager, 1);
    initRes();

    gameManager->initManager();
    auto gameMap = gameManager->getGameMap();
    auto rm = gameManager->getResourcesManager();
    gameMap->initMap(GameDef::GAME_MAP_SIZE_256, GameDef::GAME_MAP_SIZE_256, 64,
                     10000, "gameMapTile");

    auto& frames = rm->getSpriteFrames(ResKey::SpFrame::Hero);
    auto hero = TestHero::create(frames[0], frames[1], frames[2]);
    hero->setPosition(visibleSize / 2);
    gameManager->setHero(hero);
    {
        GameSpritePhysicsInfo info;
        info.activeContact = true;
        info.gravityEffect = true;
        info.needPhysics = true;
        info.contactWithWall = true;

        info.physicsShape = rm->getPhysicsBody(ResKey::Physics::Hero);
        hero->setPhysicsInfo(info);
    }

    // 创建蜡烛
    for (int y = 0; y < 20; ++y) {
        rand_int r0(0, 250);
        Vec2 pos = gameMap->convertInPixel(iVec2(r0(), r0())).first;

        auto frames0 = rm->getSpriteFrames(ResKey::SpFrame::MapCandle);
        auto frames1 = rm->getSpriteFrames(ResKey::SpFrame::Particle);
        auto c = MapCandle::create(frames0[0], frames0[1], frames1[0],
                                   frames0[2], frames0[2]);
        c->setPosition(pos);
        gameManager->addGameSprite(c, GameRenderOrder::user0);
    }

    // 游戏背景
    this->mapBk = Sprite::create("mapBk.png");
    mapBk->setAnchorPoint(Vec2(0, 0));
    mapBk->setPosition(Vec2(0, 0));
    Texture2D::TexParams texParams = {GL_LINEAR, GL_LINEAR, GL_REPEAT,
                                      GL_REPEAT};
    mapBk->getTexture()->setTexParameters(texParams);
    mapBk->setTextureRect(Rect(0, 0, 64 * 256, 64 * 256));
    this->addChild(mapBk, 0);

    this->schedule(
        [&](float) {
            auto gm = GameManager::getInstance();
            auto gr = gm->getGameRenderer();
            const auto deltaPos = gr->getGameCameraPosition();
            mapBk->setPosition(deltaPos / 2);
        },
        "setPos");
}

void GamePageLayer0::initRes() {
    auto gm = GameManager::getInstance();
    auto rm = gm->getResourcesManager();

    rm->addSpriteSheet("test.plist");
    // 加载游戏地图资源
    rm->addSpriteFrames({"game_map_tile_grass.png"},
                        ResKey::SpFrame::GameMapGrass);
    rm->addSpriteFrames({"game_map_tile_dirt.png"},
                        ResKey::SpFrame::GameMapDirt);
    rm->addSpriteFrames({"game_map_tile_rock.png"},
                        ResKey::SpFrame::GameMapRock);
    rm->addSpriteFrames(
        {"game_map_tile_cover0.png", "game_map_tile_cover1.png",
         "game_map_tile_cover2.png", "game_map_tile_cover3.png"},
        ResKey::SpFrame::GameMapCover);
    ////////////////////////////////

    // 英雄资源
    rm->addSpriteFrames({"hero_0.png", "hero_center.png", "hero_out.png"},
                        ResKey::SpFrame::Hero);
    rm->addSpriteFrames({"hero_bullet.png"}, ResKey::SpFrame::HeroBullet);
    rm->addSpriteFrames({"item_0.png"}, ResKey::SpFrame::Item0);

    // 敌人资源
    rm->addSpriteFrames({"enemy_0.png", "enemy_bullet.png"},
                        ResKey::SpFrame::Enemy0);

    // 蜡烛
    rm->addSpriteFrames({"mapLight.png", "candle_0.png", "candle_light.png"},
                        ResKey::SpFrame::MapCandle);

    // 统一粒子
    rm->addSpriteFrames({"particle.png"}, ResKey::SpFrame::Particle);

    rm->addPhysicsShapes("physics_test.plist");
    rm->addPhysicsBody("enemy_0", ResKey::Physics::Enemy0);
    rm->addPhysicsBody("enemy_bullet", ResKey::Physics::Enemy0Bullet);
    rm->addPhysicsBody("gameMapTile", ResKey::Physics::GameMapTile);
    rm->addPhysicsBody("hero_0", ResKey::Physics::Hero);
    rm->addPhysicsBody("item_0", ResKey::Physics::Item0);
    rm->addPhysicsBody("hero_bullet", ResKey::Physics::HeroBullet);

    rm->addTexture("mapBk.png", ResKey::GAME_MAP_BACK_GROUND);
}

void GamePageLayer0::fadeOutLoading() {
    this->scheduleOnce(
        [&](float) {
            for (int x = 0; x < 9; ++x) {
                auto sp = loadingBlocks[x];
                auto a0 = FadeOut::create(0.3f);
                auto a1 = CallFuncN::create(
                    [&](Node* node) { node->setVisible(false); });
                auto s0 = Sequence::create({a0, a1});
                sp->runAction(s0);
            }
            {
                auto a0 = FadeOut::create(0.3f);
                auto a1 = CallFuncN::create(
                    [&](Node* node) { node->setVisible(false); });
                auto s0 = Sequence::create({a0, a1});
                spAct0->runAction(s0);
            }
            {
                auto a0 = FadeOut::create(0.3f);
                auto a1 = CallFuncN::create(
                    [&](Node* node) { node->setVisible(false); });
                auto s0 = Sequence::create({a0, a1});
                bk->runAction(s0);
            }
        },
        0.2f, "_out");
}

void GamePageLayer0::init0() {
    auto gm = GameManager::getInstance();
    auto rm = gm->getResourcesManager();
    {
        auto hero = gm->getHero();
        auto visibleSize = Director::getInstance()->getVisibleSize();

        auto lightSp = Sprite::createWithSpriteFrameName("colorLight.png");
        lightSp->setPosition(visibleSize / 2);
        // lightSp->setOpacity(255 * 0.6);
        lightSp->setScale(15);
        hero->addLightSprite(lightSp, 0);
    }

    {
        auto t_item = TestItemSprite::create(
            rm->getSpriteFrames(ResKey::SpFrame::Item0)[0]);
        t_item->setPosition(Vec2(800, 800));
        GameSpritePhysicsInfo info;
        info.activeContact = true;
        info.gravityEffect = false;
        info.needPhysics = true;

        info.physicsShape = rm->getPhysicsBody(ResKey::Physics::Item0);
        t_item->setPhysicsInfo(info);
        gm->addGameSprite(t_item, GameRenderOrder::user1);
    }
    this->schedule([&](float) { addEnemy(); }, 4.5f, "addE");
}

Vec2 GamePageLayer0::getLoadingBlocksPos(int x) {
    std::array<Vec2, 9> arr{Vec2(1115, 885), Vec2(1280, 885), Vec2(1445, 885),
                            Vec2(1115, 720), Vec2(1280, 720), Vec2(1445, 720),
                            Vec2(1115, 555), Vec2(1280, 555), Vec2(1445, 555)};
    return arr[x];
}

//////////////////////////////////////////////////////////////////////////

SelectMapPage* SelectMapPage::createScene() {
    auto s = SelectMapPage::create();
    return s;
}

SelectMapPage* SelectMapPage::create() {
    auto s = new (std::nothrow) SelectMapPage();
    if (s && s->init(false)) {
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

bool SelectMapPage::init(bool isGamePage) {
    if (!MyBasicScene::init(isGamePage)) {
        return false;
    }

    auto l = SelectMapPageLayer0::create();
    this->addChild(l, 1);

    return true;
}

bool SelectMapPageLayer0::init() {
    if (!Layer::init()) {
        return false;
    }
    return true;
}