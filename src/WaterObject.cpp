

#include "WaterObject.hpp"


WaterObject::WaterObject(Vector2D<int> pos, Vector2D<int> shape) :
    rect_{pos, shape.x, shape.y}
{

}

void WaterObject::update()
{

}

void WaterObject::render(const SDL_Renderer* renderer, const Vector2D<int>& origin) const
{
    std::vector<SDL_Vertex> vertices;
    
    int left = rect_.get_left();
    int right = rect_.get_right();
    int bottom = rect_.get_bottom();
    int surface_y_base = rect_.get_y() + 20;

    int step = 4;
    
    for (int x = left; x < right; x += step) 
    {
        int next_x = std::min(x + step, right);

        float y_left = 48.0;
        float y_right = 48.0;

        SDL_Color top_color = { 0, 150, 255, 180 }; 
        SDL_Color bot_color = { 0, 50, 150, 220 };  

        SDL_Vertex tl = { {(float)x - origin.x, (float)bottom - y_left - origin.y}, top_color, {0, 0} };
        SDL_Vertex tr = { {(float)next_x - origin.x, (float)bottom - y_right - origin.y}, top_color, {0, 0} };
        SDL_Vertex bl = { {(float)x - origin.x, (float)bottom - origin.y}, bot_color, {0, 0} };
        SDL_Vertex br = { {(float)next_x - origin.x, (float)bottom - origin.y}, bot_color, {0, 0} };

        vertices.push_back(tl); vertices.push_back(tr); vertices.push_back(bl);
        vertices.push_back(bl); vertices.push_back(tr); vertices.push_back(br);
    }

    if (!vertices.empty()) 
    { SDL_RenderGeometry(const_cast<SDL_Renderer*>(renderer), nullptr, vertices.data(), vertices.size(), nullptr, 0); }
}

WaterObjects::WaterObjects( const SDL_Renderer* renderer, const Vector2D<int>& origin) :
    context_{renderer, origin}
{

}
void WaterObjects::load_level(const JsonLevelFormat& json_level_format_data)
{
    const auto& data = json_level_format_data.get_import_data();
    auto it = data.find(JsonLevelFormat::WATER);
    if ( it != data.end() )
    {
        for ( const auto& [vec2_pos, grid_shape] : it->second )
        {
            if ( const auto* grid_shape_ptr = std::get_if<Vector2D<int>>(&grid_shape) )
            { water_objects_.emplace_back(vec2_pos, (*grid_shape_ptr) * TILE_SIZE); }  
        }
    }
}
void WaterObjects::update()
{}

void WaterObjects::render() const
{
    for ( const auto& water_object : water_objects_ )
    { water_object.render(context_.renderer, context_.origin); }
}