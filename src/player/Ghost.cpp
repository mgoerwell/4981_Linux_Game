#include "Ghost.h"
#include "../log/log.h"

Ghost::Ghost(int32_t id,const SDL_Rect dest) :Entity(id, dest), Movable(id, dest, GHOST_VELOCITY) {
    logv("Create Ghost\n");
}

Ghost::~Ghost() {
    logv("Destroy Ghost\n");
}
    
void Ghost::onCollision() {
    //do nothing
}

void Ghost::collidingProjectile(int damage) {
    //do nothing.
}

void Ghost::move(float moveX, float moveY, CollisionHandler &ch) {
    setX(getX() + moveX);
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this),this)){
        setX(getX() - moveX);
    }
    setY(getY() + moveY);
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this),this)){
        setY(getY() - moveY);
    }
}