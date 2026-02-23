

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <set>

#include "Settings.hpp"
#include "EventManager.hpp"
#include "GraphicsManager.hpp"
#include "Land.hpp"

class Entity
{
    public:
        Entity(Vector2D<int> pos, Animation animation);
        virtual void update(const GraphicsManager& graphics_manager, const EventManager& event_manager, const Land& land, float dt) = 0;
        virtual void render(Vector2D<int> origin) const = 0;
    protected:
        Rect get_rect() const;

        struct Collisions 
        {
            bool top;
            bool bot;
            bool right;
            bool left;
            void reset();

        } collisions_;

        std::vector<Vector2D<int>> neighbour_offsets_;
        std::pair<int, int> neighbour_area_shape_;
        Vector2D<int> pos_;
        Animation animation_;

};


class Player : public Entity
{
    public: 
        Player(Vector2D<int> pos, const GraphicsManager& graphics_manager);
        void update(const GraphicsManager& graphics_manager, const EventManager& event_manager, const Land& land, float dt) override;
        void render(Vector2D<int> origin) const override;
    private:
        static inline double speed_ = 2.0;
        
        Vector2D<double> velocity_;
        static inline double delta_velocity_y_ = 0.1;
        static inline double max_velocity_y_ = 3.0;
};

#endif