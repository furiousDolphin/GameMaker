
#ifndef EDITORMENU_HPP_
#define EDITORMENU_HPP_

#include "Settings.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include "Vector2D.hpp"
#include "Rect.hpp"

#include "Button.hpp"
#include "Settings.hpp"
#include "EditorGraphics.hpp"


class EditorMenu
{
    public:
        EditorMenu( EditorContext& editor_context, const EditorGraphics& editor_graphics ); 

        void load_data();
        void create_buttons();
        const Rect& get_rect() { return rect_; }
        void load_textures();
        void update();
        void render();

    private:
        Rect    rect_;
        Buttons buttons_;

        EditorContext&        editor_context_;
        const EditorGraphics& editor_graphics_;
};

#endif