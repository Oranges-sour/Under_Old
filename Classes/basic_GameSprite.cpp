#include "basic_GameSprite.h"

#include "GameDefault.h"
#include "GameManager.h"
#include "MyMath.h"
#include "MyResourcesDef.h"
#include "PhysicsShapeCache.h"
#include "ResourcesManager.h"

using namespace std;

// void basic_GameSprite::addBuff(GameObjectBuffType objectBuffType, float time)
//{
//	//检测是否有此buff,有就增加时间,没有就添加新的
//	auto it = buffs.find(objectBuffType);
//	if (it != buffs.end())
//	{
//		it->second += time;
//	}
//	else
//	{
//		buffs.insert(pair<GameObjectBuffType, float>{ objectBuffType,
// time
//});
//	}
//}

// void basic_GameSprite::subBuff(GameObjectBuffType objectBuffType, float time)
//{
//	//检测是否有此buff,有就减少时间,没有就什么也不干
//	auto it = buffs.find(objectBuffType);
//	if (it != buffs.end())
//	{
//		//减少时间之后的剩余时间
//		float nowTime = it->second - time;
//		//减少完时间后如果时间没有剩余(小于0),就移除此buff
//		if (nowTime < 0)
//		{
//			buffs.erase(objectBuffType);
//		}
//		else
//		{
//			it->second = nowTime;
//		}
//	}
//}

// const map<GameObjectBuffType, float>& basic_GameSprite::getBuff()
//{
//	return buffs;
//}

// bool basic_GameSprite::isBuff(GameObjectBuffType objectBuffType, float time)
//{
//	auto it = buffs.find(objectBuffType);
//	if (it != buffs.end() && it->second >= time)
//	{
//		return true;
//	}
//	return false;
//}

void basic_GameSprite::onContact(basic_GameSprite* contactTarget) {}

void basic_GameSprite::getContact(basic_GameSprite* contactFrom) {}

void basic_GameSprite::kill() {
    GameManager::getInstance()->removeGameSprite(this);
}

GameSpriteType basic_GameSprite::getGameSpriteType() {
    return {GameSpriteType::Type0::unknow, GameSpriteType::Type1::unknow};
}

void basic_GameSprite::setPhysicsInfo(
    const GameSpritePhysicsInfo& physicsInfo) {
    PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo.physicsShape,
                                                      this);
    this->physicsInfo = physicsInfo;
}

GameSpritePhysicsInfo& basic_GameSprite::getPhysicsInfo() {
    return physicsInfo;
}

void basic_GameSprite::uploadPhysicsBody() {
    if (!physicsBody) {
        physicsBody = PhysicsShapeCache::getInstance()->createBodyWithName(
            physicsInfo.physicsShape);
        physicsBody->setName("__PhysicsBody");
        physicsBody->retain();
    }
    if (!this->getComponent("__PhysicsBody")) {
        this->addComponent(physicsBody);
    }
}

void basic_GameSprite::unloadPhysicsBody() {
    this->removeComponent(physicsBody);
}

const std::map<int, Sprite*>& basic_GameSprite::getAllLightSprites() {
    return lightSprites;
}

Sprite* basic_GameSprite::getLightSprite(int key) {
    auto iter = lightSprites.find(key);
    if (iter != lightSprites.end()) {
        return iter->second;
    }
    return nullptr;
}

void basic_GameSprite::addLightSprite(Sprite* lightSprite, int key) {
    lightSprites.insert({key, lightSprite});
    lightSprite->setVisible(false);
    this->addChild(lightSprite, -1);
}

void basic_GameSprite::removeLightSprite(int key) {
    auto iter = lightSprites.find(key);
    iter->second->removeFromParent();
    lightSprites.erase(iter);
}

bool basic_GameSprite::initGameSprite() {
    // this->logicSchedule([&](float dt)
    //	{
    //		//buff的时间减少
    //		for (auto it = buffs.begin(); it != buffs.end();)
    //		{
    //			//减少时间之后的剩余时间
    //			float nowTime = it->second - dt;
    //			//减少完时间后如果时间没有剩余(小于0),就移除此buff
    //			if (nowTime < 0)
    //				it = buffs.erase(it);
    //			else
    //			{
    //				it->second = nowTime;
    //				++it;
    //			}
    //		}
    //	}, "__updateBuff");

    return true;
}

void basic_GameSprite::cleanup() {
    for (auto& it : lightSprites) {
        it.second->removeFromParent();
    }
    if (physicsBody) {
        physicsBody->release();
    }

    Sprite::cleanup();
}

void basic_GameSprite::setPhysicsBoundingSize(const Size& size) {
    physicsBoundingSize = size;
}

Size basic_GameSprite::getPhysicsBoundingSize() { return physicsBoundingSize; }
