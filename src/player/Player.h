#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "../sprites/LTexture.h"
#include "../player/Marine.h"
#include "../player/Ghost.h"
#include "../turrets/Turret.h"
#include "../inventory/Inventory.h"
#include <SDL2/SDL.h>
#include "../game/GameManager.h"

constexpr int PLAYER_PLACE_DISTANCE = 100;

class Player {
public:


    void handleKeyboardInput(const Uint8 *state); // Handles player input with keyboard state
    void handleMouseUpdate(Window& w, float camX, float camY);

    void setControl(Marine& newControl);

    void handleMouseWheelInput(const SDL_Event *e);

    // Added by Mark.C 02/07/2017
    void handlePlacementClick(SDL_Renderer *renderer);

    void handleTempBarricade(SDL_Renderer *renderer);
    void handleTempTurret(SDL_Renderer *renderer);

    //Added by Matt Goerwell 03/14/2017
    Ghost& getGhost() {return ghost;}
    void moveGhost(const float delta);

    Player();
    ~Player();

    //Stays as pointer cause the player gets a marine object after the ctor is called
    Marine *marine = NULL;

private:
    int tempBarricadeID;
    int tempTurretID;
    bool holdingTurret;
    int pickupTick;
    int pickupDelay;
    //entity to control camera while player is dead.
    Ghost ghost;
};

#endif
