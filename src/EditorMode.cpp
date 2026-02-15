#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include "EditorMode.hpp"

EditorMode::EditorMode( AppContext& app_context ):
    Mode            { app_context                      },
    editor_data_    { this->make_editor_data()         },
    editor_graphics_{ app_context,
                      editor_data_                     },
    editor_context_ { app_context.event_manager,
                      editor_data_,
                      app_context_.renderer, 
                      origin_, 
                      canva_id_,
                      editor_graphics_                 },
    editor_canvas_  { editor_context_,
                      { editor_context_,
                        CanvaObjectType::BACKGROUND },
                      { editor_context_,
                        CanvaObjectType::ENTITIES   },
                      { editor_context_,
                        CanvaObjectType::FOREGROUND } },    
    editor_menu_    { editor_context_,
                      editor_graphics_                 },
    buttons_        {                                  }
{
    canva_id_ = 2;
    this->create_buttons();
}

EditorDataType EditorMode::make_editor_data()
{
    char* base_path = SDL_GetBasePath();
    if (!base_path) 
        throw std::invalid_argument( "nie wyszlo z base_path" ); 

    const auto& path = EDITOR_DATA_PATH;

    fs::path exe_path(base_path);        
    fs::path project_root = exe_path.parent_path().parent_path(); 
    fs::path file_path = project_root / path;


    std::ifstream file{ file_path };
    if (!file.is_open())
        throw std::invalid_argument("nie moge otworzyc sciezki -> " + path);

    nlohmann::json j;
    file >> j;

    
    auto safe_get_string = [](const nlohmann::json& obj,
                              const std::string& key,
                              const std::string& def) -> std::string
    {
        if (obj.contains(key) && !obj[key].is_null())
            return obj[key].get<std::string>();
        return def;
    };

    EditorDataType editor_data;

    for (const auto& [key, val] : j.items())
    {
        
        if (!std::all_of(key.begin(), key.end(), ::isdigit))
            continue; 

        int id = std::stoi(key);

        editor_data[id] = 
        {
            safe_get_string(val, "style", ""),
            safe_get_string(val, "type", ""),
            safe_get_string(val, "menu", ""),
            safe_get_string(val, "menu_surf", ""),
            safe_get_string(val, "preview", ""),
            safe_get_string(val, "graphics", "")
        };
    }
    return editor_data;


}

void EditorMode::draw_grid() const
{
    int tile_size = 64;

    int x_offset = origin_.x % tile_size;
    int y_offset = origin_.y % tile_size;

    SDL_SetRenderDrawColor( app_context_.renderer, 0x00, 0x00, 0x00, 0xff);

    for (int x = x_offset; x <= WIDTH; x += tile_size) 
    {
        SDL_RenderDrawLine( app_context_.renderer, x, 0, x, HEIGHT);
    }
  
    for (int y = y_offset; y <= HEIGHT; y += tile_size) 
    {
        SDL_RenderDrawLine( app_context_.renderer, 0, y, WIDTH, y);
    }
}

void EditorMode::pan_input()
{
    if( app_context_.event_manager.middle_got_clicked() )
    {
        mouse_origin_vector_ = app_context_.event_manager.mouse_pos() - origin_;
    }

    if( app_context_.event_manager.middle_is_clicked() &&
        app_context_.event_manager.mouse_motion() )
    {
        origin_ = app_context_.event_manager.mouse_pos() - mouse_origin_vector_;
    }

    //if( origin_ == app_context_.event_manager.mouse_pos() )
    //    std::cout << "japierdole\n";
}

void EditorMode::create_buttons()
{
    buttons_.add( std::make_unique<TextButton>( TextButton{ Vector2D<int>{ 20, 20 },
                                                            app_context_.renderer,
                                                            "GAME", 
                                                            { 0x00, 0x00, 0xff }, 
                                                            { 0x00, 0xff, 0x00 }, 
                                                            app_context_.fonts.at( FontTypes::MINECRAFT_24 ).get_font(), 
                                                            [this]() { app_context_.mode_type = ModeType::GAME; } } ) );
}

void EditorMode::update_grid_pos()
{
    auto [ x, y ] = app_context_.event_manager.mouse_pos() - origin_;
      
    grid_pos_.x = static_cast<int>( x/TILE_SIZE ) - ( ( x < 0 ) ? 1 : 0);
    grid_pos_.y = static_cast<int>( y/TILE_SIZE ) - ( ( y < 0 ) ? 1 : 0);
}

void EditorMode::update( float dt )
{
    const auto& mouse_pos  = app_context_.event_manager.mouse_pos();

/*-----------------------------------------------------------------------------------*/

    if( !buttons_.update( app_context_.event_manager ) )
    {

        if( editor_menu_.get_rect().collide_point( mouse_pos ) ) 
        { 
            editor_menu_.update(); 
        }

        else                                                       
        { 
            editor_canvas_.tiles.update();
            editor_canvas_.background_objects.update();
            editor_canvas_.entity_objects.update();
            editor_canvas_.foreground_objects.update();
        }
    }

/*-----------------------------------------------------------------------------------*/

    this->pan_input();
    this->update_grid_pos();  
    
    editor_graphics_.update_animations( dt );
}

void EditorMode::render_preview()
{
    const auto& [canva_type, canva_texture] = editor_graphics_.get_preview( canva_id_ );
    const auto& mouse_pos                   = app_context_.event_manager.mouse_pos();

    auto w = canva_texture.get_width();
    auto h = canva_texture.get_height();

    Vector2D<int> pos;

    switch( canva_type )
    {
        using enum CanvaType;

        case TILE:
            pos = { grid_pos_*TILE_SIZE + origin_ + Vector2D<int>{ 0, TILE_SIZE - h } };
            break;
        case OBJECT:
            pos = { mouse_pos - Vector2D<int>{ w/2, h/2 } };
            break;
    }

    canva_texture.render( pos );
}

void EditorMode::render()
{
    SDL_SetRenderDrawColor( app_context_.renderer, 0xff, 0xff, 0xff, 0xff );
    SDL_RenderClear( app_context_.renderer );


    this->draw_grid();

    editor_canvas_.background_objects.render();
    editor_canvas_.tiles.render(); 
    editor_canvas_.entity_objects.render();
    editor_canvas_.foreground_objects.render();

    this->render_preview();

    editor_menu_.render();
    buttons_.render( app_context_.renderer );

    SDL_RenderPresent( app_context_.renderer );
}

void EditorMode::run( float dt )
{
    this->update( dt );
    this->render();
}



