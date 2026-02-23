#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include "EditorMode.hpp"

EditorMode::EditorMode( SDL_Renderer* renderer, EventManager& event_manager, GraphicsManager& graphics_manager, PersistentState& persistent_state ):
    context_{renderer, event_manager, graphics_manager, persistent_state},
    buttons_{event_manager, graphics_manager},
    data_manager_{graphics_manager},
    menu_{event_manager, graphics_manager, origin_, data_manager_, canva_id_},
    canva_tiles_{event_manager, graphics_manager, origin_, data_manager_, canva_id_},
    canva_objects_{event_manager, graphics_manager, origin_, data_manager_, canva_id_}
{
    canva_id_ = 2;
    this->create_buttons();
}

void EditorMode::create_buttons()
{
    const auto* main_menu_textures = context_.graphics_manager.get_text_button_textures_ptr("MAIN MENU", GraphicsManager::MINECRAFT_24);
    auto main_menu_func = [this]()
    { context_.persistent_state.mode = ModeType::MAIN_MENU; this->export_data(); };
    buttons_.add(std::make_unique<TextButton>(Vector2D<int>(0, 0), main_menu_func, main_menu_textures));
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

void EditorMode::export_data()
{
    for ( const auto& canva_object : canva_objects_ )
    {
        Vector2D<int> pos = canva_object.get_pos();
        Vector2D<int> grid_pos = pos.to_grid(TILE_SIZE);
        Vector2D<int> offset = TILE_SIZE*grid_pos - pos;

        auto it = canva_tiles_.find(grid_pos);
        if ( it != canva_tiles_.end() )
        {
            auto& [_, canva_tile] = *it;
            canva_tile.add_id(canva_id_, data_manager_, offset);
        }
        else
        { canva_tiles_.emplace(grid_pos, CanvaTile(canva_id_, data_manager_, offset)); }
    }  

    ExportFormat export_map = canva_tiles_.export_data();
    std::string file_name = "map.json";
    
    using json = nlohmann::json;

    json j;
    for ( const auto& [series_key, tiles] : export_map)
    {
        for ( const auto& [pos_key, val] : tiles )
        {
            std::visit(
                [&](auto&& arg)
                {j[series_key][pos_key] = arg;}, 
                val);  
        }
    }

    std::ofstream file{file_name};
    if (file.is_open())
    {file<<j.dump(4);}

    canva_tiles_.clear();
    canva_objects_.clear();
}

void EditorMode::import_data()
{
    std::string file_name = context_.persistent_state.level;
    std::ifstream file{file_name};
    if ( !file.is_open() )
    { throw std::runtime_error("nie udalo sie otworzyc map.json"); }

    using json = nlohmann::json;
    json j;
    ExportFormat imported_data;

    file >> j;

    for ( auto& [series_key, tiles] : j.items() )
    {
        for ( auto& [pos_key, val] : tiles.items() )
        {
            if ( val.is_number_integer() )
            { imported_data[series_key][pos_key] = val.get<int>(); }
            else if ( val.is_string() )
            { imported_data[series_key][pos_key] = val.get<std::string>(); }
        }
    }


    canva_tiles_.clear();
    canva_tiles_.import_data(imported_data);
    canva_objects_.clear();
    canva_objects_.import_data(imported_data);
}

void EditorMode::update( float dt )
{
    auto& request_editor_reload = context_.persistent_state.request_editor_reload;
    if (request_editor_reload)
    {
        request_editor_reload = false;
        this->import_data();
    }

    this->pan_input();
    buttons_.update();
    data_manager_.update(dt);
    if (!menu_.update())
    { 
        canva_tiles_.update();
        canva_objects_.update();
    }

    if (context_.event_manager.key_down(SDL_SCANCODE_S))
    { this->export_data(); }
}

void EditorMode::render()
{
    SDL_Renderer* renderer = context_.renderer;
    SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0xff );
    SDL_RenderClear( renderer );

    this->draw_grid();
    canva_tiles_.render();
    canva_objects_.render();
    buttons_.render();
    menu_.render();

    //------------------------------------------------------------------------------------

    const auto* prev_texture_ptr = data_manager_.get_series(canva_id_).preview_texture;
    Vector2D<int> mouse_pos = context_.event_manager.mouse_pos();

    if (data_manager_.is_object(canva_id_))
    {
        int w = prev_texture_ptr->get_width();
        int h = prev_texture_ptr->get_height();
        prev_texture_ptr->render(mouse_pos.x - w/2, mouse_pos.y - h/2);
    }
    else if ( data_manager_.is_tile(canva_id_) )  
    { prev_texture_ptr->render(TILE_SIZE*mouse_pos.to_grid(TILE_SIZE)); }

    //------------------------------------------------------------------------------------

    SDL_RenderPresent( renderer );
}

void EditorMode::run( float dt )
{
    this->update( dt );
    this->render();
}



