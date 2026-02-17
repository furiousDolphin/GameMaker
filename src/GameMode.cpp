#include "GameMode.hpp"

GameMode::GameMode( AppContext& app_context ):
    Mode{ app_context }
{
    this->create_buttons();
}

void GameMode::create_buttons()
{
    const auto& textures = app_context_.graphics_manager.get_texture("EDITOR", GraphicsManager::MINECRAFT_24);
    buttons_.add(std::make_unique<TextButton>(Vector2D<int>(0, 0), [this](){app_context_.mode_type = ModeType::EDITOR;}, textures));
}

void GameMode::update()
{
    const auto& event_manager = app_context_.event_manager;
    buttons_.update(event_manager);
}

void GameMode::render()
{
    SDL_Renderer* renderer = app_context_.renderer;
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