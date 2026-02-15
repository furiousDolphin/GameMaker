
#ifndef CANVATILE_HPP_
#define CANVATILE_HPP_

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>


#include "Vector2D.hpp"
#include "Rect.hpp"

#include "Settings.hpp"


//dodać podział na render-context i update_context    pojebane


class CanvaTile
{
    public:
        CanvaTile( Vector2D<int> grid_pos );

        const Vector2D<int>& get_grid_pos() const { return grid_pos_; }

        bool get_check_neighbours_flag() { return check_neighbours_flag_; }

        void add_id(    const EditorContext& editor_context );
        void remove_id( const EditorContext& editor_context );

        bool any_id();
        
        bool has_land()  { return has_land_; } 
        bool has_water() { return has_water_; } 

        void set_land_index( int index )            { land_index_ = index;      }
        void set_water_type( WaterType water_type ) { water_type_ = water_type; }

        void update( const EditorContext& editor_context );
        void render( const EditorContext& editor_context );


    private:
        Vector2D<int> grid_pos_;

        bool has_land_;
        int  land_index_;

        bool has_water_;
        WaterType water_type_;

        bool has_coin_;
        int coin_id_;

        bool has_enemy_;
        int enemy_id_;

        bool check_neighbours_flag_;

        std::vector< std::pair< Vector2D<int>, int > > objects_;       
};

class CanvaTiles
{
    public:
        CanvaTiles( const EditorContext& editor_context );

        void add_id();
        void remove_id();
        void check_neighbours();
        void update_mouse_grid_pos();
        void clear() { canva_tiles_.clear(); }
        void update();
        void render();

    private:
        Vector2D<int> mouse_grid_pos_;

        std::array< std::pair< Vector2D<int>, std::string >, 8 > neighbours_;
        std::unordered_map< Vector2D<int>, CanvaTile >           canva_tiles_;
        
        const EditorContext& editor_context_;        
};

#endif