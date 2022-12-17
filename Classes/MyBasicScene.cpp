#include "MyBasicScene.h"

#include "GameDefault.h"

bool MyBasicScene::init(bool isGamePage)
{
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event*) {

    };
    keyBoardListener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event*) {
        switch (code) {
            case EventKeyboard::KeyCode::KEY_F2:
                MyTool::setFullScreen();
                break;
        }
    };
    auto evd = Director::getInstance()->getEventDispatcher();
    evd->addEventListenerWithSceneGraphPriority(keyBoardListener, this);

    if (!isGamePage) {
        auto mouseListener = EventListenerMouse::create();
        mouseListener->onMouseDown = [&](EventMouse*) {

        };
        mouseListener->onMouseMove = [&](EventMouse*) {

        };
        mouseListener->onMouseUp = [&](EventMouse* event) {
            constexpr int COUNT_MAX = 4;
            const auto pos = event->getLocationInView();
            Sprite* sp = nullptr;
            if (mouseDownCount == COUNT_MAX) {
                sp = Sprite::create("mouseDown_1.png");
            } else {
                sp = Sprite::create("mouseDown_0.png");
            }

            this->addChild(sp, 2);
            {
                sp->setPosition(pos);
                sp->setScale(0);

                float time = 0.15;
                if (mouseDownCount == COUNT_MAX) {
                    time += 0.5;
                    mouseDownCount = 0;
                }

                auto a0 = ScaleTo::create(time - 0.03, 1);
                auto a1 = FadeOut::create(time);

                auto e1 = EaseOut::create(a0, 2);
                auto e2 = EaseOut::create(a1, 2);
                auto spa = Spawn::create({e1, e2});
                auto callB = CallFuncN::create(
                    [](Node* node) { node->removeFromParent(); });
                auto seq = Sequence::create({spa, callB});
                sp->runAction(seq);
            }

            ++mouseDownCount;
        };
        evd->addEventListenerWithSceneGraphPriority(mouseListener, this);

        this->schedule(
            [&](float) {
                if (mouseDownCount > 0) {
                    --mouseDownCount;
                }
            },
            0.6, "subMouseCount");
    }

    return true;
}

bool MyBasicScene::init()
{
    return true;
}
