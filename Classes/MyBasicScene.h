#ifndef __MY_BASIC_SCENE_H__
#define __MY_BASIC_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class MyBasicScene : public Scene {
public:
    virtual bool init(bool isGamePage);

private:
    virtual bool init() final override;
    int mouseDownCount = 0;
};

#endif