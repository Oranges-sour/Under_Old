#include "GameRenderer.h"

#include <algorithm>

#include "GameDefault.h"
#include "MyMath.h"
#include "basic_GameSprite.h"
using namespace std;

GameRenderer* GameRenderer::create()
{
    GameRenderer* gr = new GameRenderer();
    if (gr && gr->init()) {
        gr->autorelease();
        return gr;
    }
    CC_SAFE_DELETE(gr);
    return nullptr;
}

bool GameRenderer::init()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    objectRenderer = Node::create();
    finalDraw = Node::create();
    finalDraw->setAnchorPoint(Vec2(0, 0));
    //默认1.0，缩放可以debug
    finalDraw->setScale(1.0f);
    lightDraw = Sprite::create();
    lightRenderTexture =
        RenderTexture::create(visibleSize.width, visibleSize.height);

    lightDraw->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    lightDraw->setScaleY(-1);

    //debug 开关
    lightDraw->setVisible(true);

    finalDraw->addChild(objectRenderer, 1);

    this->addChild(finalDraw, 1);
    this->addChild(lightDraw, 2);

    lightRenderTexture->retain();

    this->schedule([&](float) { updateShake(); }, 1.0f / 30, "_updShake");

    return true;
}

void GameRenderer::addNode(Node* node, int zOrder)
{
    objectRenderer->addChild(node, zOrder);
}

void GameRenderer::removeNode(Node* node) { objectRenderer->removeChild(node); }

void GameRenderer::_update(const vector<basic_GameSprite*>& gameObjects,
                           const Vec2& heroPosition, const Vec2& mousePosition)
{
    auto spriteFrame = updateLight(gameObjects);
    lightDraw->setSpriteFrame(spriteFrame);
    lightDraw->setBlendFunc({GL_DST_COLOR, GL_ZERO});

    //更新摄像机的位置
    const auto vs = Director::getInstance()->getVisibleSize() / 2;
    const auto p1 = Vec2(-vs.width + mousePosition.x, -vs.height + mousePosition.y);
    const auto p2 = heroPosition + p1;
    const auto centerPos = MyMath::getPosOnLine(
        heroPosition, p2, MyMath::distance(heroPosition, p2) / 15);
    cameraPosNeed = Vec2(vs.width - centerPos.x, vs.height - centerPos.y);
    const auto nowCameraPos = getGameCameraPosition();
    const auto deltaPos = cameraPosNeed - nowCameraPos;

    const auto calcuSpeed = [](float x) -> float { return POT(x) / (12 * (x + 3)); };

    const float dis = MyMath::distance(Vec2(0, 0), deltaPos);
    const float speed = calcuSpeed(dis);

    auto movePos = MyMath::getPosOnLine(Vec2(0, 0), deltaPos, speed);
    setGameCameraPosition(nowCameraPos + movePos);
}

SpriteFrame* GameRenderer::updateLight(
    const vector<basic_GameSprite*>& gameObjects)
{
    auto& deltaPos = finalDraw->getPosition();
    vector<vector<Sprite*>> needToRender;

    const UINT number = MyTool::getMaxThreadNumber();
    needToRender.resize(number);

    const Rect screenRect(-deltaPos, MyMath::getScreenSize());
    const auto callInTh = [&](UINT from, UINT to, UINT num) {
        auto& v = needToRender[num];
        for (UINT x = from; x < to; ++x) {
            auto& gameSp = gameObjects[x];
            auto& li = gameSp->getAllLightSprites();
            for (auto& it : li) {
                auto& sp = it.second;
                if (screenRect.intersectsRect(sp->getBoundingBox())) {
                    v.push_back(sp);
                }
            }
        }
    };
    MyTool::callInThread(gameObjects.size(), callInTh);

    lightRenderTexture->beginWithClear(0, 0, 0, 1);
    for (int x = 0; x < needToRender.size(); ++x) {
        auto& v = needToRender[x];
        for (int y = 0; y < v.size(); ++y) {
            auto& sprite = v[y];
            sprite->setPosition(sprite->getPosition() + deltaPos);
            sprite->setVisible(true);
            sprite->visit();
            sprite->setVisible(false);
            sprite->setPosition(sprite->getPosition() - deltaPos);
        }
    }
    lightRenderTexture->end();
    return lightRenderTexture->getSprite()->getSpriteFrame();
}

void GameRenderer::updateShake()
{
    ++shakeCount;
    vector<shared_ptr<ShakeBag>> needToErase;
    Vec2 delta(0, 0);
    for (int x = 0; x < shakes.size(); ++x) {
        auto& shakeBag = *shakes[x];
        delta += Vec2(shakeBag.amplitude * DEG::cos(shakeBag.angle),
                      shakeBag.amplitude * DEG::sin(shakeBag.angle));
        shakeBag.time -= 1.0 / 30.0f;
        if (shakeCount % 2 == 0) {
            shakeBag.amplitude -= shakeBag.delta;
            if (shakeBag.time <= 0) {
                needToErase.push_back(shakes[x]);
            }
        }
    }
    if (shakeCount % 2 == 0) {
        delta = -delta;
    }
    setGameCameraPosition(getGameCameraPosition() + delta);
    for (int x = 0; x < needToErase.size(); ++x) {
        auto& shakeBag = needToErase[x];
        shakes.erase(remove(shakes.begin(), shakes.end(), shakeBag),
                     shakes.end());
    }
    needToErase.clear();
}

void GameRenderer::setGameCameraPosition(const Vec2& position)
{
    finalDraw->setPosition(position);
}

const Vec2& GameRenderer::getGameCameraPosition()
{
    return finalDraw->getPosition();
}

bool GameRenderer::isInCamera(const Vec2& pos)
{
    const auto p1 = -getGameCameraPosition();
    const Rect r1(p1, MyMath::getScreenSize());
    return r1.containsPoint(pos);
}

bool GameRenderer::isInCamera(const Rect& rect)
{
    const auto p1 = -getGameCameraPosition();
    const Rect r1(p1, MyMath::getScreenSize());
    return r1.intersectsRect(rect);
}

void GameRenderer::shakeCamera(float angle, float amplitude, float time)
{
    constexpr float frequency = 30;
    auto shakeBag = make_shared<ShakeBag>();

    shakeBag->amplitude = amplitude;
    shakeBag->angle = angle;
    shakeBag->time = time;
    shakeBag->delta = amplitude / (frequency * time) * 2;

    shakes.push_back(shakeBag);
}

void GameRenderer::cleanup()
{
    lightRenderTexture->release();
    Node::cleanup();
}
