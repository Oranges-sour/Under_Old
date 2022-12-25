#ifndef __MY_RESOURCES_DEF_H__
#define __MY_RESOURCES_DEF_H__

#include <string>

namespace ResKey {
typedef int ResourceKey;

/************************************************************************/
/*                        游戏地图键值                                    */
/************************************************************************/
namespace SpFrame {
enum {
    GameMapGrass = 1,
    GameMapDirt,
    GameMapRock,
    GameMapCover,
    Joystick_Move_1,
    Joystick_Move_2,
    Joystick_Attack_1,
    Joystick_Attack_2,
    Joystick_Disable,

    Hero_Stay,
    Hero_Run,
    Enemy0,
    HeroBullet,
    MapCandle,
    Item0,
    Particle
};
}

/************************************************************************/

/************************************************************************/
/*                           着色器键值                                  */
/************************************************************************/

/************************************************************************/

/************************************************************************/
/*                           物理body键值                                */
/************************************************************************/
namespace Physics {
enum {
    GameMapTile = 1,
    Hero,
    HeroBullet,
    Enemy0,
    Enemy0Bullet,
    Item0,
};
}

/************************************************************************/

/************************************************************************/
/*                           光源材质键值                                */
/************************************************************************/

constexpr ResourceKey LIGHT_TEXTURE = 1;
constexpr ResourceKey GAME_MAP_BACK_GROUND = 2;

/************************************************************************/

//框架所需要的
namespace Frame {
//此图片的大小必须和地图瓦片大小一致!
const std::string PHYSICS_SHOW("physicsShow.png");
}  // namespace Frame
}  // namespace ResKey

#endif
