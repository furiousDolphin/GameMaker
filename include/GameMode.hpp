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
#include "PersistentState.hpp"
#include "Land.hpp"




class GameMode : public Mode
{
    public:
        GameMode( SDL_Renderer* renderer, EventManager& event_manager, GraphicsManager& graphics_manager, PersistentState& persistent_state );

        void run( float dt ) override;

    private:
        void update();
        void render();
        void create_buttons();
        void import_data();

        Vector2D<int> origin_;

        struct Context
        {
            SDL_Renderer* renderer;
            EventManager& event_manager;
            GraphicsManager& graphics_manager;
            PersistentState& persistent_state;
        } context_;

        Buttons buttons_;
        Land land_;
};

#endif