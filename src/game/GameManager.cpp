#include "GameManager.h"
#include "../collision/HitBox.h"
#include <memory>
#include <utility>
#include <atomic>
#include "../log/log.h"

Weapon w;
GameManager GameManager::sInstance;
//Returns the already existing GameManager or if there isn't one, makes
//a new one and returns it.
GameManager *GameManager::instance() {
    return &GameManager::sInstance;
}

int32_t GameManager::generateID() {
    static std::atomic<int32_t> counter{-1};
    return ++counter;
}

GameManager::GameManager():collisionHandler() {
    logv("Create GM\n");
}

GameManager::~GameManager() {
    logv("Destroy GM\n");
}

// Render all objects in level
void GameManager::renderObjects(SDL_Renderer* gRenderer, const float camX, const float camY) {

    for (const auto& m : weaponDropManager) {
        m.second.texture.render(gRenderer, m.second.getX() - camX, m.second.getY() - camY);
    }
    for (const auto& m : marineManager) {
        m.second.texture.render(gRenderer, m.second.getX() - camX, m.second.getY() - camY,
                nullptr, m.second.getAngle());
    }

    for (const auto& o : objectManager) {
            o.second.texture.render(gRenderer, o.second.getX() - camX, o.second.getY() - camY);

    }

    for (const auto& z : zombieManager) {
        z.second.texture.render(gRenderer, z.second.getX() - camX, z.second.getY() - camY,
                nullptr, z.second.getAngle());
    }

    for (const auto& m : turretManager) {
        m.second.texture.render(gRenderer, m.second.getX() - camX, m.second.getY() - camY,
                nullptr, m.second.getAngle());
    }

    for (const auto& b : barricadeManager) {
        b.second.texture.render(gRenderer, b.second.getX()-camX, b.second.getY()-camY);
    }

    for (const auto& w : wallManager) {
        w.second.texture.render(gRenderer, w.second.getX() - camX, w.second.getY() - camY);
    }

}

// Update marine movements. health, and actions
void GameManager::updateMarines(const float delta) {
    for (auto& m : marineManager) {
        m.second.move((m.second.getDX()*delta), (m.second.getDY()*delta), collisionHandler);
    }
}

// Update zombie movements.
void GameManager::updateZombies(const float delta) {
    for (auto& z : zombieManager) {
        z.second.generateRandomMove();
        z.second.move((z.second.getDX()*delta), (z.second.getDY()*delta), collisionHandler);
    }
}
// Create marine add it to manager, returns marine id
int32_t GameManager::createMarine() {
    const int32_t id = generateID();
    SDL_Rect marineRect = {0,0,100,100};
    SDL_Rect moveRect = {0,0,100,100};
    SDL_Rect projRect = {0,0,100,100};
    SDL_Rect damRect = {0,0,100,100};

    Marine m(id, marineRect, moveRect, projRect, damRect);
    marineManager.insert({id, m});
    return id;
}

bool GameManager::createMarine(SDL_Renderer* gRenderer, const float x, const float y){
    const int32_t id = generateID();
    SDL_Rect marineRect = {0,0,100,100};
    SDL_Rect moveRect = {0,0,100,100};
    SDL_Rect projRect = {0,0,100,100};
    SDL_Rect damRect = {0,0,100,100};

    Marine m(id, marineRect, moveRect, projRect, damRect);
    marineManager.insert({id, m});
    if(marineManager.count(id)){
        if (!marineManager.at(id).texture.loadFromFile("assets/texture/arrow.png", gRenderer)) {
            logv("Failed to load the marine texture!\n");
            deleteMarine(id);
            return false;
        }
        marineManager.at(id).setPosition(x,y);
        return true;
    }
    return false;
}

void GameManager::deleteMarine(const int32_t id) {
    marineManager.erase(id);
}


// Adds marine to level
bool GameManager::addMarine(const int32_t id, const Marine& newMarine) {
    if (marineManager.count(id)) {
        return false;
    }
    marineManager.insert({id,newMarine});
    return true;
}

// Get a marine by its id
Marine& GameManager::getMarine(const int32_t id) {
    return marineManager.find(id)->second;
}

// Create Turret add it to manager, returns tower id
int32_t GameManager::createTurret() {
    const int32_t id = generateID();
    SDL_Rect turretRect = {0,0,100,100};
    SDL_Rect moveRect = {0,0,100,100};
    SDL_Rect projRect = {0,0,100,100};
    SDL_Rect damRect = {0,0,100,100};
    SDL_Rect pickRect = {0,0,120,120};

    turretManager.insert({id, Turret(id, turretRect, moveRect, projRect, damRect, pickRect)});
    return id;
}

// Deletes tower from level
void GameManager::deleteTurret(const int32_t id) {
    turretManager.erase(id);
}

// Adds tower to level
bool GameManager::addTurret (const int32_t id, const Turret& newTurret) {
    if (turretManager.count(id)) {
        return false;
    }
    turretManager.insert({id, newTurret});
    return true;
}

// Create turret add it to turret, returns if success
int32_t GameManager::createTurret(SDL_Renderer* gRenderer, const float x, const float y) {
    const int32_t id = generateID();
    SDL_Rect turretRect = {0,0,100,100};
    SDL_Rect moveRect = {0,0,100,100};
    SDL_Rect projRect = {0,0,100,100};
    SDL_Rect damRect = {0,0,100,100};
    SDL_Rect pickRect = {0,0,120,120};

    turretManager.insert({id, Turret(id, turretRect, moveRect, projRect, damRect, pickRect)});
    if(turretManager.count(id)){
        if (!turretManager.at(id).texture.loadFromFile("assets/texture/turret.png", gRenderer)) {
            logv("Failed to load the turret texture!\n");
            deleteTurret(id);
            return -1;
        }
        turretManager.at(id).setPosition(x,y);
        return id;
    }
    return -1;
}

// Get a tower by its id
Turret& GameManager::getTurret(const int32_t id) {
    return turretManager.find(id)->second;
}

int32_t GameManager::addZombie(const Zombie& newZombie) {
    const int32_t id = generateID();

    zombieManager.insert({id,newZombie});
    return id;
}

// Create zombie add it to manager, returns success
bool GameManager::createZombie(SDL_Renderer* gRenderer, const float x, const float y) {
    const int32_t id = generateID();

    SDL_Rect zombieRect = {0,0,100,100};
    SDL_Rect moveRect = {0,0,100,100};
    SDL_Rect projRect = {0,0,100,100};
    SDL_Rect damRect = {0,0,100,100};


    zombieManager.insert({id, Zombie(id, zombieRect, moveRect, projRect, damRect)});
    if(zombieManager.count(id)){
        if (!zombieManager.at(id).texture.loadFromFile("assets/texture/zombie.png", gRenderer)) {
            logv("Failed to load the Zombie texture!\n");
            deleteZombie(id);
            return false;
        }
        zombieManager.at(id).setPosition(x,y);
        return true;
    }
    return false;
}

// Deletes zombie from level
void GameManager::deleteZombie(const int32_t id) {
    zombieManager.erase(id);

}

int32_t GameManager::addObject(const Object& newObject) {
    objectManager.insert({newObject.getId(), newObject});
    return newObject.getId();
}

// Deletes Object from level
void GameManager::deleteObject(const int32_t id) {
    objectManager.erase(id);
}


int32_t GameManager::addWeapon(std::shared_ptr<Weapon> weapon){

    const int32_t id = weapon->getId();
    weaponManager.insert({id, weapon});
    if(weaponManager.count(id)){
        weaponManager.at(id)->setId(id);
        return id;
    }
    return -1;

}

int32_t GameManager::addWeaponDrop(WeaponDrop& newWeaponDrop) {
    const int32_t id = newWeaponDrop.getId();

    weaponDropManager.insert({id, newWeaponDrop});
    return id;
}



// Create weapon drop add it to manager, returns success
bool GameManager::createWeaponDrop(SDL_Renderer* gRenderer, const float x, const float y) {

    Rifle w;
    const int32_t wid = w.getId();
    const int32_t id = generateID();

    SDL_Rect weaponDropRect = {static_cast<int>(x),static_cast<int>(y),100,100};
    SDL_Rect pickRect = {static_cast<int>(x),static_cast<int>(y),100,100};

    addWeapon(std::dynamic_pointer_cast<Weapon>(std::make_shared<Rifle>(w)));

    WeaponDrop wd(id, weaponDropRect, pickRect, wid);
    weaponDropManager.insert({id, wd});

    if(weaponDropManager.count(id)){
        weaponDropManager.at(id).setX(x);
        weaponDropManager.at(id).setY(y);

        if(!weaponDropManager.at(id).texture.loadFromFile("assets/texture/shotGun.png", gRenderer)) {
            logv("Failed to load the player texture!\n");
            deleteWeaponDrop(id);
            return false;
        }
        weaponDropManager.at(id).setPosition(x,y);
        return true;
    }
    return false;
}

//returns weapon drop in  weaponDropManager
WeaponDrop& GameManager::getWeaponDrop(const int32_t id){
    return weaponDropManager.at(id);
}

//returns weapon in weaponManager
std::shared_ptr<Weapon> GameManager::getWeapon(const int32_t id){

    if(weaponManager.count(id)){
        return weaponManager.at(id);
    }
    logv("Couldnt find Weapon\n");
    return nullptr;

}

// Deletes weapon from level
void GameManager::deleteWeaponDrop(const int32_t id) {

    const auto& it = weaponDropManager.find(id);
    if(it != weaponDropManager.end()){
        weaponDropManager.erase(id);
    } else {
        logv("Couldnt Delete Weapon Drop\n");
    }

}

// Returns Collision Handler
CollisionHandler& GameManager::getCollisionHandler() {
    return collisionHandler;
}

// Update colliders to current state
void GameManager::updateCollider() {
    collisionHandler = CollisionHandler();

    for (auto& m : marineManager) {
        collisionHandler.quadtreeMarine.insert(&m.second);
    }

    for (auto& z : zombieManager) {
        collisionHandler.quadtreeZombie.insert(&z.second);
    }

    for (auto& o : objectManager) {
        collisionHandler.quadtreeObj.insert(&o.second);
    }

    for (auto& m : turretManager) {
        if (m.second.isPlaced()) {
            collisionHandler.quadtreeTurret.insert(&m.second);
            collisionHandler.quadtreePickUp.insert(&m.second);
        }
    }

    for (auto& b : barricadeManager) {
        if (b.second.isPlaced()) {
            collisionHandler.quadtreeBarricade.insert(&b.second);
        }
    }

    for (auto& m : weaponDropManager) {
        collisionHandler.quadtreePickUp.insert(&m.second);
    }

    for (auto& w : wallManager) {
        collisionHandler.quadtreeWall.insert(&w.second);
    }
}

// Create barricade add it to manager, returns success
int32_t GameManager::createBarricade(SDL_Renderer* gRenderer, const float x, const float y) {
    const int32_t id = generateID();

    SDL_Rect barricadeRect = {0,0,100,100};
    SDL_Rect moveRect = {0,0,100,100};
    SDL_Rect pickRect = {0,0,100,100};

    Barricade b(id, barricadeRect, moveRect, pickRect);
    barricadeManager.insert({id, b});
    if(barricadeManager.count(id)){
        if (!barricadeManager.at(id).texture.loadFromFile("assets/texture/barricade.png", gRenderer)) {
            logv("Failed to load the barricade texture!\n");
            deleteBarricade(id);
            return -1;
        }
        barricadeManager.at(id).setPosition(x,y);
        return id;
    }
    return -1;
}


void GameManager::deleteBarricade(const int32_t id) {
    barricadeManager.erase(id);
}
// Get a barricade by its id
Barricade& GameManager::getBarricade(const int32_t id) {
    return barricadeManager.find(id)->second;
}

// Create zombie add it to manager, returns success
int32_t GameManager::createWall(SDL_Renderer* gRenderer,
        const float x, const float y, const int w, const int h) {

    const int32_t id = generateID();

    SDL_Rect wallRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_Rect moveRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_Rect pickRect = {static_cast<int>(x), static_cast<int>(y), w, h};

    wallManager.insert({id, Wall(id, wallRect, moveRect, pickRect, h, h)});

    if(wallManager.count(id)){
        if (!wallManager.at(id).texture.loadFromFile("assets/texture/wall.png", gRenderer)) {
            logv("Failed to load the wall texture!\n");
            deleteBarricade(id);
            return -1;
        }

        wallManager.at(id).texture.setDimensions(w, h);
        wallManager.at(id).setPosition(x,y);
        return id;
    }
    return -1;
}


void GameManager::setBoundary(SDL_Renderer* gRenderer,
        const float startX, const float startY, const float endX, const float endY) {

    int width = endX - startX + 200;
    int height = 100;

    const float x = startX - 100;
    const float y = startY - 100;

    createWall(gRenderer, x, y, width, height);
    createWall(gRenderer, x, endY, width, height);

    width = 100;
    height = endY - startY + 100;

    createWall(gRenderer, endX, startY, width, height);
    createWall(gRenderer, x, startY, width, height);

    const float sX = (endX + startX) / 2 - BASE_WIDTH - 100;
    const float eX = (endX + startX) / 2 + BASE_WIDTH + 100;
    const float sY = (endY + startY) / 2 - BASE_HEIGHT - 100;
    const float eY = (endY + startY) / 2 + BASE_HEIGHT + 100;

    width = eX - sX;
    height = 100;

    createWall(gRenderer, sX, sY, width / 2, height);
    createWall(gRenderer, sX + (width / 4 * 3), sY, width / 4, height);
    createWall(gRenderer, sX, eY, width / 4, height);
    createWall(gRenderer, sX + width / 2 + 100, eY, width / 2, height);

    width = 100;
    height = eY - sY;

    createWall(gRenderer, sX, sY, width, height / 2);
    createWall(gRenderer, sX, sY + (height / 4 * 3), width, height / 4);
    createWall(gRenderer, eX, sY, width, height / 1.5);
    createWall(gRenderer, eX, sY + (height / 4 * 3), width, height / 4);
}

bool GameManager::createZombieWave(SDL_Renderer* gRenderer, const int n){

    std::vector<Point> spawnPoints;
    spawnPoints.emplace_back(Point(-900, -900));
    spawnPoints.emplace_back(Point(1900, -900));
    spawnPoints.emplace_back(Point(2900, -900));
    spawnPoints.emplace_back(Point(2900, 2900));
    spawnPoints.emplace_back(Point(1900, 2900));
    spawnPoints.emplace_back(Point(-900, 2900));

    if(zombieManager.size() >= spawnPoints.size() * 5) {
        unsigned int count = 0;
        std::vector<int32_t> ids;
        for (const auto& z : zombieManager) {
            if(count >= spawnPoints.size()){
                break;
            }
            ids.push_back(z.first);
            ++count;
        }
        for (const auto& id : ids) {
            deleteZombie(id);
        }
    }

    for (int i = 0; i < n; ++i) {
        for (const auto& p : spawnPoints)
            createZombie(gRenderer, p.first, p.second);
    }

    return true;

}
