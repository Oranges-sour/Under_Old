#include "LoadingPage.h"

#include "MainPage.h"

LoadingPage* LoadingPage::create()
{
    auto s = new (std::nothrow) LoadingPage();
    if (s && s->init(false)) {
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

LoadingPage* LoadingPage::createScene()
{
    auto scene = LoadingPage::create();
    return scene;
}

bool LoadingPage::init(bool isGamePage)
{
    if (!MyBasicScene::init(isGamePage)) {
        return false;
    }

    auto layer0 = LoadingPageLayer0::create();
    this->addChild(layer0, 1);

    return true;
}

bool LoadingPageLayer0::init()
{
    if (!Layer::init()) {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
        "loadingPage.plist");

    auto blackBk = Sprite::create("blackBk.png");
    blackBk->setPosition(visibleSize / 2);
    this->addChild(blackBk, 3);
    {
        auto a0 = FadeOut::create(0.3f);
        blackBk->runAction(a0);
    }

    auto bk = Sprite::create("loadingPageBk.png");
    bk->setPosition(visibleSize / 2);
    this->addChild(bk, 1);

    auto spAct0 = Sprite::createWithSpriteFrameName("loadingAct0.png");
    spAct0->setPosition(visibleSize / 2);
    this->addChild(spAct0, 2);
    {
        auto a0 = RotateBy::create(0.6f, -90.0f);
        auto e0 = EaseInOut::create(a0, 3);
        auto a1 = DelayTime::create(0.4f);
        auto s0 = Sequence::create({e0, a1});
        spAct0->runAction(RepeatForever::create(s0));
    }

    auto spAct1 = Sprite::createWithSpriteFrameName("loadingAct1.png");
    spAct1->setPosition(visibleSize / 2);
    this->addChild(spAct1, 2);
    {
        auto a0 = ScaleTo::create(0.4f, 1.2f);
        auto e0 = EaseIn::create(a0, 3);
        auto a1 = ScaleTo::create(0.6f, 1.0f);
        auto e1 = EaseOut::create(a1, 2);
        auto s0 = Sequence::create({e0, e1});

        auto a2 = RotateBy::create(0.6f, 180.0f);
        auto e2 = EaseInOut::create(a2, 3);
        auto a3 = DelayTime::create(0.4f);
        auto s1 = Sequence::create({e2, a3});
        spAct1->runAction(RepeatForever::create(s0));
        spAct1->runAction(RepeatForever::create(s1));
    }

    this->scheduleOnce(
        [this, blackBk](float) {
            {
                auto a0 = FadeIn::create(0.3f);
                blackBk->runAction(a0);
            }
            this->scheduleOnce(
                [&](float) {
                    auto mainPage = MainPage::createScene();
                    Director::getInstance()->replaceScene(mainPage);
                },
                0.3f, "changePage");
        },
        0.2f, "delayTurn");
    return true;
}