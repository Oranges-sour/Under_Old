#ifndef __CONTROL_MANAGER_H__
#define __CONTROL_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

#include "iVec2.h"

class ControlManager final : public Node {
public:
    static ControlManager* create();

    virtual bool init() override;

    const iVec2& getMoveVec();

    const Vec2& getMousePosition();

private:
    void _update();

    void keyDown(EventKeyboard::KeyCode key);
    void keyUp(EventKeyboard::KeyCode key);

    void mouseLDown(const Vec2& pos);
    void mouseLUp(const Vec2& pos);

    void mouseRDown(const Vec2& pos);
    void mouseRUp(const Vec2& pos);

    void mouseMove(const Vec2& pos);

private:
    bool isRightMouseDown = false;
    float rightMouseDownTime = 0;
    bool isShift = false;
    iVec2 moveVec;
    Vec2 mousePosition;
};

#endif