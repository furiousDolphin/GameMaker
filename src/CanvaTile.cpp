

#include "CanvaObject.hpp"
#include "CanvaTile.hpp"


CanvaTile::CanvaTile( int id, const EditorDataManager& editor_data_manager, Vector2D<int> object_offset ): 
    land_index_ {-1}
{
    this->add_id(id, editor_data_manager, object_offset );
}

bool CanvaTile::get_neighbours_flag() const
{ return neighbours_flag_; }

bool CanvaTile::has_land() const 
{ return land_index_ != -1; }

void CanvaTile::add_id( int canva_id, const EditorDataManager& editor_data_manager, Vector2D<int> object_offset )
{
    const auto& series = editor_data_manager.get_series(canva_id);
    const auto& style = series.style;

    neighbours_flag_ = false;

    if( style == "terrain" && !this->has_land() ) 
    { 
        land_index_ = 0;
        neighbours_flag_ = true; 
    }
    else
    { objects_.emplace_back(object_offset, canva_id ); }   
}

void CanvaTile::remove_id( int canva_id, const EditorDataManager& editor_data_manager )
{
    const auto& series = editor_data_manager.get_series(canva_id);
    const auto& style = series.style;

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

void CanvaTile::render( Vector2D<int> pos, const GraphicsManager& graphics_manager ) const
{
    if( this->has_land() ) 
    {
        const auto& land_textures = graphics_manager.get_vectorized_textures(GraphicsManager::LAND);
        land_textures[land_index_].render( pos );
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
    const auto& editor_data_manager = context_.editor_data_manager;
    Vector2D<int> mouse_pos = context_.event_manager.mouse_pos();
    Vector2D<int> mouse_grid_pos = (mouse_pos - context_.origin).to_grid(TILE_SIZE);
    int id = context_.canva_id;

    auto pomocnicza = [&]()
    {
        auto it = canva_tiles_.find( mouse_grid_pos );
        if ( it == canva_tiles_.end() )
        { return canva_tiles_.emplace(mouse_grid_pos, CanvaTile(id, editor_data_manager)).first; }
        else
        { 
            it->second.add_id(id, editor_data_manager);
            return it;
        }
    };

    auto it = pomocnicza();
    const auto& [grid_pos, canva_tile] = *it;

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

        canva_tile.remove_id(context_.canva_id, context_.editor_data_manager);

        if( canva_tile.get_neighbours_flag() ) 
        { this->check_neighbours(); }

        if( !canva_tile.any_id() )
        { canva_tiles_.erase(it); }
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
    { 
        auto pos = grid_pos*TILE_SIZE + context_.origin;
        canva_tile.render( pos, context_.graphics_manager ); 
    }    
}