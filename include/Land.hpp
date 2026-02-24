

#ifndef LAND_HPP_
#define LAND_HPP_

#include "Settings.hpp"
#include "GraphicsManager.hpp"
#include "JsonLevelFormat.hpp"

class Land
{
    public:
        Land(const GraphicsManager& graphics_manager, const Vector2D<int>& origin);
        
        void load_level(const JsonLevelFormat& json_level_format_data);
        std::vector<Vector2D<int>> get_physical_tiles_around(
            Vector2D<double> entity_pos, 
            const std::vector<Vector2D<int>>& entity_neighbour_offsets ) const;

        void update();
        void render() const;
    private:
        struct Context
        {
            const GraphicsManager& graphics_manager;
            const Vector2D<int>& origin;
        } context_;

        std::unordered_map<Vector2D<int>, int> land_tiles_;
};

#endif
