#include "Ghost.h"

Ghost::Ghost() : Movable(GHOST_VELOCITY) {
    printf("Create Ghost\n");
}

Ghost::~Ghost() {
    printf("Destroy Ghost\n");
}
    
void Ghost::onCollision() {
    //do nothing
}

void Ghost::collidingProjectile(int damage) {
    //do nothing.
}
