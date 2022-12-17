#include "ResourcesManager.h"

#include "PhysicsShapeCache.h"
using namespace std;

ResourcesManager::ResourcesManager(bool autoReleaseResources)
{
	this->autoReleaseResources = autoReleaseResources;
}

ResourcesManager::~ResourcesManager()
{
	if (autoReleaseResources)
	{
		releaseResources();
	}
}

void ResourcesManager::addShader(const string& vsh, const string& fsh, int key)
{
	auto shader = GLProgram::createWithFilenames(vsh, fsh);
	shader->retain();
	shaderPool.insert({ key, shader });
}

void ResourcesManager::addSpriteSheet(const string& filename)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename);
	spriteSheetPool.push_back(filename);
}

void ResourcesManager::addSpriteFrames(const vector<string>& frames, int key)
{
	spriteFramePool.insert({ key,frames });
}

void ResourcesManager::addTexture(const std::string& filename, int key)
{
	Director::getInstance()->getTextureCache()->addImage(filename);
	texturePool.insert({ key,filename });
}

void ResourcesManager::addPhysicsShapes(const std::string& filename)
{
	PhysicsShapeCache::getInstance()->addShapesWithFile(filename);
	physicsShapePool.push_back(filename);
}

void ResourcesManager::addPhysicsBody(const std::string& name, int key)
{
	physicsBodyPool.insert({ key,name });
}

const std::string& ResourcesManager::getPhysicsBody(int key)
{
	return physicsBodyPool.find(key)->second;
}

GLProgram* ResourcesManager::getShader(int key)
{
	auto it = shaderPool.find(key);
	if (it != shaderPool.end())
	{
		return it->second;
	}
	return nullptr;
}

const std::vector<std::string>& ResourcesManager::getSpriteFrames(int key)
{
	return spriteFramePool.find(key)->second;
}

const std::string& ResourcesManager::getTexture(int key)
{
	return texturePool.find(key)->second;
}

void ResourcesManager::releaseResources()
{
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	for (auto it = spriteSheetPool.begin(); it != spriteSheetPool.end(); ++it)
	{
		spriteFrameCache->removeSpriteFramesFromFile((*it));
	}

	auto textureCache = Director::getInstance()->getTextureCache();
	for (auto it = texturePool.begin(); it != texturePool.end(); ++it)
	{
		textureCache->removeTextureForKey(it->second);
	}

	for (auto it = shaderPool.begin(); it != shaderPool.end(); ++it)
	{
		it->second->release();
	}

	auto physicsShapeCache = PhysicsShapeCache::getInstance();
	for (auto it = physicsShapePool.begin(); it != physicsShapePool.end(); ++it)
	{
		physicsShapeCache->removeShapesWithFile((*it));
	}

	/*spriteFramePool中的资源会自动释放,不需要手动管理.
	因为它仅仅是存储了精灵帧的名称,没有真实的资源.
	*/
}
