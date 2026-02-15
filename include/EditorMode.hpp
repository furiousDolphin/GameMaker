#ifndef EDITORMODE_HPP_
#define EDITORMODE_HPP_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


#include "Settings.hpp"
#include "Mode.hpp"


#include "CanvaTile.hpp"
#include "CanvaObject.hpp"

#include "EditorMenu.hpp"



class EditorMode : public Mode
{
    public:
        EditorMode( AppContext& app_context );


        EditorDataType make_editor_data();


        void pan_input();
        void update_grid_pos();
        void draw_grid() const;
        void create_buttons();

        void update( float dt );
        void render();
        void render_preview();

        void run( float dt );

    private:
      
        Vector2D<int> origin_;
        Vector2D<int> mouse_origin_vector_;  
        Vector2D<int> grid_pos_;

        EditorDataType editor_data_;
        int            canva_id_;

        EditorGraphics editor_graphics_;
        EditorContext  editor_context_;


        struct Canvas
        {
            CanvaTiles   tiles;
            CanvaObjects background_objects;
            CanvaObjects entity_objects;
            CanvaObjects foreground_objects;

        } editor_canvas_;


        EditorMenu editor_menu_;  

        Buttons buttons_;
};


#endif