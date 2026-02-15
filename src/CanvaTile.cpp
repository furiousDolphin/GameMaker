

#include "Settings.hpp"
#include "EventManager.hpp"
#include "GraphicsManager.hpp"
#include "EditorGraphics.hpp"
#include "CanvaTile.hpp"


CanvaTile::CanvaTile( Vector2D<int> grid_pos ): 
    grid_pos_  { grid_pos },
    has_land_  { false    },
    has_water_ { false    },
    has_coin_  { false    }, 
    has_enemy_ { false    }
{}

void CanvaTile::add_id( const EditorContext& editor_context )
{
    check_neighbours_flag_ = false;

    const auto id     = editor_context.canva_id;
    const auto& style = editor_context.editor_data.at( id ).style;

         if( style == "terrain" && !has_land_    ) { has_land_  = true; check_neighbours_flag_ = true; }
    else if( style == "water"   && !has_water_   ) { has_water_ = true; check_neighbours_flag_ = true; }
    else if( style == "coin"    && !has_coin_    ) { has_coin_  = true; coin_id_               = id;   }
    else if( style == "enemy"   && !has_enemy_   ) { has_enemy_ = true; enemy_id_              = id;   }
}

void CanvaTile::remove_id( const EditorContext& editor_context )
{
    check_neighbours_flag_ = false;

    const auto  id    = editor_context.canva_id;
    const auto& style = editor_context.editor_data.at( id ).style;

         if( style == "terrain" && has_land_    ) { has_land_  = false; check_neighbours_flag_ = true; }
    else if( style == "water"   && has_water_   ) { has_water_ = false; check_neighbours_flag_ = true; }
    else if( style == "coin"    && has_coin_    ) { has_coin_  = false;                                }
    else if( style == "enemy"   && has_enemy_   ) { has_enemy_ = false;                                }
}

bool CanvaTile::any_id()
{
    return ( has_land_ || has_water_ || has_coin_ || has_enemy_ );
}

void CanvaTile::render( const EditorContext& editor_context )
{
    auto render_grid_pos = grid_pos_*TILE_SIZE + editor_context.origin;

    if( has_land_ ) 
    {
        const auto& texture = editor_context.editor_graphics.get_texture( GraphicsManager::LAND, land_index_ );

        texture.render( render_grid_pos );
    }
    if( has_enemy_ )
    {
        const auto& animation = editor_context.editor_graphics.get_animation( enemy_id_ );
        const auto& texture   = animation.get_texture();

        auto w = texture->get_width();
        auto h = texture->get_height();

        texture->render( render_grid_pos + Vector2D<int>( (TILE_SIZE-w)/2,
                                                           TILE_SIZE-h ) );
    }

    if( has_coin_ )
    {
        const auto& animation = editor_context.editor_graphics.get_animation( coin_id_ );
        const auto& texture   = animation.get_texture();

        auto w = texture->get_width();
        auto h = texture->get_height();

        texture->render( render_grid_pos + Vector2D<int>( (TILE_SIZE-w)/2,
                                                          (TILE_SIZE-h)/2  ) );
    }
    if( has_water_ )
        if( water_type_ == WaterType::TOP ) 
        {
            const auto& animation = editor_context.editor_graphics.get_animation( 3 );
            const auto& texture   = animation.get_texture();

            texture->render( render_grid_pos );
        }
        else
        {
            const auto& texture = editor_context.editor_graphics.get_texture( GraphicsManager::WATER_BOTTOM );

            texture.render( render_grid_pos );
        }
}

CanvaTiles::CanvaTiles( const EditorContext& editor_context ): 
    editor_context_{ editor_context         },
    canva_tiles_   {                        }, 
    neighbours_    { { { { 0, -1 }, "A" },
                       { { 1, -1 }, "B" },
                       { { 1,  0 }, "C" },
                       { { 1,  1 }, "D" },
                       { { 0,  1 }, "E" },
                       { {-1,  1 }, "F" },
                       { {-1,  0 }, "G" },
                       { {-1, -1 }, "H" } } } 
{}

void CanvaTiles::add_id() 
{ 
    if( canva_tiles_.find( mouse_grid_pos_ ) == canva_tiles_.end() )
    {
        canva_tiles_.insert_or_assign(mouse_grid_pos_, CanvaTile(mouse_grid_pos_));
    } 

    auto& canva_tile = canva_tiles_.at( mouse_grid_pos_ );
    canva_tile.add_id( editor_context_ );
            
    if( canva_tile.get_check_neighbours_flag() ) this->check_neighbours();
}

void CanvaTiles::remove_id() 
{ 
    if( canva_tiles_.find( mouse_grid_pos_ ) != canva_tiles_.end() )
    {
        auto& canva_tile = canva_tiles_.at( mouse_grid_pos_ );
        canva_tile.remove_id( editor_context_ );

        if( !canva_tile.any_id() )
        {                   
            if( canva_tile.get_check_neighbours_flag() ) this->check_neighbours();

            canva_tiles_.erase( mouse_grid_pos_ ); 
        }
        else
            if( canva_tile.get_check_neighbours_flag() ) this->check_neighbours();
    } 
}

void CanvaTiles::check_neighbours()
{
    Vector2D<int> current_pos, neigbour_pos;
    std::string terrain_texture_name;

    for( int i = 0; i < 3; i++ )
    {
        for( int j = 0; j < 3; j++ )
        {
            current_pos = { mouse_grid_pos_.x + i - 1,
                            mouse_grid_pos_.y + j - 1 };

            if( canva_tiles_.find( current_pos ) != canva_tiles_.end() )
            {
                auto& canva_tile = canva_tiles_.at( current_pos );


                if( canva_tile.has_land() )
                {
                    terrain_texture_name.clear();

                    for( const auto& [ neighbour_offset, neighbour_sign ] : neighbours_ )
                    {
                        neigbour_pos = current_pos + neighbour_offset;

                        if( canva_tiles_.find( neigbour_pos ) != canva_tiles_.end() )
                        {
                            auto& neigbour_tile = canva_tiles_.at( neigbour_pos );

                            if( neigbour_tile.has_land() )
                            {
                                terrain_texture_name += neighbour_sign;
                            }
                        }
                    }
                    

                    const auto& graphics = editor_context_.editor_graphics;

                         if( auto index = graphics.get_texture_index( GraphicsManager::LAND, terrain_texture_name ) ) canva_tile.set_land_index( *index );
                    else if( auto index = graphics.get_texture_index( GraphicsManager::LAND, "X"                  ) ) canva_tile.set_land_index( *index );

                }


                if( canva_tile.has_water() )
                {
                    const auto& [neighbour_offset, neighbour_sign] = *( neighbours_.begin() ); 
                    auto         neighbour_pos                     = current_pos + neighbour_offset;
                    auto         it                                = canva_tiles_.find( neighbour_pos );

                    if( it != canva_tiles_.end() )
                    {
                        auto& neighbour_tile = it->second;

                        if( neighbour_tile.has_water() ) canva_tile.set_water_type( WaterType::BOTTOM );
                        else                             canva_tile.set_water_type( WaterType::TOP    );                       
                    }
                    else canva_tile.set_water_type( WaterType::TOP );   
                }
            }
        }
    }
}

void CanvaTiles::update_mouse_grid_pos()
{
    auto [ x, y ] = editor_context_.event_manager.mouse_pos() - editor_context_.origin;
    
    mouse_grid_pos_ = {static_cast<int>( x/TILE_SIZE ) - ( ( x < 0 ) ? 1 : 0),
                       static_cast<int>( y/TILE_SIZE ) - ( ( y < 0 ) ? 1 : 0) };
}

void CanvaTiles::update()
{
         if( editor_context_.event_manager.left_got_clicked()  ) { this->update_mouse_grid_pos(); this->add_id();    }
    else if( editor_context_.event_manager.right_got_clicked() ) { this->update_mouse_grid_pos(); this->remove_id(); }
}

void CanvaTiles::render()
{
    for( auto& [ grid_pos, canva_tile ] : canva_tiles_ )
    {
        canva_tile.render( editor_context_ );          
    }    
}