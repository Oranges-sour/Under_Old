#ifndef __LOADING_PAGE_H__
#define __LOADING_PAGE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "MyBasicScene.h"

class LoadingPage : public MyBasicScene {
   public:
       static LoadingPage* create();
    static LoadingPage* createScene();

    virtual bool init(bool isGamePage) override;
};

class LoadingPageLayer0 : public Layer {
   public:
    virtual bool init() override;

    CREATE_FUNC(LoadingPageLayer0);
};
#endif
