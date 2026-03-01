
#include <utility>
#include <iostream>
#include <memory>


#include "EditorMenu.hpp"


EditorMenu::EditorMenu( 
    EventManager& event_manager, 
    GraphicsManager& graphics_manager, 
    Vector2D<int>& origin, 
    EditorDataManager& editor_data_manager, 
    int& canva_id 
) :
    context_{event_manager, origin, editor_data_manager, canva_id},
    buttons_{event_manager, graphics_manager}
{
    int margin = 10;

    rect_ = 
    {{ 
        WIDTH  - 2*TILE_SIZE - 4*margin,
        HEIGHT - 2*TILE_SIZE - 4*margin,
        2*TILE_SIZE + 4*margin,
        2*TILE_SIZE + 4*margin 
    }};

    this->create_data();
    this->create_buttons();
}

void EditorMenu::create_buttons()
{
    int margin = 10;
    auto& id = context_.canva_id;

    buttons_.add(std::make_unique<EditorMenuButton>( rect_.get_pos() + Vector2D<int>( margin, margin), [&id]( int new_id ) { id = new_id; }, &textures_.at("terrain"), nullptr ));
    buttons_.add(std::make_unique<EditorMenuButton>( rect_.get_pos() + Vector2D<int>( 3*margin + TILE_SIZE, margin), [&id]( int new_id ) { id = new_id; }, &textures_.at("enemy"), nullptr ));
    buttons_.add(std::make_unique<EditorMenuButton>( rect_.get_pos() + Vector2D<int>( margin, 3*margin + TILE_SIZE), [&id]( int new_id ) { id = new_id; }, &textures_.at("coin"), nullptr ));
    buttons_.add(std::make_unique<EditorMenuButton>( rect_.get_pos() + Vector2D<int>( 3*margin + TILE_SIZE, 3*margin + TILE_SIZE), [&id]( int new_id ) { id = new_id; }, &textures_.at("palm fg"), &textures_.at("palm bg") ));


}

void EditorMenu::create_data()
{
    for ( const auto& [id, series] : context_.editor_data_manager )
    {
        if ( series.menu != "" ) // zastapic za pomoca std::variant
        {
            auto it = textures_.find(series.menu);
            if ( it == textures_.end() )
            { 
                std::vector<std::pair<int, const Texture*>> v{{id, series.menu_texture}};
                textures_.emplace(series.menu, std::move(v));
            }

            else
            { 
                auto& v = it->second;
                v.emplace_back(id, series.menu_texture); 
            }
        }
    }
}

bool EditorMenu::update()
{
    Vector2D<int> mouse_pos = context_.event_manager.mouse_pos(); 

    bool collision = false;
    if ( rect_.collide_point(mouse_pos) )
    {
        collision = true;
        buttons_.update();
    }
    return collision;
}

void EditorMenu::render()
{ buttons_.render(); }






