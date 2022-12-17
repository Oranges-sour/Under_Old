#include "GameMap.h"

#include <array>
#include <functional>

#include "GameDefault.h"
#include "GameManager.h"
#include "MyMath.h"
#include "MyResourcesDef.h"
#include "PhysicsShapeCache.h"
#include "Random.h"
#include "ResourcesManager.h"
using namespace std;

/*>>地图变换规定<<
 * 地图数组中的左下角(0,0)的块显示出来(屏幕位置)必定在左下角
 * 地图数组中的右上角(mapWidth - 1, mapHeight -
 * 1)的块显示出来(屏幕位置)必定在右上角
 */

GameMap* GameMap::create() {
    GameMap* gm = new (std::nothrow) GameMap();
    if (gm) {
        gm->autorelease();
        return gm;
    }
    return nullptr;
}

void GameMap::initMap(UINT width_tile, UINT height_tile, float tilePixelSize,
                      unsigned int seed, const string& physicsBody) {
    this->mapWidth = width_tile;
    this->mapHeight = height_tile;
    this->tilePixelSize = tilePixelSize;

    mapStruct.init(width_tile, height_tile, tilePixelSize, seed);

    createTileSprite();
    this->physicsBody = physicsBody;
    setTilePhysicsBody(physicsBody);

    this->setAnchorPoint(Vec2(0, 0));
    this->setPosition(0, 0);
}

Size GameMap::getMapPixleSize() {
    return Size(mapWidth * tilePixelSize, mapHeight * tilePixelSize);
}

Size GameMap::getMapTileSize() { return Size(mapWidth, mapHeight); }

iVec2 GameMap::convertInMap(const Vec2& position) {
    const auto size = getMapPixleSize();
    if (Rect(Vec2(0, 0), size).containsPoint(position)) {
        return iVec2(static_cast<int>(position.x / tilePixelSize),
                     static_cast<int>(position.y / tilePixelSize));
    }
    return iVec2(-1, -1);
}

std::pair<Vec2, Vec2> GameMap::convertInPixel(const iVec2& position) {
    const Vec2 leftBottom(tilePixelSize * position.x,
                          tilePixelSize * position.y);
    const Vec2 rightTop(tilePixelSize * position.x + 1,
                        tilePixelSize * position.y + 1);
    return std::pair<Vec2, Vec2>(leftBottom, rightTop);
}

GameMapTile& GameMap::at(const iVec2& position) {
    return mapStruct.at(static_cast<UINT>(position.x),
                        static_cast<UINT>(position.y));
}

bool GameMap::tileSameWith(const iVec2& from, const iVec2& to,
                           GameMapTileType type) {
    bool isSame = true;
    for (int x = from.x; x <= to.x; ++x) {
        for (int y = from.y; y <= to.y; ++y) {
            if (at(iVec2(x, y)).type != type) {
                isSame = false;
                break;
            }
        }
    }
    return isSame;
}

pair<bool, GameMapTile*> GameMap::findMapTile(const iVec2& from,
                                              const iVec2& to) {
    CCASSERT((from.x == to.x || from.y == to.y), "只能搜索一条");
    CCASSERT((from.x <= to.x && from.y <= to.y), "from必须小于to");

    if (from.x == to.x) {
        // 纵向搜索
        for (int y = from.y; y <= to.y; ++y) {
            auto& tile = at(iVec2(from.x, y));
            // 空气块不算数!
            if (tile.type != GameMapTileType::air) {
                return pair<bool, GameMapTile*>(true, &tile);
            }
        }
    } else {
        // 横向搜索
        for (int x = from.x; x <= to.x; ++x) {
            auto& tile = at(iVec2(x, from.y));
            // 空气块不算数!
            if (tile.type != GameMapTileType::air) {
                return pair<bool, GameMapTile*>(true, &tile);
            }
        }
    }
    return pair<bool, GameMapTile*>(false, nullptr);
}

void GameMap::_update() { updatePhysicsTile(); }

const vector<basic_GameMapBuilding*>& GameMap::getMapBuildings() {
    return buildings;
}

bool GameMap::isInitFinish() { return initFinish; }

pair<iVec2, iVec2> GameMap::getInitProgress() {
    // 计算进度
    if (init_Height >= 0) {
        iVec2 w(init_Width, mapBlockWidth - 1);
        iVec2 h(mapBlockHeight - 1 - init_Height, mapBlockHeight - 1);
        return pair<iVec2, iVec2>(w, h);
    }
    iVec2 w(mapBlockWidth - 1, mapBlockWidth - 1);
    iVec2 h(mapBlockHeight - 1, mapBlockHeight - 1);
    return pair<iVec2, iVec2>(w, h);
}

void GameMap::createTileSprite() {
    // 延时生成,防止内存占用过多
    // 由于材质的内存占用过大,所有的地图材质都会被缩小渲染

    vector<vector<Sprite*>> tiles;
    tiles.resize(mapHeight, vector<Sprite*>(mapWidth));

    // 一个大图的瓦片数量的宽和高
    constexpr int mapBlockTileSize = 32;

    // 渲染时地图的真实大小: size = realSize / scaleRate,为了节省内存
    constexpr unsigned int scaleRate = 2;

    // 计算一个大图的大小
    mapBlockSize = tilePixelSize * mapBlockTileSize;

    // 计算横竖的大图数量
    mapBlockWidth =
        MyMath::floatRoundInt(mapWidth / static_cast<float>(mapBlockTileSize));
    mapBlockHeight =
        MyMath::floatRoundInt(mapHeight / static_cast<float>(mapBlockTileSize));
    mapBlocks.resize(mapBlockHeight, vector<Sprite*>(mapBlockWidth));
    for (int x = 0; x < mapBlockWidth; ++x) {
        for (int y = 0; y < mapBlockHeight; ++y) {
            auto sp = Sprite::create();
            mapBlocks[y][x] = sp;
            sp->setAnchorPoint(Vec2(0, 1));
            sp->setScaleY(-static_cast<float>(scaleRate));
            sp->setScaleX(static_cast<float>(scaleRate));
            this->addChild(sp, 2);
        }
    }

    for (int x = 0; x < mapBlockWidth; ++x) {
        for (int y = 0; y < mapBlockHeight; ++y) {
            auto& bigMap = mapBlocks[y][x];
            bigMap->setPosition(Vec2(mapBlockSize * x, y * mapBlockSize));
        }
    }

    init_Width = 0;
    init_Height = 0;

    this->schedule(
        [scaleRate, mapBlockTileSize, this](float) {
            // 渲染地图
            renderMap(scaleRate, mapBlockTileSize);
            ++init_Width;

            if (init_Width == mapBlockWidth) {
                init_Width = 0;
                ++init_Height;
            }

            if (init_Height == mapBlockHeight) {
                initFinish = true;
                this->unschedule("initMap");
            }
        },
        "initMap");
}

void GameMap::setTilePhysicsBody(const string& physicsBody) {
    physicsTiles.resize(mapHeight, vector<GameMapPhysicsTile>(mapWidth));

    for (int x = 0; x < mapWidth; ++x) {
        for (int y = 0; y < mapHeight; ++y) {
            // 只有方块不是空气,才需要物理碰撞
            auto& tile = physicsTiles[y][x];
            if (mapStruct.at(x, y).type != GameMapTileType::air)
                tile.needPhysics = true;
            else
                tile.needPhysics = false;
            tile.physicsTile = nullptr;
            tile.posInMap.x = x;
            tile.posInMap.y = y;
        }
    }
}

void GameMap::updatePhysicsTile() {
    // 物理模拟会停留的帧数
    constexpr int PHYSICS_STAY_FRAMES = 20;

    auto sprites = GameManager::getInstance()->getAllSprites();
    for (auto& s : sprites) {
        auto& a = s.second;
        for (auto& sp : a) {
            if (!sp->getPhysicsInfo().needPhysics) {
                continue;
            }
            const auto spriteBox = sp->getBoundingBox();
            // 获得精灵左上角坐标
            const auto spriteLeftBottomPos =
                Vec2(spriteBox.getMinX(), spriteBox.getMinY());
            // 右下角
            const auto spriteRightTopPos =
                Vec2(spriteBox.getMaxX(), spriteBox.getMaxY());

            auto mapLeftBottomPos = this->convertInMap(spriteLeftBottomPos);
            auto mapRightTopPos = this->convertInMap(spriteRightTopPos);

            for (int x = mapLeftBottomPos.x; x <= mapRightTopPos.x; ++x) {
                for (int y = mapLeftBottomPos.y; y <= mapRightTopPos.y; ++y) {
                    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
                        auto& pt = physicsTiles[y][x];
                        if (pt.needPhysics) {
                            pt.count = PHYSICS_STAY_FRAMES;
                            pt.isInDirty = true;
                            dirtyPhysicsTiles.push_back(&pt);
                        }
                    }
                }
            }
        }
    }

    // 需要从dirty中删除的
    vector<GameMapPhysicsTile*> needToErase;
    for (int x = 0; x < dirtyPhysicsTiles.size(); ++x) {
        auto& pt = dirtyPhysicsTiles[x];
        // 计数不为0,准备创建物理模拟块
        if (pt->count > 0) {
            // 发现之前没有创建过,开始创建
            if (pt->physicsTile == nullptr) {
                auto pyt = GameMapPhysicsObject::create();

                GameSpritePhysicsInfo pyInfo;
                pyInfo.physicsShape = physicsBody;
                pyInfo.activeContact = false;
                pyInfo.gravityEffect = false;

                pyt->setPhysicsInfo(pyInfo);
                pyt->setAnchorPoint(Vec2(0, 0));
                pt->physicsTile = pyt;
                this->addChild(pyt, 2);
            }
        } else {
            // 该删除了
            if (pt->physicsTile != nullptr) {
                pt->physicsTile->removeFromParent();
                pt->physicsTile = nullptr;
            }
            needToErase.push_back(pt);
            pt->isInDirty = false;
        }
    }

    for (auto it = needToErase.begin(); it != needToErase.end(); ++it) {
        dirtyPhysicsTiles.erase(
            remove(dirtyPhysicsTiles.begin(), dirtyPhysicsTiles.end(), *it),
            dirtyPhysicsTiles.end());
    }

    // 先对不为0的计数值进行相减操作
    const auto subPhysicsTileCount = [&]() {
        const auto doSub = [&](UINT from, UINT to, UINT) {
            for (int x = from; x < to; ++x) {
                auto& c = dirtyPhysicsTiles[x]->count;
                if (c > 0) {
                    c -= 1;
                }
            }
        };
        MyTool::callInThread(dirtyPhysicsTiles.size(), doSub);
    };

    const auto setPhysicsTilePos = [&]() {
        const auto doSet = [&](UINT from, UINT to, UINT) {
            for (int x = from; x < to; ++x) {
                auto& phyTile = dirtyPhysicsTiles[x];
                phyTile->physicsTile->setPosition(
                    Vec2(phyTile->posInMap.x * tilePixelSize,
                         phyTile->posInMap.y * tilePixelSize));
            }
        };
        MyTool::callInThread(dirtyPhysicsTiles.size(), doSet);
    };

    subPhysicsTileCount();
    setPhysicsTilePos();
}

GameMap::TileTextureBlock GameMap::getTileTextureBlock(const iVec2& position) {
    auto gameMana = GameManager::getInstance();
    auto resMana = gameMana->getResourcesManager();

    TileTextureBlock block;
    // 初始化
    block.first = nullptr;
    for (auto& it : block.second) {
        it = nullptr;
    }

    string tileSpriteName;
    switch (at(position).type) {
        case GameMapTileType::dirt:
            tileSpriteName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapDirt)[0];
            block.first = Sprite::createWithSpriteFrameName(tileSpriteName);
            break;
        case GameMapTileType::grass:
            tileSpriteName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapGrass)[0];
            block.first = Sprite::createWithSpriteFrameName(tileSpriteName);
            break;
        case GameMapTileType::rock:
            tileSpriteName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapRock)[0];
            block.first = Sprite::createWithSpriteFrameName(tileSpriteName);
            break;
        default:
            break;
    }

    // 如果这个块是空气块,也就不需要遮罩了
    if (block.first == nullptr) {
        return block;
    }

    if (position.x - 1 >= 0 && position.y + 1 < mapHeight) {
        if (at(iVec2(position.x - 1, position.y)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x - 1, position.y + 1)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x, position.y + 1)).type ==
                GameMapTileType::air) {
            string coverName;
            coverName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapCover)[0];
            block.second[0] = Sprite::createWithSpriteFrameName(coverName);
        }
    }

    if (position.x + 1 < mapWidth && position.y + 1 < mapHeight) {
        if (at(iVec2(position.x + 1, position.y)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x + 1, position.y + 1)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x, position.y + 1)).type ==
                GameMapTileType::air) {
            string coverName;
            coverName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapCover)[1];
            block.second[1] = Sprite::createWithSpriteFrameName(coverName);
        }
    }

    if (position.x + 1 < mapWidth && position.y - 1 >= 0) {
        if (at(iVec2(position.x + 1, position.y)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x + 1, position.y - 1)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x, position.y - 1)).type ==
                GameMapTileType::air) {
            string coverName;
            coverName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapCover)[2];
            block.second[2] = Sprite::createWithSpriteFrameName(coverName);
        }
    }

    if (position.x - 1 >= 0 && position.y - 1 >= 0) {
        if (at(iVec2(position.x - 1, position.y)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x - 1, position.y - 1)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x, position.y - 1)).type ==
                GameMapTileType::air) {
            string coverName;
            coverName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapCover)[3];
            block.second[3] = Sprite::createWithSpriteFrameName(coverName);
        }
    }

    return block;
}

void GameMap::renderMap(unsigned int scaleRate, int mapBlockTileSize) {
    const int startX = init_Width * mapBlockTileSize;
    const int endX = (init_Width + 1) * mapBlockTileSize;
    const int startY = init_Height * mapBlockTileSize;
    const int endY = (init_Height + 1) * mapBlockTileSize;
    // 遍历的范围为: [startX, endX) 和 [startY, endY)

    vector<vector<TileTextureBlock>> tiles;
    tiles.resize(mapBlockTileSize, vector<TileTextureBlock>(mapBlockTileSize));

    // 创建临时地图块
    for (int x = startX, dx = 0; x < endX; ++x, ++dx) {
        for (int y = startY, dy = 0; y < endY; ++y, ++dy) {
            auto block = getTileTextureBlock(iVec2(x, y));
            tiles[dy][dx] = block;

            auto sp = block.first;
            if (sp) {
                sp->setAnchorPoint(Vec2(0, 0));
                const float rate = 1.0f / scaleRate;
                sp->setScale(rate);
                sp->setPosition(Vec2(dx * tilePixelSize, dy * tilePixelSize) *
                                rate);
            }
            for (auto& it : block.second) {
                if (it) {
                    it->setAnchorPoint(Vec2(0, 0));
                    const float rate = 1.0f / scaleRate;
                    it->setScale(rate);
                    it->setPosition(
                        Vec2(dx * tilePixelSize, dy * tilePixelSize) * rate);
                    it->setBlendFunc({GL_DST_COLOR, GL_ZERO});
                }
            }
        }
    }

    // 开始渲染
    auto rt = RenderTexture::create(mapBlockSize / scaleRate,
                                    mapBlockSize / scaleRate);
    rt->beginWithClear(0, 0, 0, 0);
    //////////////////////////////////////////////////////////////////////////
    for (int x = 0; x < mapBlockTileSize; ++x) {
        for (int y = 0; y < mapBlockTileSize; ++y) {
            auto& block = tiles[y][x];
            if (block.first) {
                block.first->visit();
            }
            for (auto& it : block.second) {
                if (it) {
                    it->visit();
                }
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    rt->end();
    // 将刚刚渲染好的地图存储到数组中
    auto& block = mapBlocks[init_Height][init_Width];
    block->setSpriteFrame(rt->getSprite()->getSpriteFrame());
    block->getTexture()->setAliasTexParameters();
}

void GameMapStruct::init(UINT width, UINT height, float tilePixelSize,
                         unsigned int seed) {
    this->width = width;
    this->height = height;
    _map.resize(height, vector<GameMapTile>(width));
    initMapBlock(seed);

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            _map[y][x].tileRect =
                Rect(Vec2(x * tilePixelSize, y * tilePixelSize),
                     Size(tilePixelSize, tilePixelSize));
        }
    }
}

GameMapTile& GameMapStruct::at(UINT x, UINT y) { return _map[y][x]; }

void GameMapStruct::initMapBlock(unsigned int seed) {
    Random random(seed);
    // 测试
    /*for (int y = 0; y < height; ++y)
        {
                for (int x = 0; x < width; ++x)
                {
                        _map[y][x].type = GameMapBlockType::dirt;
                }
        }
        return;*/
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    this->initMapBlock_Prelin(random);

    // 遍历所有地图块
    const auto rangeAll = [this](const function<void(int, int)>& func) {
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                func(x, y);
            }
        }
    };

    // 随机空气块
    /*rand_int r0(1, 25);
    rangeAll([&](int x, int y) {
        _map[y][x].type = GameMapTileType::dirt;
        if (r0(random) == 1) {
            _map[y][x].type = GameMapTileType::air;
        }
    });*/

    // 拓展空气块
    /*rand_int r1(1, 3);
    rand_int r2(0, 2);
    rangeAll([&](int x, int y) {
        if (_map[y][x].type == GameMapTileType::air) {
            int xAdd = r1(random);
            int yAdd = r2(random);
            for (int my = y - yAdd; my < y + yAdd; ++my) {
                for (int mx = x - xAdd + 1; mx < x + xAdd; ++mx) {
                    if (my >= 0 && my < height && mx >= 0 && mx < width) {
                        _map[my][mx].type = GameMapTileType::air;
                    }
                }
            }
        }
    });*/

    // 防止一格高的路
    rangeAll([&](int x, int y) {
        if (y - 2 >= 0 && y + 2 < height) {
            /*形状图例:(X是判断的基准块)
            ####      ####
            #OOO      OOOO
            OOOO  ->  OOOO
            OX##      OX##
            O###      O###
            */
            if (x - 1 >= 0) {
                // 左上
                if (_map[y][x].type == GameMapTileType::dirt &&
                    _map[y + 1][x].type == GameMapTileType::air &&
                    _map[y + 1][x - 1].type == GameMapTileType::air &&
                    _map[y + 2][x - 1].type == GameMapTileType::dirt) {
                    _map[y + 2][x - 1].type = GameMapTileType::air;
                }
            }

            /*形状图例:(X是判断的基准块)
            ####      ####
            OOO#      OOOO
            OOOO  ->  OOOO
            ##XO      ##XO
            ###O      ###O
            */
            if (x + 1 < width) {
                // 右上
                if (_map[y][x].type == GameMapTileType::dirt &&
                    _map[y + 1][x].type == GameMapTileType::air &&
                    _map[y + 1][x + 1].type == GameMapTileType::air &&
                    _map[y + 2][x + 1].type == GameMapTileType::dirt) {
                    _map[y - 2][x + 1].type = GameMapTileType::air;
                }
            }
        }

        if (y - 1 >= 0 && y + 1 < height) {
            /*形状图例: #:泥土  O:空气
            #######      OOOOOOO
            OOOOOOO  ->  OOOOOOO
            #######      OOOOOOO
            */
            if (_map[y][x].type == GameMapTileType::air &&
                _map[y - 1][x].type == GameMapTileType::dirt &&
                _map[y + 1][x].type == GameMapTileType::dirt) {
                _map[y - 1][x].type = GameMapTileType::air;
                _map[y + 1][x].type = GameMapTileType::air;
            }
        }

        if (x - 1 >= 0 && x + 1 < width) {
            /*形状图例: #:泥土  O:空气
            #O#    OOO
            #O# -> OOO
            #O#    OOO
            */
            if (_map[y][x].type == GameMapTileType::air) {
                if (_map[y][x - 1].type == GameMapTileType::dirt &&
                    _map[y][x + 1].type == GameMapTileType::dirt) {
                    _map[y][x - 1].type = GameMapTileType::air;
                    _map[y][x + 1].type = GameMapTileType::air;
                }
            }
        }
    });

    // 随机岩石
    rand_int r3(0, 3);
    rangeAll([&](int x, int y) {
        if (_map[y][x].type == GameMapTileType::dirt) {
            int temp = 0;
            // 随机与边缘泥土块的距离
            int dis = r3(random);

            if (y - dis >= 0) {
                if (_map[y - dis][x].type != GameMapTileType::air) {
                    ++temp;
                }
            }

            if (y + dis < height) {
                if (_map[y + dis][x].type != GameMapTileType::air) {
                    ++temp;
                }
            }

            if (x - dis >= 0) {
                if (_map[y][x - dis].type != GameMapTileType::air) {
                    ++temp;
                }
            }

            if (x + dis < width) {
                if (_map[y][x + dis].type != GameMapTileType::air) {
                    ++temp;
                }
            }

            if (temp == 4) {
                _map[y][x].type = GameMapTileType::rock;
            }
        }
    });

    // 草地
    rangeAll([&](int x, int y) {
        if (_map[y][x].type == GameMapTileType::dirt) {
            if (y + 1 < height) {
                if (_map[y + 1][x].type == GameMapTileType::air) {
                    _map[y][x].type = GameMapTileType::grass;
                }
            }
        }
    });

    // 测试区域掏空
    for (int x = 0; x < 25; ++x) {
        for (int y = 0; y < 25; ++y) {
            _map[x][y].type = GameMapTileType::air;
        }
    }

    // 边界两格泥土填充

    // 左纵
    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < height; ++y) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    // 右纵
    for (int x = width - 2; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    // 横下
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < width; ++x) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    // 横上
    for (int y = height - 2; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    _map[0][0].type = GameMapTileType::grass;
}

void GameMapStruct::initMapBlock_Prelin(Random& random) {
    const int width = this->width;
    const int height = this->height;
    // 参数
    const int tunc = 110;
    const int octaveCount = 7;
    const float scalingBias = 0.3f;

    vector<float> noiseSeed(width * height, 0.0f);

    rand_float r0(0.0f, 1.0f);
    for (auto& it : noiseSeed) {
        it = r0(random);
    }

    vector<float> noise2D = move(this->perlinNoise2D(width, height, octaveCount,
                                                     scalingBias, noiseSeed));

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            float k = noise2D[y * width + x];

            int a = 255.0f * k;

            if (a > tunc) {
                this->_map[x][y].type = GameMapTileType::air;
            }
        }
    }
}

vector<float> GameMapStruct::perlinNoise2D(int width, int height, int octaves,
                                           float bias,
                                           vector<float>& noiseSeed) {
    vector<float> output(width * height);

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            float fNoise = 0.0f;
            float fScaleAcc = 0.0f;
            float fScale = 1.0f;

            for (int o = 0; o < octaves; o++) {
                int nPitch = width >> o;
                int nSampleX1 = (x / nPitch) * nPitch;
                int nSampleY1 = (y / nPitch) * nPitch;

                int nSampleX2 = (nSampleX1 + nPitch) % width;
                int nSampleY2 = (nSampleY1 + nPitch) % width;

                float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
                float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

                float fSampleT =
                    (1.0f - fBlendX) *
                        noiseSeed[nSampleY1 * width + nSampleX1] +
                    fBlendX * noiseSeed[nSampleY1 * width + nSampleX2];
                float fSampleB =
                    (1.0f - fBlendX) *
                        noiseSeed[nSampleY2 * width + nSampleX1] +
                    fBlendX * noiseSeed[nSampleY2 * width + nSampleX2];

                fScaleAcc += fScale;
                fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
                fScale = fScale / bias;
            }

            // Scale to seed range
            output[y * width + x] = fNoise / fScaleAcc;
        }
    return move(output);
}

GameMapPhysicsObject* GameMapPhysicsObject::create() {
    auto pt = new (std::nothrow) GameMapPhysicsObject();
    if (pt && pt->initGameSprite()) {
        pt->autorelease();
        return pt;
    }
    CC_SAFE_DELETE(pt);
    return nullptr;
}

bool GameMapPhysicsObject::initGameSprite() {
    basic_GameSprite::initGameSprite();
    Sprite::initWithSpriteFrameName(ResKey::Frame::PHYSICS_SHOW);
    // 被撞击时是否高亮显示
    this->setVisible(false);
    return true;
}

const Rect& GameMapTile::getRect() { return tileRect; }
