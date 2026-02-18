


#include "CanvaTile.hpp"


CanvaTile::CanvaTile( Vector2D<int> grid_pos ): 
    grid_pos_  { grid_pos },
    land_index_ {-1}
{}

bool CanvaTile::get_neighbours_flag() const
{ return neighbours_flag_; }

bool CanvaTile::has_land() const 
{ return land_index_ != -1; }

void CanvaTile::add_id( int canva_id, const std::string& style )
{
    neighbours_flag_ = false;

    if( style == "terrain" && !this->has_land() ) 
    { 
        land_index_ = 0;
        neighbours_flag_ = true; 
    }
}

void CanvaTile::remove_id( int canva_id, const std::string& style )
{
    neighbours_flag_ = false;

    if( style == "terrain" && this->has_land() ) 
    { 
        land_index_ = -1; 
        neighbours_flag_ = true; 
    }

}

bool CanvaTile::any_id() const
{ return ( this->has_land() ); }

void CanvaTile::update()
{}

void CanvaTile::render( const Vector2D<int>& origin, const GraphicsManager& graphics_manager ) const
{
    auto render_grid_pos = grid_pos_*TILE_SIZE + origin;

    if( this->has_land() ) 
    {
        const auto& land_textures = graphics_manager.get_vectorized_textures(GraphicsManager::LAND);
        land_textures[land_index_].render( render_grid_pos );
    }
}


CanvaTiles::CanvaTiles( 
    EventManager& event_manager, 
    GraphicsManager& graphics_manager, 
    Vector2D<int>& origin, 
    EditorDataManager& editor_data_manager,
    int& canva_id
) :
    context_{event_manager, graphics_manager, origin, canva_id, editor_data_manager}
{

}

void CanvaTiles::add_id() 
{ 
    Vector2D<int> mouse_pos = context_.event_manager.mouse_pos();
    Vector2D<int> mouse_grid_pos = (mouse_pos - context_.origin).to_grid(TILE_SIZE);

    if( canva_tiles_.find( mouse_grid_pos ) == canva_tiles_.end() )
    { canva_tiles_.insert_or_assign(mouse_grid_pos, CanvaTile(mouse_grid_pos)); } 

    auto& canva_tile = canva_tiles_.at( mouse_grid_pos );
    int id = context_.canva_id;
    const auto& style = context_.editor_data_manager.get_series(id).style;

    canva_tile.add_id( id, style );
            
    if( canva_tile.get_neighbours_flag() ) 
    { this->check_neighbours(); }
}

void CanvaTiles::remove_id() 
{ 
    Vector2D<int> mouse_pos = context_.event_manager.mouse_pos();
    Vector2D<int> mouse_grid_pos = (mouse_pos - context_.origin).to_grid(TILE_SIZE);

    auto it = canva_tiles_.find( mouse_grid_pos );
    if( it != canva_tiles_.end() )
    {
        auto& [grid_pos, canva_tile] = *it;

        int id = context_.canva_id;
        const auto& style = context_.editor_data_manager.get_series(id).style;
        canva_tile.remove_id(id, style);

        if( !canva_tile.any_id() )
        { canva_tiles_.erase( mouse_grid_pos ); }

        if( canva_tile.get_neighbours_flag() ) 
        { this->check_neighbours(); }
    } 
}

void CanvaTiles::check_neighbours()
{
    Vector2D<int> current_pos, neigbour_pos;
    std::string terrain_texture_name;

    Vector2D<int> mouse_pos = context_.event_manager.mouse_pos();
    Vector2D<int> mouse_grid_pos = (mouse_pos - context_.origin).to_grid(TILE_SIZE);

    for( int i = 0; i < 3; i++ )
    {
        for( int j = 0; j < 3; j++ )
        {
            current_pos = { mouse_grid_pos.x + i - 1, mouse_grid_pos.y + j - 1 };

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
                            { terrain_texture_name += neighbour_sign; }
                        }
                    }
                    

                    const auto& graphics_manager = context_.graphics_manager;
                    const auto& land_textures = graphics_manager.get_vectorized_textures(GraphicsManager::LAND);
                    std::optional<int> index = land_textures.get_index_from_name(terrain_texture_name);

                    if( index.has_value() ) 
                    {canva_tile.land_index_ = index.value(); }

                    else
                    { canva_tile.land_index_ = land_textures.get_index_from_name("X").value(); }

                }
            }
        }
    }
}

void CanvaTiles::update()
{
    const auto& event_manager = context_.event_manager;
    if( event_manager.left_got_clicked() ) 
    { this->add_id(); }
    else if( event_manager.right_got_clicked() ) 
    { this->remove_id(); }
}

void CanvaTiles::render() const
{
    for( const auto& [ grid_pos, canva_tile ] : canva_tiles_ )
    { canva_tile.render( context_.origin, context_.graphics_manager ); }    
}