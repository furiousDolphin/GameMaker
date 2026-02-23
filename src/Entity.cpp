
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
        { neighbour_offsets_.insert({row, col}); }
    }
}

void Entity::Collisions::reset()
{
    top = false;
    bot = false;
    right = false;
    left = false;
}

Player::Player(Vector2D<int> pos, const GraphicsManager& graphics_manager) :
    Entity{pos, graphics_manager.copy_animation(GraphicsManager::PLAYER_IDLE_RIGHT)}
{}

void Player::update(const GraphicsManager& graphics_manager, const EventManager& event_manager, float dt)
{

}

void Player::render()
{

}