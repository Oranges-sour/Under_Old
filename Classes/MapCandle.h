#ifndef __MAP_GANDLE_H__
#define __MAP_GANDLE_H__

#include "cocos2d.h"
USING_NS_CC;

#include <set>
#include <string>
#include <vector>
using namespace std;

#include "basic_GameSprite.h"

class CandleStar;

class MapCandle : public basic_GameSprite {
public:
    static MapCandle* create(const string& frame, const string& star,
                             const string& par, const string& candleLight,
                             const string& starLight);
    virtual bool initGameSprite(const string& frame, const string& star,
                                const string& par, const string& candleLight,
                                const string& starLight);

    void removeStar(CandleStar* star);

    void _update();

    void createStar();

private:
    bool canCreate = false;
    string starFrame;
    string particle;
    string starLight;
    vector<CandleStar*> needToErase;
    set<CandleStar*> stars;
};

class CandleStar : public basic_GameSprite {
public:
    static CandleStar* create(MapCandle* candle, const string& frame,
                              const string& light, const string& par);
    virtual bool initGameSprite(MapCandle* candle, const string& frame,
                                const string& light, const string& par);

    void _update();

    bool canKill();

    void createPar();

    virtual void kill() override;

private:
    bool _canKill = false;
    string particle;
    MapCandle* candle;
};

#endif