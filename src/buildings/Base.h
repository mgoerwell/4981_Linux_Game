#ifndef BASE_H
#define BASE_H
//#include <string>
//#include "LTexture.h"
//#include "HitBox.h"
#include "../basic/Entity.h"
#include "Object.h"
//#include "CollisionHandler.h"
//#include "Player.h"
//#include <vector>
//#include <map>
//#include <utility>
//#include <SDL2/SDL.h>

//TODO :  change later.
constexpr int BASE_HEIGHT = 500;
constexpr int BASE_WIDTH = 500;
constexpr int MAP_HEIGHT = 2000;
constexpr int MAP_WIDTH = 2000;

typedef std::pair<float, float> Point;

class Base : public Object {
public:
    Base(int32_t nid = 0, const SDL_Rect dest = {1000, 1000, BASE_WIDTH, BASE_HEIGHT}, int health = 100);
    virtual ~Base();

    void onCollision();
    void collidingProjectile(int damage);
    Point getSpawnPoint();

private:
    int health;

};
#endif
