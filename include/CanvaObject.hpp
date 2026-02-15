

#ifndef CANVAOBJECT_HPP_
#define CANVAOBJECT_HPP_

#include <memory>
#include <unordered_map>

#include "Vector2D.hpp"
#include "Rect.hpp"

#include "Settings.hpp"


class CanvaObject
{
    public:
        CanvaObject( Rect rect, int id ): 
            rect_       { rect  },
            id_         { id    },
            is_grabbed_ { false } {}

        void grab_and_move( const EditorContext& editor_context );
        void update(        const EditorContext& editor_context );
        void render(        const EditorContext& editor_context );
        bool is_grabbed() { return is_grabbed_; }

        Rect get_rect() { return rect_; }

    private:

        Rect          rect_;
        Vector2D<int> grab_pos_;
        bool          is_grabbed_;
        int           id_;
};

class CanvaObjects
{
    public:
        CanvaObjects( const EditorContext& editor_context, CanvaObjectType canva_objects_type ): 
            editor_context_    { editor_context     },
            canva_objects_type_{ canva_objects_type } 
        {}

        void update();
        bool verify_canva_object_type( int id );
        void add( Vector2D<int> pos, int id );
        void render();

    private:
        const EditorContext& editor_context_;
        std::unordered_map< Vector2D<int>, CanvaObject > canva_objects_;

        CanvaObjectType canva_objects_type_;
};

#endif