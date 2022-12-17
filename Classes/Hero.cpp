#include "Hero.h"

#include "GameItem.h"

HeroItemBag::HeroItemBag() {}

HeroItemBag::~HeroItemBag()
{
    for (auto& item : items)
        for (auto& it : item.second) delete it;
}

void HeroItemBag::addItem(basic_GameItem* item)
{
    const auto type = item->getItemType();
    auto iter = items.find(type);
    if (iter != items.end())
        iter->second.push_back(item);
    else
        items.insert({type, {item}});
}

void HeroItemBag::removeItem(basic_GameItem* item)
{
    const auto type = item->getItemType();
    auto iter = items.find(type);
    if (iter != items.end()) {
        auto& vec = iter->second;
        vec.erase(remove(vec.begin(), vec.end(), item), vec.end());
        delete item;
        if (vec.size() == 0) items.erase(iter);
    } else
        //未找见Item,不会内存释放,可能出现内存泄露
        CC_ASSERT(false);
}

const HeroItemBag::ItemMap& HeroItemBag::getAllItems() { return items; }

int basic_Hero::getHitPoint() { return hitPoint; }

void basic_Hero::setHitPoint(int hitPoint) { this->hitPoint = hitPoint; }

int basic_Hero::getEnergy() { return energy; }

void basic_Hero::setEnergy(int energy) { this->energy = energy; }

HeroItemBag& basic_Hero::getItemBag() { return itemBag; }

void basic_Hero::setActionType(HeroActionType type) { actionType = type; }

HeroActionType basic_Hero::getActionType() { return actionType; }

void basic_Hero::onContact(basic_GameSprite* contactTarget)
{
    //如果是游戏物品则直接处理
    if (contactTarget->getGameSpriteType().type0 ==
        GameSpriteType::Type0::item) {
        auto item = dynamic_cast<basic_GameItemSprite*>(contactTarget);
        item->itemPickedUp(this);
    }
}
