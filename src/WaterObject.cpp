

#include "WaterObject.hpp"


WaterObject::WaterObject(Vector2D<int> pos, Vector2D<int> shape) :
    rect_{pos, shape.x, shape.y}
{
    double L = static_cast<double>(shape.x)/TILE_SIZE;
    std::string L_str = std::to_string(L);
    wave_eq_ptr_ = std::make_unique<WaveEquation>("u(0)=0", "u("+L_str+")=0", 0.2, 100.0, 8);

    int N = (shape.x/4 + 1);
    int step = shape.x/N;

    x_dense_ = Eigen::VectorXd::LinSpaced(N, pos.x, pos.x+shape.x);
    wave_sim_ptr_ = std::make_unique<WaveSimulation>(*wave_eq_ptr_, x_dense_);

    Eigen::VectorXd x_in_tiles = (x_dense_.array() - pos.x)/TILE_SIZE;
    Eigen::VectorXd init_u = 0.5*(-(x_in_tiles.array() - L/2.0).square()/0.4).exp();
    Eigen::VectorXd init_v = Eigen::VectorXd::Zero(N);
    wave_sim_ptr_->set_u_v(init_u, init_v);
}

void WaterObject::update(float dt)
{ wave_sim_ptr_->update(dt); }

void WaterObject::render(const SDL_Renderer* renderer, const Vector2D<int>& origin) const
{
    std::vector<SDL_Vertex> vertices;
    
    int left = rect_.get_left();
    int right = rect_.get_right();
    int bottom = rect_.get_bottom();
    float top_level = rect_.get_h() - 0.25*TILE_SIZE;

    auto [u, _] = wave_sim_ptr_->get_u_v();
    float step_x = (right - left) / (float)(u.size() - 1);

    for (int i = 0; i < (int)u.size() - 1; ++i) 
    {
        float x1 = left + i * step_x;
        float x2 = left + (i + 1) * step_x;

        float y1 = bottom - top_level;
        float y2 = bottom - top_level; 

        // float y1 = bottom - (top_level + (float)u[i]*TILE_SIZE);
        // float y2 = bottom - (top_level + (float)u[i+1]*TILE_SIZE);    

        SDL_Color color_top = { 0, 120, 255, 100 }; 
        SDL_Color color_bot = { 0, 30, 100, 130 };  

        SDL_Vertex tl = { {x1 - origin.x, y1 - origin.y}, color_top, {0, 0} };
        SDL_Vertex tr = { {x2 - origin.x, y2 - origin.y}, color_top, {0, 0} };
        SDL_Vertex bl = { {x1 - origin.x, bottom - origin.y}, color_bot, {0, 0} };
        SDL_Vertex br = { {x2 - origin.x, bottom - origin.y}, color_bot, {0, 0} };

        vertices.push_back(tl); vertices.push_back(tr); vertices.push_back(bl);
        vertices.push_back(bl); vertices.push_back(tr); vertices.push_back(br);
    }

    if (!vertices.empty()) 
    { SDL_RenderGeometry(const_cast<SDL_Renderer*>(renderer), nullptr, vertices.data(), vertices.size(), nullptr, 0); }
}

WaterObjects::WaterObjects( const SDL_Renderer* renderer, const Vector2D<int>& origin, const float& dt) :
    context_{renderer, origin, dt}
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
{
    for ( auto& water_object : water_objects_ )
    { water_object.update(context_.dt); }
}

void WaterObjects::render() const
{
    for ( const auto& water_object : water_objects_ )
    { water_object.render(context_.renderer, context_.origin); }
}