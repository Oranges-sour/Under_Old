#include "MainPage.h"
#include "GamePage.h"

MainPage* MainPage::create()
{
    auto s = new(std::nothrow)MainPage();
    if (s && s->init(false)) {
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

MainPage* MainPage::createScene()
{
    auto scene = MainPage::create();
    return scene;
}

bool MainPage::init(bool isGamePage)
{
    if (!MyBasicScene::init(isGamePage)) {
        return false;
    }

    auto layer0 = MainPageLayer0::create();
    this->addChild(layer0, 1);

    return true;
}

bool MainPageLayer0::init()
{
    if (!Layer::init()) {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();

    auto blackBk = Sprite::create("blackBk.png");
    blackBk->setPosition(visibleSize / 2);
    this->addChild(blackBk, 3);
    {
        auto a0 = FadeOut::create(0.3f);
        blackBk->runAction(a0);
    }

    auto bk = Sprite::create("allPageBk.png");
    bk->setPosition(visibleSize / 2);
    this->addChild(bk, 1);

    this->scheduleOnce(
        [this, blackBk](float) {
            {
                auto a0 = FadeIn::create(0.3f);
                blackBk->runAction(a0);
            }
            this->scheduleOnce(
                [&](float) {
                    auto gamePage = GamePage::createScene();
                    Director::getInstance()->replaceScene(gamePage);
                },
                0.3f, "changePage");
        },
        0.1f, "delayTurn");

    return true;
}