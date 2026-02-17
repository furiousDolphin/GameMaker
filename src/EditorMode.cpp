#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include "EditorMode.hpp"

EditorMode::EditorMode( SDL_Renderer* renderer, EventManager& event_manager, GraphicsManager& graphics_manager, ModeType& mode_type ):
    context_{renderer, event_manager, graphics_manager, mode_type},
    buttons_{event_manager, graphics_manager}
{
    this->create_buttons();
}

void EditorMode::create_buttons()
{
    const auto& textures = context_.graphics_manager.get_texture("GAME", GraphicsManager::MINECRAFT_24);
    buttons_.add(std::make_unique<TextButton>(Vector2D<int>(0, 0), [this](){context_.mode_type = ModeType::GAME;}, textures));
}

void EditorMode::draw_grid() const
{

    SDL_Renderer* renderer = context_.renderer;

    int x_offset = origin_.x % TILE_SIZE;
    int y_offset = origin_.y % TILE_SIZE;

    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff);

    for (int x = x_offset; x <= WIDTH; x += TILE_SIZE) 
    { SDL_RenderDrawLine( renderer, x, 0, x, HEIGHT); }
  
    for (int y = y_offset; y <= HEIGHT; y += TILE_SIZE) 
    { SDL_RenderDrawLine( renderer, 0, y, WIDTH, y); }
}

void EditorMode::pan_input()
{
    const EventManager event_manager = context_.event_manager;
    const Vector2D<int> mouse_pos = event_manager.mouse_pos();

    if( event_manager.middle_got_clicked() )
    { mouse_origin_vector_ = mouse_pos - origin_; }

    if( event_manager.middle_is_clicked() && event_manager.mouse_motion() )
    { origin_ = mouse_pos - mouse_origin_vector_; }
}

void EditorMode::update_grid_pos()
{
    Vector2D<int> mouse_pos = context_.event_manager.mouse_pos();
    auto [ x, y ] = mouse_pos - origin_;
      
    grid_pos_.x = static_cast<int>( x/TILE_SIZE ) - ( ( x < 0 ) ? 1 : 0);
    grid_pos_.y = static_cast<int>( y/TILE_SIZE ) - ( ( y < 0 ) ? 1 : 0);
}

void EditorMode::update( float dt )
{
    this->pan_input();
    this->update_grid_pos();  
    buttons_.update();
}

void EditorMode::render()
{
    SDL_Renderer* renderer = context_.renderer;
    SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0xff );
    SDL_RenderClear( renderer );

    this->draw_grid();
    buttons_.render();

    SDL_RenderPresent( renderer );
}

void EditorMode::run( float dt )
{
    this->update( dt );
    this->render();
}



