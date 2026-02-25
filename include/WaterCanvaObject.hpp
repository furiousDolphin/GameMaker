

#ifndef WATER_CANVA_OBJECT_
#define WATER_CANVA_OBJECT_

#include <memory>
#include <unordered_map>

#include "Vector2D.hpp"
#include "Rect.hpp"

#include "Settings.hpp"
#include "EventManager.hpp"
#include "GraphicsManager.hpp"
#include "EditorDataManager.hpp"
#include "JsonLevelFormat.hpp"



class WaterCanvaObjects;
class WaterCanvaObject
{
    public:
        friend WaterCanvaObjects;
        WaterCanvaObject(Vector2D<int> top_left, Vector2D<int> grid_shape = {1, 1});
        Rect* get_colliding_rect_ptr(Vector2D<int> p);

        void render(SDL_Renderer* renderer, Vector2D<int> origin) const;
    private:

        class SlideRect : public Rect
        {
            public:
                enum Axis;
                SlideRect(
                    Vector2D<int> center, 
                    Vector2D<int> shape, 
                    SlideRect::Axis axis, 
                    std::function<int(void)> getter,  
                    std::function<void(int)> setter);

                void set_pos(Vector2D<int> new_top_left);
                Vector2D<int> get_pos() const;

                enum Axis
                { HORIZONTAL, VERTICAL };

            private:
                Axis axis_;
                std::function<int(void)> getter_;  
                std::function<void(int)> setter_;        
        };

        Rect main_r_;
        Rect boundary_r_;
        std::vector<std::unique_ptr<SlideRect>> slide_rects_;
};

class WaterCanvaObjects
{
    public:
        WaterCanvaObjects( 
            SDL_Renderer* renderer,
            EventManager& event_manager, 
            Vector2D<int>& origin, 
            EditorDataManager& editor_data_manager,
            int& canva_id);

        void export_data(JsonLevelFormat& json_level_format_data) const;
        void import_data(const JsonLevelFormat& json_level_format_data);     
        void clear();   
        void update();
        void render();

        using iterator = std::vector<WaterCanvaObject>::iterator;
        using const_iterator = std::vector<WaterCanvaObject>::const_iterator;
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

    private:
        void add(Vector2D<int> pos, Vector2D<int> grid_shape = {1, 1});
        void remove(std::vector<WaterCanvaObject>::iterator it);
        void remove(std::vector<WaterCanvaObject>::reverse_iterator rit);


        struct Context
        {
            SDL_Renderer* renderer;
            EventManager& event_manager;
            Vector2D<int>& origin; 
            int& canva_id;
            EditorDataManager& editor_data_manager;
        } context_;


        class GrabbedState
        {
            public:
                GrabbedState();        
                WaterCanvaObject& operator*() const;
                explicit operator bool() const; 
                void clear(); 
                void set_new(WaterCanvaObject* new_obj_ptr, Rect* new_rect_ptr, Vector2D<int> global_mouse_pos); 
                void update(Vector2D<int> global_mouse_pos);

            private:
                WaterCanvaObject* obj_ptr_;
                Rect* rect_ptr_;
                bool active_;
                Vector2D<int> offset_;
        };

        GrabbedState grabbed_;
        std::vector<WaterCanvaObject> water_canva_objects_;
};
#endif