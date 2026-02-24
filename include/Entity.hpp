

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
        Entity(Vector2D<double> centre, Vector2D<int> shape);
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
        Vector2D<double> pos_;
        Vector2D<int> shape_;
        Animation animation_;

};


class Player : public Entity
{
    public: 
        Player(Vector2D<double> centre);
        void update(const GraphicsManager& graphics_manager, const EventManager& event_manager, const Land& land, float dt) override;
        void render(Vector2D<int> origin) const override;
        Vector2D<double> get_center() const;
    private:

        bool set_action_;
        bool in_the_air_;

        static inline double speed_ = 200.0;
        
        Vector2D<double> velocity_;
        static inline double gravity_ = 1100.0;
        static inline double max_velocity_y_ = 800.0;
        static inline double jump_velocity_ = -550.0;
};


class Entities
{
    public:
        Entities(
            const GraphicsManager& graphics_manager, 
            const EventManager& event_manager, 
            const Land& land, 
            const float& dt, 
            const Vector2D<int>& origin);
        
        void load_level(const JsonLevelFormat& json_level_format_data);
        void add(std::unique_ptr<Entity> entity_ptr);
        void update();
        void render() const;
    private:

        struct Context
        {
            const GraphicsManager& graphics_manager;
            const EventManager& event_manager;
            const Land& land;
            const float& dt;
            const Vector2D<int>& origin;
        } context_;

        std::vector<std::unique_ptr<Entity>> entities_;
};

#endif