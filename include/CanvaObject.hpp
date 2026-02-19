

#ifndef CANVAOBJECT_HPP_
#define CANVAOBJECT_HPP_

#include <memory>
#include <unordered_map>

#include "Vector2D.hpp"
#include "Rect.hpp"

#include "Settings.hpp"
#include "EventManager.hpp"
#include "GraphicsManager.hpp"
#include "EditorDataManager.hpp"

class CanvaObjects;
class CanvaObject
{
    public:
        friend CanvaObjects;
        CanvaObject( int id ); 

    private:
        Rect& get_rect();
        void update( const Vector2D<int>& origin, const EventManager& event_manager );
        void render( const Vector2D<int>& origin, const GraphicsManager& graphics_manager ) const;

        Rect rect_;
        int id_;
};

class CanvaObjects
{
    public:
        CanvaObjects( 
            EventManager& event_manager, 
            GraphicsManager& graphics_manager, 
            Vector2D<int>& origin, 
            EditorDataManager& editor_data_manager,
            int& canva_id);

        void update();
        void render();

    private:
        void add();
        void remove(std::vector<CanvaObject>::iterator it);
        void remove(std::vector<CanvaObject>::reverse_iterator rit);


        struct Context
        {
            EventManager& event_manager;
            GraphicsManager& graphics_manager; 
            Vector2D<int>& origin; 
            int& canva_id;
            EditorDataManager& editor_data_manager;
        } context_;


        struct GrabbedState
        {
            CanvaObject* obj_ptr = nullptr;
            bool active = false;
            Vector2D<int> offset;

            CanvaObject& operator*() const 
            { return *obj_ptr; }
            explicit operator bool() const 
            { return active; }
            void clear() 
            {
                active = false;
                obj_ptr = nullptr;
            }
            void set_new(CanvaObject* new_obj_ptr, Vector2D<int> global_mouse_pos) 
            {   
                obj_ptr = new_obj_ptr; 
                active=true;
                offset = global_mouse_pos - new_obj_ptr->get_rect().get_pos();
            }
        } grabbed_;


        template<typename K, typename V>
        class ChronologicalMap 
        {
            public:
                void insert(K key, V val) 
                {
                    auto it = indices_.find(key);
                    if (it != indices_.end()) 
                    { it->second->second = val; } 
                    
                    else 
                    {
                        order_.emplace_back(key, val);
                        indices_[key] = --order_.end();
                    }
                }

                void erase(K key) 
                {
                    auto it = indices_.find(key);
                    if (it != indices_.end()) 
                    {
                        order_.erase(it->second);
                        indices_.erase(it);
                    }
                }

                V& at(K key) 
                { return indices_.at(key)->second; }

                auto rbegin() 
                { return order_.rbegin(); }
                auto rend()   
                { return order_.rend(); }
                auto begin()  
                { return order_.begin(); }
                auto end()    
                { return order_.end(); }

            private:
                std::list<std::pair<K, V>> order_;
                std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> indices_;
        };

        //ChronologicalMap<Vector2D<int>, CanvaObject> canva_objects_;
        std::vector<CanvaObject> canva_objects_;
};

#endif