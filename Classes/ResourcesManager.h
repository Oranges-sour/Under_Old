#ifndef __RESOURCES_MANAGER_H__
#define __RESOURCES_MANAGER_H__



#include "cocos2d.h"
USING_NS_CC;

#include <string>
#include <vector>
#include <map>

/*游戏资源管理器.自动添加和删除资源
不同资源类型的键值可以重复,不互相影响.但同类型资源的一定不能一样
*/
class ResourcesManager
{
public:
	/**
	*构造,并设置是否在析构函数中自动释放资源
	*@param autoReleaseResources:是否自动释放资源
	*/
	ResourcesManager(bool autoReleaseResources = true);

	/**
	*析构,默认会自动释放资源,可在构造函数中设置
	*/
	~ResourcesManager();

	/**
	*添加shader资源数据,这一步会自动将shader载入cache中
	*@param vsh:顶点着色器文件名称
	*@param fsh:片段着色器文件名称
	*@param key:键值
	*@return 无
	*/
	void addShader(const std::string& vsh, const std::string& fsh, int key);

	/**
	*添加精灵表
	*@param filename:文件名称
	*@return 无
	*/
	void addSpriteSheet(const std::string& filename);

	/**
	*添加一组精灵帧,使用键值获取时将会返回这一整组精灵帧
	*这里的添加仅仅是精灵帧的文件名称,帮助索引.
	*精灵表需要调用ResourcesManager::addSpriteSheet(...)来添加
	*@param frames:一组精灵帧
	*@param key:键值
	*@return 无
	*/
	void addSpriteFrames(const std::vector<std::string>& frames, int key);

	/**
	*添加材质,并添加进内存
	*@param filename:文件名称
	*@param key:键值
	*@return 无
	*/
	void addTexture(const std::string& filename, int key);

	/**
	*添加精灵的物理形状(plist)
	*@param filename:文件名称
	*@return 无
	*/
	void addPhysicsShapes(const std::string& filename);

	/**
	*添加一个精灵的物理Body的名称
	*@param name:Body的名称
	*@param key:键值
	*@return 无
	*/
	void addPhysicsBody(const std::string& name, int key);

	/**
	*获得一个精灵物理Body
	*@param key:键值
	*@return Body的名称
	*/
	const std::string& getPhysicsBody(int key);

	/**
	*获得一个shader
	*@param key:键值
	*@return GLProgram的指针
	*/
	GLProgram* getShader(int key);

	/**
	*获得一组精灵帧
	*@param key:键值
	*@return 一组精灵帧
	*/
	const std::vector<std::string>& getSpriteFrames(int key);

	/**
	*获得材质名称
	*@param key:键值
	*@return 材质名称
	*/
	const std::string& getTexture(int key);

	/**
	*释放资源管理器,并释放所有由此ResourcesManager载入的资源.
	*会默认在析构函数调用,可在构造函数中决定是否自动调用
	*包括材质,精灵表,shader等
	*@param 无
	*@return 无
	*/
	void releaseResources();
private:
	std::map<int, GLProgram*> shaderPool;
	std::map<int, std::string> texturePool;
	std::map<int, std::string> physicsBodyPool;
	std::map<int, std::vector<std::string>> spriteFramePool;
	std::vector<std::string> spriteSheetPool;
	std::vector<std::string> physicsShapePool;

	bool autoReleaseResources = true;
};

#endif
