#include "GameManager.h"

#include <algorithm>
#include <future>
#include <thread>
#include <utility>
using namespace std;

#include "ControlManager.h"
#include "GameDefault.h"
#include "GameMap.h"
#include "GameRenderer.h"
#include "Hero.h"
#include "Joystick.h"
#include "MyMath.h"
#include "MyResourcesDef.h"
#include "PhysicsManager.h"
#include "ResourcesManager.h"
#include "TouchesPool.h"
#include "Weapon.h"
#include "basic_GameSprite.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

GameManager* GameManager::instance = nullptr;

void GameManager::createInstance() {
    // 创建实例时,当前的实例指针应该为空!
    CC_ASSERT(instance == nullptr);
    instance = new (std::nothrow) GameManager();
    if (instance && instance->init()) {
        instance->autorelease();
        return;
    }
    CC_SAFE_DELETE(instance);
}

void GameManager::destroyInstance() {
    // 实例不应为空!
    CC_ASSERT(instance != nullptr);
    instance->removeFromParent();
    instance = nullptr;
}

GameManager* GameManager::getInstance() { return instance; }

void GameManager::initManager() {
    // 初始化渲染器
    gameRenderer = GameRenderer::create();
    // 初始化地图
    gameMap = GameMap::create();
    // 初始化物理管理器
    physicsManager = PhysicsManager::create();
    // 初始化控制器
    controlManager = ControlManager::create();

    gameObjects = make_shared<Quad<basic_GameSprite*>>(QuadCoor{0, 512},
                                                       QuadCoor{512, 0});

    this->addChild(physicsManager, 1);
    this->addChild(gameRenderer, 2);
    this->addChild(controlManager, 3);

    gameRenderer->addNode(gameMap, static_cast<int>(GameRenderOrder::map));
    gameRenderer->setVisible(false);
    // 启动主循环
    this->schedule(
        [&](float dt) {
            if (!gameMap->isInitFinish()) {
                return;
            } else {
                gameRenderer->setVisible(true);
            }
            auto p = std::move(this->mainUpdateBeforeRender(dt));
            this->gameRenderer->_update(p.first, p.second, hero->getPosition(),
                                        controlManager->getMousePosition());
            this->mainUpdateAfterRender(dt);
        },
        /*一帧运行一次*/ 0, "mainUpdate");
}

std::pair<QuadCoor /*left top*/, QuadCoor /*right bottom*/>
GameManager::mainUpdateBeforeRender(float dt) {
    auto hero_p = hero->getPosition();
    auto hero_p1 = gameMap->convertInMap(hero_p);

    auto left_top = QuadCoor{hero_p1.x - 30, hero_p1.y + 15};
    auto right_bottom = QuadCoor{hero_p1.x + 30, hero_p1.y - 15};

    vector<basic_GameSprite*> vec;

    gameObjects->visit_in_rect(left_top, right_bottom,
                               [&](const QuadCoor& coor, basic_GameSprite* sp) {
                                   vec.push_back(sp);

                                   sp->cocosScheduleUpdate(dt);
                                   sp->logicScheduleUpdate(dt);
                               });

    if (hero->quad_node.containerResult.first == true) {
        vec.push_back(hero);
    }

    for (auto& sp : vec) {
        auto p = sp->getPosition();
        auto p1 = gameMap->convertInMap(p);
        auto pp = sp->quad_node;
        auto p2 = iVec2(pp.coor.x, pp.coor.y);

        if (p1 != p2) {
            pp.container->remove(pp.containerResult.second);
            sp->quad_node = gameObjects->insert({p1.x, p1.y}, sp);
        }
    }

    this->updateHeroMove();
    physicsManager->updatePhysics(left_top, right_bottom);
    gameMap->_update(left_top, right_bottom);

    return {left_top, right_bottom};
}

void GameManager::mainUpdateAfterRender(float dt) {
    // 添加新的精灵
    for (auto iter = needToAdd.begin(); iter != needToAdd.end(); ++iter) {
        auto& sp = get<0>(*iter);
        auto& type = sp->getGameSpriteType();

        auto p = get<1>(*iter);
        auto r = gameObjects->insert({p.x, p.y}, sp);
        sp->quad_node = r;

        gameRenderer->addNode(sp, static_cast<int>(get<2>(*iter)));
        // 添加的时候retain了一次,现在release掉
        sp->release();
    }
    needToAdd.clear();

    // 移除精灵
    for (auto iter = needToErase.begin(); iter != needToErase.end(); ++iter) {
        auto& sp = *iter;
        if (sp->quad_node.containerResult.first) {
            sp->quad_node.container->remove(sp->quad_node.containerResult.second);
        }

        sp->removeFromParent();
    }
    needToErase.clear();
}

void GameManager::pauseGame() {
    gamePauseState = true;
}

void GameManager::resumeGame() {
    gamePauseState = false;
}

bool GameManager::isGamePause() { return gamePauseState; }

void GameManager::addGameSprite(basic_GameSprite* gameObject, const iVec2& vec,
                                GameRenderOrder layerOrder) {
    gameObject->retain();
    needToAdd.push_back(tuple<basic_GameSprite*, iVec2, GameRenderOrder>(
        gameObject, vec, layerOrder));
}

void GameManager::addNode(Node* node, GameRenderOrder layerOrder) {
    gameRenderer->addNode(node, static_cast<int>(layerOrder));
}

void GameManager::removeGameSprite(basic_GameSprite* gameObject) {
    auto iter = std::find(needToErase.begin(), needToErase.end(), gameObject);
    if (iter == needToErase.end()) needToErase.push_back(gameObject);
}

ResourcesManager* GameManager::getResourcesManager() {
    return userResourcesManager;
}

GameRenderer* GameManager::getGameRenderer() { return gameRenderer; }

GameMap* GameManager::getGameMap() { return gameMap; }

const Quad<basic_GameSprite*>& GameManager::getAllSprites() {
    return *gameObjects;
}

void GameManager::cleanup() {
    delete userResourcesManager;
    Node::cleanup();
}

void GameManager::setHero(basic_Hero* hero) {
    this->hero = hero;
    this->addGameSprite(hero, {0, 0}, GameRenderOrder::user0);
}

basic_Hero* GameManager::getHero() { return hero; }

bool GameManager::init() {
    userResourcesManager = new ResourcesManager();
    return true;
}

void GameManager::updateHeroMove() {
    auto& moveVec = controlManager->getMoveVec();
    auto& speedVec = hero->speedVec;
    speedVec.x = moveVec.x * 10;

    if (hero->speedVec.y < 13.0f) {
        hero->speedVec.y += moveVec.y * 2.5f;
    }
}
