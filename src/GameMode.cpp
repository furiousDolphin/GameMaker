
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include "GameMode.hpp"

GameMode::GameMode( 
    SDL_Renderer* renderer, 
    EventManager& event_manager, 
    GraphicsManager& graphics_manager, 
    PersistentState& persistent_state, 
    float& dt 
) :
    context_{renderer, event_manager, graphics_manager, persistent_state, dt},
    buttons_{event_manager, graphics_manager},
    land_{graphics_manager, origin_},
    entities_{graphics_manager, event_manager, land_, dt, origin_}
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
    JsonLevelFormat json_level_format_data{context_.persistent_state.level};
    json_level_format_data.import_from_json();

    std::cout << "import data game" << "\n";
    land_.load_level(json_level_format_data);
    entities_.load_level(json_level_format_data);
}

void GameMode::update()
{
    auto& request_game_reload = context_.persistent_state.request_game_reload;
    if (request_game_reload)
    {
        request_game_reload = false;
        this->import_data();
    }

    land_.update();
    buttons_.update();
    entities_.update();

    


    // self.render_scroll[0] += (self.player.rect().centerx - self.surf.get_width()/2 - self.render_scroll[0]) / 30
    // self.render_scroll[1] += (self.player.rect().centery - self.surf.get_height()/2 - self.render_scroll[1]) / 30
    // self.offset = (int(self.render_scroll[0]), int(self.render_scroll[1]))
}

void GameMode::render()
{
    SDL_Renderer* renderer = context_.renderer;
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
    SDL_RenderClear( renderer );

    land_.render();
    buttons_.render();
    entities_.render();
    
    SDL_RenderPresent( renderer );
}

void GameMode::run()
{
    this->update();
    this->render();
}