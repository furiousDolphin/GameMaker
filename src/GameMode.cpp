
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include "GameMode.hpp"

GameMode::GameMode( SDL_Renderer* renderer, EventManager& event_manager, GraphicsManager& graphics_manager, PersistentState& persistent_state ) :
    context_{renderer, event_manager, graphics_manager, persistent_state},
    buttons_{event_manager, graphics_manager}
{
    this->create_buttons();
}

void GameMode::create_buttons()
{
    const auto* main_menu_textures = context_.graphics_manager.get_text_button_textures_ptr("MAIN MENU", GraphicsManager::MINECRAFT_24);
    auto main_menu_button_func = [this]()
    {context_.persistent_state.mode = ModeType::MAIN_MENU;};
    buttons_.add(std::make_unique<TextButton>(Vector2D<int>(0, 0), main_menu_button_func, main_menu_textures));
}

void GameMode::import_data()
{
    std::string file_name = context_.persistent_state.level;
    std::ifstream file{file_name};
    if ( !file.is_open() )
    { throw std::runtime_error("nie udalo sie otworzyc map.json"); }

    using json = nlohmann::json;
    json j;
    ExportFormat imported_data;

    file >> j;
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