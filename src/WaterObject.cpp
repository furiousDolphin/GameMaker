

#include "WaterObject.hpp"


WaterObject::WaterObject(Vector2D<int> pos, Vector2D<int> shape) :
    rect_{pos, shape.x, shape.y}
{
    std::string L = std::to_string(static_cast<double>(shape.x)/TILE_SIZE);
    wave_eq_ptr_ = std::make_unique<WaveEquation>("u(0)=0", "u("+L+")=0", 3000.0, 20.0, 16);
    int N = (shape.x/4 + 1);
    int step = shape.x/N;
    x_dense_ = Eigen::VectorXd::LinSpaced(N, pos.x, pos.x+shape.x);
    std::cout << pos.x << " : " << pos.x+shape.x << "\n";
    std::cout << x_dense_ << "\n";
    wave_sim_ptr_ = std::make_unique<WaveSimulation>(*wave_eq_ptr_, x_dense_);

    Eigen::VectorXd init_u = Eigen::VectorXd::Zero(N);
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
    int surface_y_base = rect_.get_y() + 20;

    auto [u, _] = wave_sim_ptr_->get_u_v();
    // std::cout << u.size() << "\n";
    // int step = (right - left)/u.size();
    // int x = left;

    // for (int i = 1; i < u.size(); i++) 
    // {
    //     int next_x = std::min(x + step, right);

    //     // float y_left = u[i-1];
    //     // float y_right = u[i];
    //     float y_left = 48.0;
    //     float y_right = 48.0;        

    //     SDL_Color top_color = { 0, 150, 255, 180 }; 
    //     SDL_Color bot_color = { 0, 50, 150, 220 };  

    //     SDL_Vertex tl = { {(float)x - origin.x, (float)bottom - y_left - origin.y}, top_color, {0, 0} };
    //     SDL_Vertex tr = { {(float)next_x - origin.x, (float)bottom - y_right - origin.y}, top_color, {0, 0} };
    //     SDL_Vertex bl = { {(float)x - origin.x, (float)bottom - origin.y}, bot_color, {0, 0} };
    //     SDL_Vertex br = { {(float)next_x - origin.x, (float)bottom - origin.y}, bot_color, {0, 0} };

    //     vertices.push_back(tl); vertices.push_back(tr); vertices.push_back(bl);
    //     vertices.push_back(bl); vertices.push_back(tr); vertices.push_back(br);
    // }

    // if (!vertices.empty()) 
    // { SDL_RenderGeometry(const_cast<SDL_Renderer*>(renderer), nullptr, vertices.data(), vertices.size(), nullptr, 0); }
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