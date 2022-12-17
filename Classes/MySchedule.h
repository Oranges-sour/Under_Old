#ifndef __LOGIC_SCHEDULE_H__
#define __LOGIC_SCHEDULE_H__

#include <map>
#include <string>
#include <functional>


/*
逻辑线程的定时器
*/
class LogicSchedule
{
public:
	/**
	*逻辑定时器的主更新
	*@param dt:距离上一次更新的时间
	*@return 无
	*/
	void logicScheduleUpdate(float dt);

	/**
	*逻辑定时器,每一帧都更新
	*@param callback:回调函数
	*@param key:键值
	*@return 无
	*/
	void logicSchedule(const std::function<void(float)>& callback, const std::string& key);

	/**
	*逻辑定时器
	*@param callback:回调函数
	*@param interval:更新间隔(秒
	*@param key:键值
	*@return 无
	*/
	void logicSchedule(const std::function<void(float)>& callback, float interval, const std::string& key);

	/**
	*逻辑定时器
	*@param callback:回调函数
	*@param interval:更新间隔(秒
	*@param repeat:更新次数
	*@param delay:第一次更新的延迟时间
	*@param key:键值
	*@return 无
	*/
	void logicSchedule(const std::function<void(float)>& callback, float interval, int repeat, float delay, const std::string& key);

	/**
	*逻辑定时器,只更新一次
	*@param callback:回调函数
	*@param delay:第一次更新的延迟时间
	*@param key:键值
	*@return 无
	*/
	void logicScheduleOnce(const std::function<void(float)>& callback, float delay, const std::string& key);

	/**
	*取消逻辑定时器
	*@param key:键值
	*@return 无
	*/
	void logicUnschedule(const std::string& key);
private:
	struct SchedulePack
	{
		std::function<void(float)> callBack;
		int repeat;
		float delay;
		float interval;
		float timeCount;
	};

	std::map<std::string, SchedulePack> scheduleMap;
};

/*
主线程的定时器
*/
class CocosSchedule
{
public:
	/**
	*逻辑定时器的主更新
	*@param dt:距离上一次更新的时间
	*@return 无
	*/
	void cocosScheduleUpdate(float dt);

	/**
	*逻辑定时器,每一帧都更新
	*@param callback:回调函数
	*@param key:键值
	*@return 无
	*/
	void cocosSchedule(const std::function<void(float)>& callback, const std::string& key);

	/**
	*逻辑定时器
	*@param callback:回调函数
	*@param interval:更新间隔(秒
	*@param key:键值
	*@return 无
	*/
	void cocosSchedule(const std::function<void(float)>& callback, float interval, const std::string& key);

	/**
	*逻辑定时器
	*@param callback:回调函数
	*@param interval:更新间隔(秒
	*@param repeat:更新次数
	*@param delay:第一次更新的延迟时间
	*@param key:键值
	*@return 无
	*/
	void cocosSchedule(const std::function<void(float)>& callback, float interval, int repeat, float delay, const std::string& key);

	/**
	*逻辑定时器,只更新一次
	*@param callback:回调函数
	*@param delay:第一次更新的延迟时间
	*@param key:键值
	*@return 无
	*/
	void cocosScheduleOnce(const std::function<void(float)>& callback, float delay, const std::string& key);

	/**
	*取消逻辑定时器
	*@param key:键值
	*@return 无
	*/
	void cocosUnschedule(const std::string& key);
private:
	struct SchedulePack
	{
		std::function<void(float)> callBack;
		int repeat;
		float delay;
		float interval;
		float timeCount;
	};

	std::map<std::string, SchedulePack> scheduleMap;
};


#endif // !__LOGIC_SCHEDULE_H__
