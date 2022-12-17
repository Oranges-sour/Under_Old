#ifndef __GAME_PAGE_H__
#define __GAME_PAGE_H__

#include "cocos2d.h"
USING_NS_CC;

#include <array>
using namespace std;

#include "MyBasicScene.h"

class GamePage : public MyBasicScene {
public:
    static GamePage* create();
    static GamePage* createScene();

    virtual bool init(bool isGameScene) override;
};

class GamePageLayer0 : public Layer {
public:
    virtual bool init() override;
    virtual void cleanup() override;

    CREATE_FUNC(GamePageLayer0);

    void addEnemy();

private:
    void gameInit();
    void initRes();
    void fadeOutLoading();
    void init0();
    Vec2 getLoadingBlocksPos(int x);

private:
    std::array<Sprite*, 9> loadingBlocks;
    Sprite* mapBk = nullptr;
    Sprite* bk = nullptr;
    Sprite* spAct0 = nullptr;
};

//////////////////////////////////////////////////////////////////////////

class SelectMapPage : public MyBasicScene {
public:
    static SelectMapPage* create();
    static SelectMapPage* createScene();

    virtual bool init(bool isGamePage) override;
};

class SelectMapPageLayer0 : public Layer {
public:
    virtual bool init() override;

    CREATE_FUNC(SelectMapPageLayer0);
};
#endif
