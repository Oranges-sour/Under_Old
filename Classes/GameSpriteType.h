#ifndef __GAME_SPRITE_TYPE_H__
#define __GAME_SPRITE_TYPE_H__

struct GameSpriteType {
    enum class Type0 {
        unknow,
        mapTile,   //游戏有物理碰撞的地图块
        hero,      //英雄
        enemy,     //敌人
        item,      //游戏物品
        bullet,    //子弹
        building,  //建筑
        other      //其他
    };

    enum class Type1 {
        unknow,
        heroBullet,  //英雄发出的子弹
        enemyBullet  //敌人发出的子弹
    };

    bool operator<(const GameSpriteType& other) const
    {
        return (type0 < other.type0);
    }
    Type0 type0;
    Type1 type1;
};

#endif