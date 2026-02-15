


#include "EditorGraphics.hpp"

EditorGraphics::EditorGraphics( const AppContext& app_context, const EditorDataType& editor_data ):
    graphics_manager_{ app_context.graphics_manager }
{
    for( const auto& [ id, val ] : editor_data )
    {
        if( val.graphics != "" )
        {
            Animation animation = graphics_manager_.copy_animation( val.graphics );

            animations_.emplace( id, std::move( animation ) );
        }
    }

    for( const auto& [ id, val ] : editor_data )
    {
        if( val.preview != "" )
        {
            Texture   texture   { app_context.renderer, val.preview };
            CanvaType canva_type{ ( val.menu == "palm fg" || val.menu == "palm bg" ) ? CanvaType::OBJECT 
                                                                                     : CanvaType::TILE   };

            previews_.emplace( id, std::make_pair<CanvaType, Texture>( std::move( canva_type ), std::move( texture ) ) );
        }
    }    

    for( const auto& [ id, val ] : editor_data )
    {
        if( val.menu_surf != "" )
        {
            Texture texture{ app_context.renderer, val.menu_surf };
            
            auto it = menu_textures_.find( val.menu );

            if( it == menu_textures_.end() )
            {
                std::vector<std::pair<int, Texture>> v;
                v.emplace_back( id, std::move( texture ) );
                menu_textures_.emplace( val.menu, std::move(v) );
            }
            else
            {
                menu_textures_[ val.menu ].emplace_back( id, std::move( texture ) );
            }
        }
    }
}


const Animation& EditorGraphics::get_animation( int id ) const
{
    auto it = animations_.find( id );
    if( it != animations_.end() ) { return it->second;                                                  }
    else                          { throw  std::out_of_range( "nie znaleziono animacji dla id=" + id ); }
}

const std::pair< CanvaType, Texture >& EditorGraphics::get_preview( int id ) const
{
    auto it = previews_.find( id );
    if( it != previews_.end() ) { return it->second;                                                 }
    else                        { throw  std::out_of_range( "nie znaleziono preview dla id=" + id ); }    
}

void EditorGraphics::update_animations( float dt )
{
    for( auto& [ id, animation ] : animations_ )
    {
        animation.update( dt );
    }
}