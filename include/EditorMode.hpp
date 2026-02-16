#ifndef EDITORMODE_HPP_
#define EDITORMODE_HPP_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


#include "Settings.hpp"
#include "Mode.hpp"
#include "EventManager.hpp"



class EditorMode : public Mode
{
    public:
        EditorMode( AppContext& app_context );

        void run( float dt );

    private:
        void pan_input();
        void update_grid_pos();
        void draw_grid() const;

        void update( float dt );
        void render();

        Vector2D<int> origin_;
        Vector2D<int> mouse_origin_vector_;  
        Vector2D<int> grid_pos_;
        EditorContext editor_context_;
};


#endif