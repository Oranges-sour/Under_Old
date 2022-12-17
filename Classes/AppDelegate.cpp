#include "AppDelegate.h"

#include "LoadingPage.h"
#include "MyMath.h"
#include "Random.h"
#include "TouchesPool.h"
#include "platform/CCGL.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error \
    "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static Size designResolutionSize = Size(2560, 1440);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes:
    // red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

static void init_app()
{
    MyMath::setScreenSize(designResolutionSize);
    glfwSwapInterval(1);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    GLViewImpl* glview = dynamic_cast<GLViewImpl*>(director->getOpenGLView());
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) ||   \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect(
            "Temple",
            cocos2d::Rect(0, 0, designResolutionSize.width / 2,
                          designResolutionSize.height / 2),
            1, true);
         //glview->setCursor("mouse.png");
#else
        glview = GLViewImpl::create("Temple");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width,
                                    designResolutionSize.height,
                                    ResolutionPolicy::SHOW_ALL);

    init_app();

    // create a scene. it's an autorelease object
    auto scene = LoadingPage::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a
// phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    TouchesPool::instance->removeAllTouches();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
