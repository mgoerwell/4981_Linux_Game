#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include<SDL2/SDL.h>
#include "Marine.h"
#include "Turret.h"
#include "CollisionHandler.h"
#include "Object.h"
#include "Zombie.h"
#include "Barricade.h"
#include <map>
#include <unordered_map>
#include <vector>
#include "WeaponDrop.h"
#include <memory>

//just for tesing weapon drop
#include "weapons/Weapon.h"
#include "weapons/HandGun.h"
#include "weapons/Rifle.h"
#include "weapons/ShotGun.h"

class GameManager {
public:
    static GameManager *instance();
    ~GameManager();

    int32_t generateID();

    void renderObjects(SDL_Renderer* gRenderer, const float camX, const float camY); // Render all objects in level

    // Methods for creating, getting, and deleting marines from the level.
    int32_t createMarine();
    bool createMarine(SDL_Renderer* gRenderer, const float x, const float y);
    void deleteMarine(const int32_t id);

    bool addMarine(const int32_t id, const Marine& newMarine);
    Marine& getMarine(const int32_t id);

    // Methods for creating, getting, and deleting towers from the level.
    int32_t createTurret();
    void deleteTurret(const int32_t id);

    bool addTurret(const int32_t id, const Turret& newTurret);
    int32_t createTurret(SDL_Renderer* gRenderer, const float x, const float y) ;
    Turret& getTurret(const int32_t id);

    // Method for getting collisionHandler
    CollisionHandler& getCollisionHandler();

    void updateCollider(); // Updates CollisionHandler
    void updateMarines(const float delta); // Update marine actions
    void updateZombies(const float delta); // Update zombie actions

    int32_t addObject(const Object&);
    void deleteObject(const int32_t id);

    int32_t addZombie(const Zombie&);
    bool createZombie(SDL_Renderer* gRenderer, const float x, const float y);
    void deleteZombie(const int32_t id);

    int32_t addWeaponDrop(const WeaponDrop& newWeaponDrop);
    bool createWeaponDrop(SDL_Renderer* gRenderer, const float x, const float y);
    void deleteWeaponDrop(const int32_t id);

    int32_t createBarricade(SDL_Renderer* gRenderer, const float x, const float y);
    void deleteBarricade(const int32_t id);
    Barricade& getBarricade(const int32_t id);
private:
    static GameManager *sInstance;

    GameManager();
    CollisionHandler collisionHandler;
    std::unique_ptr<WeaponDrop> wdPointer;
    std::map<int32_t, Marine> marineManager;
    std::map<int32_t, Object> objectManager;
    std::map<int32_t, Zombie> zombieManager;
    std::map<int32_t, Turret> turretManager;
    std::map<int32_t, WeaponDrop> weaponDropManager;
    std::map<int32_t, Barricade> barricadeManager;

};


#endif
