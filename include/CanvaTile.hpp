
#ifndef CANVA_TILES_HPP_
#define CANVA_TILES_HPP_

#include <vector>
#include <unordered_map>
#include "Vector2D.hpp"

class CanvaTile
{
    public:
        CanvaTile();
    private:
        bool has_terrain_;
        int terrain_id_;
        bool has_water_;
};

struct CanvaTilesContext
{
    const EventManager& event_manager;
    const GraphicsManager& graphics_manager;
    const Vector2D<int>& origin;
};

class CanvaTiles
{
    public:
        CanvaTiles(EventManager& event_manager, GraphicsManager& graphics_manager, Vector2D<int>& origin);
        void update();
        void render();

    private:
        struct Context
        {
            EventManager& event_manager;
            GraphicsManager& graphics_manager;
            Vector2D<int>& origin;
        } context_;

        std::unordered_map<Vector2D<int>, CanvaTile> canva_tiles_;
};

#endif