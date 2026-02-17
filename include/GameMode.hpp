#ifndef GAMEMODE_HPP_
#define GAMEMODE_HPP_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


#include "Mode.hpp"
#include "Settings.hpp"
#include "Button.hpp"
#include "EventManager.hpp"
#include "GraphicsManager.hpp"

class GameMode : public Mode
{
    public:
        GameMode( AppContext& app_context );

        void run( float dt );

    private:
        void update();
        void render();
        void create_buttons();

        Buttons buttons_;
};

#endif