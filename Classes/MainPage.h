#ifndef __MAIN_PAGE_H__
#define __MAIN_PAGE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "MyBasicScene.h"

class MainPage : public MyBasicScene {
   public:
       static MainPage* create();
    static MainPage* createScene();

    virtual bool init(bool isGamePage) override;
};

class MainPageLayer0 : public Layer {
   public:
    virtual bool init() override;

    CREATE_FUNC(MainPageLayer0);
};

#endif
