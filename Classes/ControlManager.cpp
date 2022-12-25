#include "ControlManager.h"

#include <vector>
using namespace std;

#include "GameManager.h"
#include "GameRenderer.h"
#include "Hero.h"
#include "TouchesPool.h"

ControlManager* ControlManager::create()
{
    auto c = new (std::nothrow) ControlManager();
    if (c && c->init()) {
        c->autorelease();
        return c;
    }
    CC_SAFE_DELETE(c);
    return nullptr;
}

bool ControlManager::init()
{
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = [&](vector<Touch*> touch, Event*) -> void {
        for (auto& t : touch) {
            TouchesPool::instance->addToPool(t);
        }
    };

    touchListener->onTouchesEnded = [&](vector<Touch*> touch, Event*) -> void {
        for (auto& t : touch) {
            TouchesPool::instance->removeFromPool(t);
        }
    };

    touchListener->onTouchesMoved = [&](vector<Touch*> touch, Event*) -> void {
    };

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode key,
                                         Event* en) { keyDown(key); };

    keyboardListener->onKeyReleased = [&](EventKeyboard::KeyCode key,
                                          Event* en) { keyUp(key); };

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [&](EventMouse* eventMouse) {
        auto b = eventMouse->getMouseButton();
        auto pos = eventMouse->getLocationInView();
        if (b == EventMouse::MouseButton::BUTTON_RIGHT) {
            mouseRDown(pos);
        }
        if (b == EventMouse::MouseButton::BUTTON_LEFT) {
            mouseLDown(pos);
        }
        mousePosition = pos;
    };

    mouseListener->onMouseUp = [&](EventMouse* eventMouse) {
        auto b = eventMouse->getMouseButton();
        auto pos = eventMouse->getLocationInView();
        if (b == EventMouse::MouseButton::BUTTON_RIGHT) {
            mouseRUp(pos);
        }
        if (b == EventMouse::MouseButton::BUTTON_LEFT) {
            mouseLUp(pos);
        }
        mousePosition = pos;
    };

    mouseListener->onMouseMove = [&](EventMouse* eventMouse) {
        auto pos = eventMouse->getLocationInView();
        mouseMove(pos);
        mousePosition = pos;
    };

    auto _dis = Director::getInstance()->getEventDispatcher();
    _dis->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    _dis->addEventListenerWithSceneGraphPriority(mouseListener, this);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    mousePosition = visibleSize / 2;

    this->schedule([&](float) { _update(); }, "_update");

    return Node::init();
}

const iVec2& ControlManager::getMoveVec() { return moveVec; }

const Vec2& ControlManager::getMousePosition() { return mousePosition; }

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void ControlManager::_update()
{
    if (isRightMouseDown) {
        auto frameRate = Director::getInstance()->getFrameRate();
        rightMouseDownTime += 1.0f / frameRate;
    }
}

void ControlManager::keyDown(EventKeyboard::KeyCode key)
{
    auto gm = GameManager::getInstance();
    auto gr = gm->getGameRenderer();
    auto hero = gm->getHero();

    switch (key) {
        case EventKeyboard::KeyCode::KEY_A: {
            hero->setActionType(HeroActionType::LRun);

            moveVec.x -= 1;
        } break;
        case EventKeyboard::KeyCode::KEY_D: {
            hero->setActionType(HeroActionType::RRun);

            moveVec.x += 1;
        } break;
        case EventKeyboard::KeyCode::KEY_SHIFT: {
            isShift = true;
        } break;
        case EventKeyboard::KeyCode::KEY_W: {
            moveVec.y += 1;
        } break;
        case EventKeyboard::KeyCode::KEY_Q: {
        } break;
        default:
            break;
    }
}

void ControlManager::keyUp(EventKeyboard::KeyCode key)
{
    auto gm = GameManager::getInstance();
    auto gr = gm->getGameRenderer();
    auto hero = gm->getHero();

    switch (key) {
        case EventKeyboard::KeyCode::KEY_A: {
            hero->setActionType(HeroActionType::LStay);

            moveVec.x += 1;
        } break;
        case EventKeyboard::KeyCode::KEY_D: {
            hero->setActionType(HeroActionType::RStay);

            moveVec.x -= 1;
        } break;
        case EventKeyboard::KeyCode::KEY_SHIFT: {
            isShift = false;
        } break;
        case EventKeyboard::KeyCode::KEY_W: {
            moveVec.y -= 1;
        } break;
        default:
            break;
    }
}

void ControlManager::mouseLDown(const Vec2& pos) {}

void ControlManager::mouseLUp(const Vec2& pos)
{
    auto gm = GameManager::getInstance();
    auto gr = gm->getGameRenderer();
    auto hero = gm->getHero();

    auto deltaPos = gr->getGameCameraPosition();
    hero->attackFar(mousePosition - deltaPos, 0);
}

void ControlManager::mouseRDown(const Vec2& pos)
{
    isRightMouseDown = true;
    rightMouseDownTime = 0.0f;
}

void ControlManager::mouseRUp(const Vec2& pos)
{
    auto gm = GameManager::getInstance();
    auto gr = gm->getGameRenderer();
    auto hero = gm->getHero();

    auto deltaPos = gr->getGameCameraPosition();
    hero->attackFar(mousePosition - deltaPos, rightMouseDownTime);

    isRightMouseDown = false;
    rightMouseDownTime = 0.0f;
}

void ControlManager::mouseMove(const Vec2& pos) {}
