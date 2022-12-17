#include "MySchedule.h"

#include <vector>
using namespace std;

void LogicSchedule::logicScheduleUpdate(float dt)
{
    vector<string> needToErase;
    for (auto& it : scheduleMap) {
        auto& pack = it.second;
        //先减少delay的值
        if (pack.delay > 0) {
            pack.delay -= dt;
            if (pack.delay < 0)  //减少最后一次delay多减少了,加到计时
                pack.timeCount = -pack.delay;
        } else {
            //每一帧都运行
            if (pack.interval < 0) {
                --pack.repeat;
                pack.callBack(dt);
            } else {
                for (; pack.timeCount >= pack.interval &&
                       (pack.repeat >= 1 || pack.repeat < 0);
                     pack.timeCount -= pack.interval, --pack.repeat)
                    pack.callBack(pack.interval);
                pack.timeCount += dt;
            }
        }
        if (pack.repeat == 0) needToErase.push_back(it.first);
    }

    for (auto& it : needToErase) scheduleMap.erase(it);
}

void LogicSchedule::logicSchedule(const std::function<void(float)>& callback,
                                  const std::string& key)
{
    this->logicSchedule(callback, -1, -1, -1, key);
}

void LogicSchedule::logicSchedule(const std::function<void(float)>& callback,
                                  float interval, const std::string& key)
{
    this->logicSchedule(callback, interval, -1, -1, key);
}

void LogicSchedule::logicSchedule(const std::function<void(float)>& callback,
                                  float interval, int repeat, float delay,
                                  const std::string& key)
{
    SchedulePack pack;
    pack.callBack = callback;
    pack.interval = interval;
    pack.repeat = repeat;
    pack.delay = delay;
    pack.timeCount = 0.0f;
    scheduleMap[key] = pack;
}

void LogicSchedule::logicScheduleOnce(
    const std::function<void(float)>& callback, float delay,
    const std::string& key)
{
    this->logicSchedule(callback, -1, 1, delay, key);
}

void LogicSchedule::logicUnschedule(const std::string& key)
{
    scheduleMap.erase(key);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void CocosSchedule::cocosScheduleUpdate(float dt)
{
    vector<string> needToErase;
    for (auto& it : scheduleMap) {
        auto& pack = it.second;
        //先减少delay的值
        if (pack.delay > 0) {
            pack.delay -= dt;
            if (pack.delay < 0)  //减少最后一次delay多减少了,加到计时
                pack.timeCount = -pack.delay;
        } else {
            //每一帧都运行
            if (pack.interval < 0) {
                --pack.repeat;
                pack.callBack(dt);
            } else {
                for (; pack.timeCount >= pack.interval &&
                       (pack.repeat >= 1 || pack.repeat < 0);
                     pack.timeCount -= pack.interval, --pack.repeat)
                    pack.callBack(pack.interval);
                pack.timeCount += dt;
            }
        }
        if (pack.repeat == 0) needToErase.push_back(it.first);
    }

    for (auto& it : needToErase) scheduleMap.erase(it);
}

void CocosSchedule::cocosSchedule(const std::function<void(float)>& callback,
                                  const std::string& key)
{
    this->cocosSchedule(callback, -1, -1, -1, key);
}

void CocosSchedule::cocosSchedule(const std::function<void(float)>& callback,
                                  float interval, const std::string& key)
{
    this->cocosSchedule(callback, interval, -1, -1, key);
}

void CocosSchedule::cocosSchedule(const std::function<void(float)>& callback,
                                  float interval, int repeat, float delay,
                                  const std::string& key)
{
    SchedulePack pack;
    pack.callBack = callback;
    pack.interval = interval;
    pack.repeat = repeat;
    pack.delay = delay;
    pack.timeCount = 0.0f;
    scheduleMap[key] = pack;
}

void CocosSchedule::cocosScheduleOnce(
    const std::function<void(float)>& callback, float delay,
    const std::string& key)
{
    this->cocosSchedule(callback, -1, 1, delay, key);
}

void CocosSchedule::cocosUnschedule(const std::string& key)
{
    scheduleMap.erase(key);
}
