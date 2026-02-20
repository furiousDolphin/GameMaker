#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include "EditorMode.hpp"

EditorMode::EditorMode( SDL_Renderer* renderer, EventManager& event_manager, GraphicsManager& graphics_manager, ModeType& mode_type ):
    context_{renderer, event_manager, graphics_manager, mode_type},
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
    const auto* textures = context_.graphics_manager.get_text_button_textures_ptr("GAME", GraphicsManager::MINECRAFT_24);
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

    
}

void EditorMode::import_data()
{

}

void EditorMode::update( float dt )
{
    this->pan_input();
    buttons_.update();
    data_manager_.update(dt);
    if (!menu_.update())
    { 
        canva_tiles_.update();
        canva_objects_.update();
    }
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



