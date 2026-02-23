

#include "Land.hpp"

Land::Land(const GraphicsManager& graphics_manager, const Vector2D<int>& origin) :
    context_{graphics_manager, origin}
{

}

std::vector<Vector2D<int>> Land::get_physical_tiles_around(
    Vector2D<int> entity_pos,
    const std::vector<Vector2D<int>>& entity_neighbour_offsets ) const
{
    std::vector<Vector2D<int>> res;
    res.reserve(entity_neighbour_offsets.size());

    Vector2D<int> entity_grid_pos = entity_pos.to_grid(TILE_SIZE);
    for ( auto offset : entity_neighbour_offsets )
    { 
        Vector2D<int> neighbour_grid_pos = entity_grid_pos + offset;
        if ( land_tiles_.contains(neighbour_grid_pos) )
        { res.push_back(neighbour_grid_pos); }
    }
    return res;
}

void Land::load_level(const JsonLevelFormat& json_level_format_data)
{
    const auto& imported_data = json_level_format_data.get_import_data();
    auto it = imported_data.find(JsonLevelFormat::TERRAIN);

    land_tiles_.clear();
    if ( it != imported_data.end() )
    {
        for ( const auto& [vec2_pos, idx] : it->second)
        { 
            if ( auto* idx_ptr = std::get_if<int>(&idx) )
            {land_tiles_[vec2_pos] = *idx_ptr;} 
        }
    }

    std::cout << "land_tiles_.size() = " << land_tiles_.size() << "\n";
}

void Land::update()
{
    
}

void Land::render() const
{
    const auto& land_textures = context_.graphics_manager.get_vectorized_textures(GraphicsManager::LAND);
    Vector2D<int> origin = context_.origin;

    int start_x = origin.x / TILE_SIZE;
    int start_y = origin.y / TILE_SIZE;
    int end_x = (origin.x + WIDTH) / TILE_SIZE;
    int end_y = (origin.y + HEIGHT) / TILE_SIZE;

    for ( int x = start_x; x <= end_x; x++ )
    {
        for ( int y = start_y; y <= end_y; y++ )
        {
            auto it = land_tiles_.find({x, y});
            if ( it != land_tiles_.end() )
            { 
                auto& [pos, idx] = *it;
                land_textures[idx].render(pos*TILE_SIZE-origin); 
            }
        } 
    }
}