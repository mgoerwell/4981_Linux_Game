/*
 * Created by Matt Goerwell, March 13 2017
 */
#ifndef GHOST_H
#define GHOST_H
#include <string>
#include "../sprites/LTexture.h"
#include "../basic/Entity.h"
#include "../basic/Movable.h"
#include "../collision/CollisionHandler.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "../view/Window.h"

#define GHOST_VELOCITY 500

/*
 * This is a simple class that defines a movable entity to allow spectators to move around the map.
 */
class Ghost : public Movable {
public:
    //ctor
    Ghost(int32_t id, const SDL_Rect dest);
    //dtor
    virtual ~Ghost();
    //required by parent
    void onCollision();
    //required by parent
    void collidingProjectile(int damage);
    //ghost specific movement rules.
    void move(float moveX, float moveY, CollisionHandler &ch) override;
};

#endif
