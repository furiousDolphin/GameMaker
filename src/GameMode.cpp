#include "GameMode.hpp"

GameMode::GameMode( AppContext& app_context ):
    Mode{ app_context }
{
    
}

void GameMode::update()
{
    
}

void GameMode::render()
{
    SDL_Renderer* renderer = app_context_.renderer;
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
}

void GameMode::run( float dt )
{
    this->update();
    this->render();
}