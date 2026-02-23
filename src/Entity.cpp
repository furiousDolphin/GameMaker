
#include "Entity.hpp"


Entity::Entity(Vector2D<int> pos, Animation animation) :
    pos_{pos},
    animation_{std::move(animation)}
{
    int w = 23;
    int h = 31;

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

    return Rect{pos_, w, h};
}

void Entity::Collisions::reset()
{
    top = false;
    bot = false;
    right = false;
    left = false;
}

Player::Player(Vector2D<int> pos, const GraphicsManager& graphics_manager) :
    Entity{pos, graphics_manager.copy_animation(GraphicsManager::PLAYER_IDLE_RIGHT)},
    velocity_{0.0, 0.0}
{}

void Player::update(const GraphicsManager& graphics_manager, const EventManager& event_manager, const Land& land, float dt)
{
    animation_.update(dt);

    Vector2D<double> movement_frame{};
    if ( event_manager.key_down(SDL_SCANCODE_W) )
    { velocity_.y = -5.0; }

    if ( event_manager.key_down(SDL_SCANCODE_A) )
    { movement_frame.x -= speed_; }
    if ( event_manager.key_down(SDL_SCANCODE_D) )
    { movement_frame.x += speed_; }

    movement_frame += velocity_;
    velocity_.y = std::min(velocity_.y+delta_velocity_y_, max_velocity_y_ );

    if (collisions_.bot || collisions_.top) 
    { velocity_.y = 0; }

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
    pos_.x = rect.get_left();


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
    pos_.y = rect.get_top();
}

void Player::render(Vector2D<int> origin) const
{
    const auto* texture = animation_.get_texture();
    texture->render(pos_ - origin);
}