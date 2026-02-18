
#ifndef EDITORMENU_HPP_
#define EDITORMENU_HPP_


#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include "Vector2D.hpp"
#include "Rect.hpp"

#include "Button.hpp"
#include "Settings.hpp"
#include "EditorDataManager.hpp"

class EditorMenu
{
    public:
        EditorMenu( 
            EventManager& event_manager, 
            GraphicsManager& graphics_manager, 
            Vector2D<int>& origin, 
            EditorDataManager& editor_data_manager, 
            int& canva_id );

        void update();
        void render();

    private:
        void create_buttons();
        void create_data();

        struct Context
        {
            EventManager& event_manager;
            Vector2D<int>& origin;
            EditorDataManager& editor_data_manager;
            int& canva_id;

        } context_;


        Rect rect_;
        Buttons buttons_;

        std::unordered_map<std::string, std::vector<std::pair<int, const Texture*>>> textures_;
};

#endif