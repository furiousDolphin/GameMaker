
#include "Entity.hpp"


Entity::Entity(Vector2D<double> pos, Animation animation) :
    pos_{pos},
    animation_{std::move(animation)}
{
    // const auto* texture = animation.get_texture();
    // int w = texture->get_width();
    // int h = texture->get_height();

    int w = TILE_SIZE;
    int h = TILE_SIZE;

    int tiles_w = static_cast<int>(std::ceil(static_cast<float>(w) / TILE_SIZE));
    int tiles_h = static_cast<int>(std::ceil(static_cast<float>(h) / TILE_SIZE));
    
    int range_x = (tiles_w / 2) + 1;
    int range_y = (tiles_h / 2) + 1;

    for (int col = -range_x; col <= range_x; col++)
    {
        for (int row = -range_y; row <= range_y; row++)
        { neighbour_offsets_.emplace_back(row, col); }
    }

    neighbour_area_shape_ = std::pair<int, int>((2 * range_y) + 1, (2 * range_x) + 1);
}

Rect Entity::get_rect() const
{
    const auto* texture = animation_.get_texture();
    int w = texture->get_width();
    int h = texture->get_height();

    Vector2D<int> int_pos
    { 
        static_cast<int>(std::round(pos_.x)), 
        static_cast<int>(std::round(pos_.y)) 
    };

    return Rect{int_pos, w, h};
}

void Entity::Collisions::reset()
{
    top = false;
    bot = false;
    right = false;
    left = false;
}

Player::Player(Vector2D<double> pos, const GraphicsManager& graphics_manager) :
    Entity{pos, graphics_manager.copy_animation(GraphicsManager::PLAYER_IDLE_RIGHT)},
    velocity_{0.0, 0.0}
{}

void Player::update(const GraphicsManager& graphics_manager, const EventManager& event_manager, const Land& land, float dt)
{
    animation_.update(dt);

    Vector2D<double> movement_frame{};
    if ( event_manager.key_down(SDL_SCANCODE_W) )
    { velocity_.y = -300.0; }

    if ( event_manager.key_down(SDL_SCANCODE_A) )
    { movement_frame.x -= speed_*dt; }
    if ( event_manager.key_down(SDL_SCANCODE_D) )
    { movement_frame.x += speed_*dt; }

    movement_frame+=velocity_*dt;





    collisions_.reset();
    std::vector<Vector2D<int>> physical_tiles_around;
    Rect rect;

    
    physical_tiles_around = land.get_physical_tiles_around(pos_, neighbour_offsets_);

    pos_.x += movement_frame.x;
    rect = this->get_rect();
    for ( auto neighbour_grid_pos : physical_tiles_around )
    {
        Rect neighbour_rect{neighbour_grid_pos*TILE_SIZE, TILE_SIZE, TILE_SIZE};

        if ( rect.collide_rect(neighbour_rect) )
        {
            if ( movement_frame.x > 0 )
            {
                collisions_.right = true;
                rect.set_right(neighbour_rect.get_left());
            }
            if ( movement_frame.x < 0 )
            {
                collisions_.left = true;
                rect.set_left(neighbour_rect.get_right());
            }
        }
    }
    pos_.x = static_cast<double>(rect.get_left());


    physical_tiles_around = land.get_physical_tiles_around(pos_, neighbour_offsets_);

    pos_.y += movement_frame.y;
    rect = this->get_rect();
    for ( auto neighbour_grid_pos : physical_tiles_around )
    {
        Rect neighbour_rect{neighbour_grid_pos*TILE_SIZE, TILE_SIZE, TILE_SIZE};

        if ( rect.collide_rect(neighbour_rect) )
        {
            if ( movement_frame.y > 0 )
            {
                collisions_.bot = true;
                rect.set_bottom(neighbour_rect.get_top());
            }
            if ( movement_frame.y < 0 )
            {
                collisions_.top = true;
                rect.set_top(neighbour_rect.get_bottom());
            }
        }
    }
    pos_.y = static_cast<double>(rect.get_top());
    if (collisions_.top || collisions_.bot)
    { velocity_.y = 0.0; }

    velocity_.y = std::min(velocity_.y+delta_velocity_y_, max_velocity_y_ ); //./build/GameMakerApp.exe

    if (collisions_.bot || collisions_.top) 
    { velocity_.y = 0; }

}

void Player::render(Vector2D<int> origin) const
{
    const auto* texture = animation_.get_texture();
    texture->render(static_cast<Vector2D<int>>(pos_) - origin);
}

Entities::Entities(
    const GraphicsManager& graphics_manager, 
    const EventManager& event_manager, 
    const Land& land, 
    const float& dt, 
    const Vector2D<int>& origin
) :
    context_{graphics_manager, event_manager, land, dt, origin}
{

}
  
void Entities::load_level(const JsonLevelFormat& json_level_format_data)
{
    const auto& imported_data = json_level_format_data.get_import_data();
    auto it = imported_data.find(JsonLevelFormat::ENTITIES);

    entities_.clear();
    if ( it != imported_data.end() )
    {
        for ( const auto& [vec2_pos, idx] : it->second)
        { 
            if ( auto* idx_ptr = std::get_if<int>(&idx) )
            {
                if ( *idx_ptr == 0 )
                { this->add(std::make_unique<Player>(static_cast<Vector2D<double>>(vec2_pos), context_.graphics_manager)); }
            } 
        }
    }
}

void Entities::add(std::unique_ptr<Entity> entity_ptr)
{ entities_.push_back(std::move(entity_ptr)); }

void Entities::update()
{
    for ( auto& entity : entities_ )
    { entity->update(context_.graphics_manager, context_.event_manager, context_.land, context_.dt); }
}

void Entities::render() const
{
    for ( const auto& entity : entities_ )
    { entity->render(context_.origin); }
}