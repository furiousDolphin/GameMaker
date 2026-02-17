#include "GameMode.hpp"

GameMode::GameMode( SDL_Renderer* renderer, EventManager& event_manager, GraphicsManager& graphics_manager, ModeType& mode_type ) :
    context_{renderer, event_manager, graphics_manager, mode_type},
    buttons_{event_manager, graphics_manager}
{
    this->create_buttons();
}

void GameMode::create_buttons()
{
    const auto& textures = context_.graphics_manager.get_texture("EDITOR", GraphicsManager::MINECRAFT_24);
    buttons_.add(std::make_unique<TextButton>(Vector2D<int>(0, 0), [this](){context_.mode_type = ModeType::EDITOR;}, textures));
}

void GameMode::update()
{
    buttons_.update();
}

void GameMode::render()
{
    SDL_Renderer* renderer = context_.renderer;
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
    SDL_RenderClear( renderer );

    buttons_.render();
    SDL_RenderPresent( renderer );
}

void GameMode::run( float dt )
{
    this->update();
    this->render();
}