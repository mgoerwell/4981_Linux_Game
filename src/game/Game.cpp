#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "Game.h"
#include "GameStateMatch.h"
#include "GameStateMenu.h"
#include "../view/Window.h"
#include "../player/Player.h"
#include "../buildings/Base.h"
#include "Level.h"
#include "../view/Camera.h"
#include "../log/log.h"


void Game::run() {
    // End program if stateID is 0 after a end of a loop
    while (stateID > 0) {
        loadState();
        if (state->load()) {
            state->loop();
        }
    }
}

void Game::loadState() {
    logv("Starting ");
    if (state != NULL) {
        delete state;
    }
    // Sets the state by the state ID
    switch(stateID) {
        case 1:
            logv("Menu State\n");
            state = new GameStateMenu(*this, window.getWidth(), window.getHeight());
            break;
        case 2:
            logv("Match State\n");

            state = new GameStateMatch(*this, window.getWidth(), window.getHeight());

            break;
        default:
            break;
    }
     // Reset stateID back to zero to allow states to end program or incase of load failure
    stateID = 0;
}

bool Game::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        logv( "SDL could not initialize! %s\n", SDL_GetError() );
        success = false;
    } else {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
            logv( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        window = Window();
        if ( !window.init() ) {
            logv( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        } else {
            //Create renderer for window
            renderer = window.createRenderer();
            if ( renderer  == NULL ) {
                logv( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            } else {

                //Initialize renderer color
                SDL_SetRenderDrawColor( renderer , 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if ( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                    logv( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                } else {
                    screenSurface = window.getScreenSurface();
                }

                 //Initialize SDL_ttf
                if ( TTF_Init() == -1 ) {
                    logv( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }

                //Initialize SDL_mixer
                if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
                    logv( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }

                 screenSurface = window.getScreenSurface();

            }
        }
    }

    return success;
}

bool Game::loadMedia() {
    //Loading success flag
    bool success = true;


    // Load graphics, audio, and fonts here

    return success;
}

SDL_Surface* Game::loadSurface( std::string path ) {
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL ) {
        logv( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    } else if (screenSurface == NULL) {
        logv( "Unable to load image %s!\n  Window surface is NULL\n", path.c_str());
    } else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, 0 );
        if( optimizedSurface == NULL ) {
            logv( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}


// Sets texture
SDL_Texture* Game::loadTexture( std::string path ) {
    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL ) {
        logv( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL ) {
            logv( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

void Game::close() {

    if (state != NULL) {
        delete state;
    }

    //Destroy window
    SDL_DestroyRenderer( renderer );
    window.free();
    renderer = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}
