
#include <utility>
#include <iostream>
#include <memory>


#include "EditorMenu.hpp"


EditorMenu::EditorMenu( EditorContext& editor_context, const EditorGraphics& editor_graphics ):
    editor_context_ { editor_context  },
    editor_graphics_{ editor_graphics }
{
    int margin = 10;
    rect_ = {{ WIDTH  - 2*TILE_SIZE - 4*margin,
               HEIGHT - 2*TILE_SIZE - 4*margin,
               2*TILE_SIZE + 4*margin,
               2*TILE_SIZE + 4*margin }};

    this->create_buttons();
}

void EditorMenu::create_buttons()
{
    int margin = 10;

          auto& id       = editor_context_.canva_id;
    const auto& textures = editor_graphics_.get_menu_textures();

    buttons_.add(std::make_unique<EditorMenuButton>( rect_.get_pos() + Vector2D<int>( margin,               margin               ), &textures.at("terrain"), nullptr,                 [&id]( int new_id ) { id = new_id; } ));
    buttons_.add(std::make_unique<EditorMenuButton>( rect_.get_pos() + Vector2D<int>( 3*margin + TILE_SIZE, margin               ), &textures.at("coin"   ), nullptr,                 [&id]( int new_id ) { id = new_id; } ));
    buttons_.add(std::make_unique<EditorMenuButton>( rect_.get_pos() + Vector2D<int>( margin,               3*margin + TILE_SIZE ), &textures.at("enemy"  ), nullptr,                 [&id]( int new_id ) { id = new_id; } ));
    buttons_.add(std::make_unique<EditorMenuButton>( rect_.get_pos() + Vector2D<int>( 3*margin + TILE_SIZE, 3*margin + TILE_SIZE ), &textures.at("palm fg"), &textures.at("palm bg"), [&id]( int new_id ) { id = new_id; } ));
}

void EditorMenu::update()
{
    buttons_.update( editor_context_.event_manager );
}

void EditorMenu::render()
{
    buttons_.render( editor_context_.renderer );
}






