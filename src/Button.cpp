#include <cmath>


#include "Button.hpp"

Button::Button( Vector2D<int> pos ): 
    rect_{ pos } 
{}


TextButton::TextButton(       Vector2D<int>               pos,
                              SDL_Renderer*               renderer,
                        const std::string&                text,
                              SDL_Color                   marked_color,
                              SDL_Color                   unmarked_color,
                              TTF_Font*                   font, 
                              std::function< void(void) > fun ):

    Button   { pos                                      },
    pos_     { pos                                      },
    textures_{ { renderer, text, marked_color,   font },
               { renderer, text, unmarked_color, font },
               { nullptr                              } },
    fun_     { fun                                      }
{
    auto w = textures_.marked.get_width();
    auto h = textures_.marked.get_height();

    rect_.set_w( w );
    rect_.set_h( h );
}

bool TextButton::update( const EventManager& event_manager )
{
    bool collision = false;

      if( rect_.collide_point( event_manager.mouse_pos() ) ) 
      { 
        if( event_manager.left_got_clicked() )
        { 
            this->fun_(); 
            collision = true;
            textures_.current = &textures_.marked;   
        }
    }
                                                               
    else
    { 
        textures_.current = &textures_.unmarked; 
    }   

    return collision;
}


void TextButton::render( SDL_Renderer* renderer )
{
    textures_.current->render( pos_ );
}


EditorMenuButton::EditorMenuButton( Vector2D<int> pos,
                                    const std::vector< std::pair< int, Texture > >* main_textures,
                                    const std::vector< std::pair< int, Texture > >* alt_textures,
                                    std::function< void(int) > fun ):
    Button         { pos           },
    pos_           { pos           },
    main_textures_ { main_textures },
    alt_textures_  { alt_textures  },
    fun_           { fun           },
    texture_index_ { 0             },
    main_active_   { true          }
{
    this->update_rect();
}

int EditorMenuButton::get_id()
{
    auto& tex = (main_active_ || alt_textures_ == nullptr) ? main_textures_ 
                                                           : alt_textures_;
    return tex->at(texture_index_).first;
}

void EditorMenuButton::update_rect()
{
    auto& tex = (main_active_ || alt_textures_ == nullptr) ? main_textures_ 
                                                           : alt_textures_;
    auto w = tex->at(texture_index_).second.get_width();
    auto h = tex->at(texture_index_).second.get_height();

    rect_ = { pos_ + Vector2D<int>( -w/2 + TILE_SIZE/2,
                                    -h/2 + TILE_SIZE/2 ), w, h };    
}

bool EditorMenuButton::update( const EventManager& event_manager )
{
    bool collision = false;


    if( rect_.collide_point( event_manager.mouse_pos() ) && event_manager.left_got_clicked() )
    {
        this->fun_( this->get_id() );
    }

    main_active_ = true;

    if( rect_.collide_point( event_manager.mouse_pos() ) )
    {
        collision = true;

        if( event_manager.key_down( SDL_SCANCODE_LALT ) )
        {
            main_active_ = false;
            this->update_rect();
        }
        else
        {
            main_active_ = true;
            this->update_rect();
        }
        
        if( event_manager.get_scroll_state() != 0 )
        {
            texture_index_ += event_manager.get_scroll_state();
            texture_index_ = std::max( 0, std::min( texture_index_, static_cast<int>(main_textures_->size()) - 1 ) );

            this->update_rect();
            this->fun_( this->get_id() );
        }
    }

    return collision;
}

void EditorMenuButton::render( SDL_Renderer* renderer )
{
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
    SDL_RenderFillRect( renderer, rect_.sdl_rect_ptr() );

    auto& tex = (main_active_ || alt_textures_ == nullptr) ? main_textures_ 
                                                           : alt_textures_;

    tex->at(texture_index_).second.render( rect_.get_pos() );   
}





void Buttons::add( std::unique_ptr<Button> button )
{
    buttons_.push_back( std::move(button) );
}

bool Buttons::update( const EventManager& event_manager )
{
    bool any = false;

    for( auto& button : buttons_ )
    {
        any = button->update( event_manager);
    }

    return any;
}

void Buttons::render( SDL_Renderer* renderer )
{
    for( auto& button : buttons_ )
    {
        button->render( renderer);
    }
}
