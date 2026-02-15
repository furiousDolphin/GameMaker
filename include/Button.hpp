
#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "Vector2D.hpp"
#include "Rect.hpp"

#include "Settings.hpp"
#include "EventManager.hpp"
#include "SDL_Management.hpp"

class Buttons;

class Button
{
    public:
        Button( Vector2D<int> pos );
        virtual ~Button() = default;


        virtual bool update( const EventManager& event_manager ) = 0;
        virtual void render( SDL_Renderer* renderer ) = 0;

    protected:
        Rect rect_;
};

class Buttons
{
    public:
        Buttons() = default;
        bool update( const EventManager& event_manager );
        void render( SDL_Renderer* renderer );
        void add( std::unique_ptr<Button> button );


    private:
        std::vector< std::unique_ptr<Button> > buttons_;
        
};




class TextButton : public Button
{
    public:
        TextButton(       Vector2D<int>               pos,
                          SDL_Renderer*               renderer,
                    const std::string&                text,
                          SDL_Color                   marked_color,
                          SDL_Color                   unmarked_color,
                          TTF_Font*                   font, 
                          std::function< void(void) > fun  = nullptr );

        
        bool update( const EventManager& event_manager ) override;
        void render(       SDL_Renderer* renderer      ) override;

    private:
        std::function< void(void) > fun_;
        Vector2D<int>               pos_;
        
        struct Textures
        {
            Texture  marked;
            Texture  unmarked; 
            Texture* current;

        } textures_;

};


class EditorMenuButton : public Button
{
    public:
        EditorMenuButton( Vector2D<int> pos,
                          const std::vector< std::pair< int, Texture > >*  main_textures,
                          const std::vector< std::pair< int, Texture > >*  alt_textures = nullptr,
                          std::function< void(int) > fun = nullptr );

        int  get_id();                         
        void update_rect();      


        bool update( const EventManager& event_manager ) override;
        void render( SDL_Renderer* renderer )            override;

    private:
        const std::vector< std::pair< int, Texture > >* main_textures_;
        const std::vector< std::pair< int, Texture > >* alt_textures_;

        MenuTexturesType textures_;
        std::function< void(int) > fun_;

        int texture_index_;
        bool main_active_;

        Vector2D<int> pos_;
};



#endif