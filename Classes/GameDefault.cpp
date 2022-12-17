#include "GameDefault.h"

#include "cocos2d.h"
USING_NS_CC;

#include <functional>
#include <future>
#include <thread>
#include <vector>
using namespace std;



namespace {
const UINT __threadsQuantity = thread::hardware_concurrency();
constexpr UINT MAX_THREAD_NUMBER = 6;
}  // namespace

void MyTool::callInThread(
    const size_t vecSize,
    const function<void(UINT from, UINT to, UINT threadNumber)>& callFunc) {
    
    vector<future<void>> futures;
    const UINT threadsQuantity = getMaxThreadNumber();
    if (vecSize >= threadsQuantity) {
        //一个线程中需要更新的
        const UINT n = vecSize / threadsQuantity;
        for (UINT _x = 0; _x < threadsQuantity - 1; ++_x) {
            futures.push_back(
                async(launch::async, callFunc, n * _x, n * (_x + 1), _x));
        }
        //如果vecSize /
        // threadsQuantity的实数值不为整数时,使用整数n来进行更新会有对象更新不到.
        //所以最后一个线程直接更新到末尾
        futures.push_back(async(launch::async, callFunc,
                                n * (threadsQuantity - 1), vecSize,
                                threadsQuantity - 1));
    } else {
        // vector的大小比线程数量少时,就直接开vector的大小的线程来更新
        for (UINT _x = 0; _x < vecSize; ++_x)
            futures.push_back(async(launch::async, callFunc, _x, _x + 1, _x));
    }

    for (auto it = futures.begin(); it != futures.end(); ++it) {
        (*it).get();
    }
}

UINT MyTool::getMaxThreadNumber()
{
    //可用的线程数量
    UINT threadsQuantity = __threadsQuantity - 1;
    //防止创建线程带来的性能问题
    if (threadsQuantity > MAX_THREAD_NUMBER)
        threadsQuantity = MAX_THREAD_NUMBER;
    return threadsQuantity;
}

namespace {
bool isFullScreen = false;
Size lastFrameSize = Size(0, 0);
}  // namespace

void MyTool::setFullScreen() {
    auto director = Director::getInstance();
    GLViewImpl* glview = dynamic_cast<GLViewImpl*>(director->getOpenGLView());
    if (!isFullScreen) {
        lastFrameSize = glview->getFrameSize();
        glview->setFullscreen();
        isFullScreen = true;
    } else {
        glview->setWindowed(lastFrameSize.width, lastFrameSize.height);
        isFullScreen = false;
    }
}