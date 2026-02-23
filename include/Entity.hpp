

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <set>

#include "Settings.hpp"
#include "EventManager.hpp"
#include "GraphicsManager.hpp"

class Entity
{
    public:
        Entity(Vector2D<int> pos, Animation animation);
        virtual void update(const GraphicsManager& graphics_manager, const EventManager& event_manager, float dt) = 0;
        virtual void render() = 0;
    protected:
        struct Collisions 
        {
            bool top;
            bool bot;
            bool right;
            bool left;
            void reset();

        } collisions_;

        std::set<Vector2D<int>> neighbour_offsets_;
        Vector2D<int> pos_;
        Animation animation_;
};


class Player : public Entity
{
    public: 
        Player(Vector2D<int> pos, const GraphicsManager& graphics_manager);
        void update(const GraphicsManager& graphics_manager, const EventManager& event_manager, float dt) override;
        void render() override;
    private:
        


};

#endif