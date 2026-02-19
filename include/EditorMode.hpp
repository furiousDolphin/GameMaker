#ifndef EDITORMODE_HPP_
#define EDITORMODE_HPP_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


#include "Settings.hpp"
#include "Mode.hpp"
#include "EventManager.hpp"
#include "Button.hpp"
#include "CanvaTile.hpp"
#include "CanvaObject.hpp"
#include "EditorMenu.hpp"





class EditorMode : public Mode
{
    public:
        EditorMode( SDL_Renderer* renderer, EventManager& event_manager, GraphicsManager& graphics_manager, ModeType& mode_type );

        void run( float dt ) override;

    private:
        void create_buttons();
        void pan_input();
        void draw_grid() const;

        void update( float dt );
        void render();

        Vector2D<int> origin_;
        Vector2D<int> mouse_origin_vector_;  
        Vector2D<int> grid_pos_;
        int canva_id_;

        struct Context
        {
            SDL_Renderer* renderer;
            EventManager& event_manager;
            GraphicsManager& graphics_manager;
            ModeType& mode_type;
        } context_;

        Buttons buttons_;
        EditorDataManager data_manager_;
        EditorMenu menu_;
        CanvaTiles canva_tiles_;
        CanvaObjects canva_objects_;
};


#endif