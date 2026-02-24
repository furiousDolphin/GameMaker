
#ifndef CANVATILE_HPP_
#define CANVATILE_HPP_

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>


#include "Vector2D.hpp"
#include "Rect.hpp"

#include "Settings.hpp"
#include "EventManager.hpp"
#include "GraphicsManager.hpp"
#include "EditorDataManager.hpp"
#include "JsonLevelFormat.hpp"



class CanvaTiles;
class CanvaTile
{
    public:
        friend CanvaTiles;
        CanvaTile() = default;
        CanvaTile( int id, const EditorDataManager& editor_data_manager);
        void add_id(int canva_id, const EditorDataManager& editor_data_manager);

    private:
        bool get_neighbours_flag() const;
        bool has_land() const;
        bool has_water() const;
        bool any_id() const;
        void remove_id(int canva_id, const EditorDataManager& editor_data_manager);
        void update();
        void render( Vector2D<int> pos, const GraphicsManager& graphics_manager ) const;

        int land_index_;
        bool has_water_;
        bool neighbours_flag_;      
};

class CanvaTiles
{
    public:
        CanvaTiles( 
            EventManager& event_manager, 
            GraphicsManager& graphics_manager, 
            Vector2D<int>& origin, 
            EditorDataManager& editor_data_manager,
            int& canva_id );

        void export_data(JsonLevelFormat& json_level_format_data) const;
        void import_data(const JsonLevelFormat& json_level_format_data);
        void clear(); 

        void update();
        void render() const;

        using iterator = std::unordered_map< Vector2D<int>, CanvaTile >::iterator;
        using const_iterator = std::unordered_map< Vector2D<int>, CanvaTile >::const_iterator;

        iterator emplace(const Vector2D<int>& pos, CanvaTile canva_tile);

        iterator find(const Vector2D<int> grid_pos);
        const_iterator find(const Vector2D<int> grid_pos) const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

    private:
        void add_id();
        void remove_id();
        void check_neighbours();

        static inline std::array< std::pair< Vector2D<int>, std::string >, 8 > neighbours_ 
        { { { { 0, -1 }, "A" },
            { { 1, -1 }, "B" },
            { { 1,  0 }, "C" },
            { { 1,  1 }, "D" },
            { { 0,  1 }, "E" },
            { {-1,  1 }, "F" },
            { {-1,  0 }, "G" },
            { {-1, -1 }, "H" } } };

        struct Context
        {
            EventManager& event_manager;
            GraphicsManager& graphics_manager;
            Vector2D<int>& origin;
            int& canva_id;
            EditorDataManager& editor_data_manager;
        } context_;

        std::unordered_map< Vector2D<int>, CanvaTile > canva_tiles_;
              
};

#endif