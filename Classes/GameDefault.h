#ifndef __GAME_DEFAULT_H__
#define __GAME_DEFAULT_H__

#include <functional>

#include "cocos2d.h"
USING_NS_CC;

typedef unsigned int UINT;

namespace GameDef {
//游戏一"刻"的时间(1/60秒)
constexpr float GAME_TICK_TIME = 1.0f / 60;

constexpr UINT GAME_MAP_SIZE_64 = 64;
constexpr UINT GAME_MAP_SIZE_128 = 128;
constexpr UINT GAME_MAP_SIZE_192 = 192;
constexpr UINT GAME_MAP_SIZE_256 = 256;
}  // namespace GameDef

namespace MyTool {
/**
*针对于大vector更新的多线程拆分更新
*@param vecSize:vector的大小
*@param callFunc:更新函数
*@return 无
*@例子:

std::vector<int> vec;
callInThread(vec.size(),
[&](UINT from, UINT to, UINT threadNumber)
{
        for(int x = from; x < to; ++x)
        {
                vec[x] += 1;
        }
});
*/
void callInThread(
    const size_t vecSize,
    const std::function<void(UINT from, UINT to, UINT threadNumber)>& callFunc);

/**
*获得callInThread()能使用的最大线程数量
*@return 最大线程数量
*/
UINT getMaxThreadNumber();

/**
 *设置是否全屏,每调用一次,窗口状态就会更改一次
 *@param 无
 *@return 无
 */
void setFullScreen();
}  // namespace MyTool

#endif
