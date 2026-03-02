#ifndef WATER_HPP_
#define WATER_HPP_

#include "Settings.hpp"
#include "GraphicsManager.hpp"
#include "JsonLevelFormat.hpp"
#include "WaveSimulation.hpp"

class WaterObjects;
class WaterObject
{
    public:
        friend WaterObjects;
        WaterObject(Vector2D<int> pos, Vector2D<int> shape);
    private:
        void update(float dt);
        void render(const SDL_Renderer* renderer, const Vector2D<int>& origin) const;
        std::unique_ptr<WaveEquation> wave_eq_ptr_;
        std::unique_ptr<WaveSimulation> wave_sim_ptr_;
        Eigen::VectorXd x_dense_;

        Rect rect_;
        static inline SDL_Color top_color = { 0, 150, 255, 180 }; 
        static inline SDL_Color bot_color = { 0, 50, 150, 220 }; 

};

class WaterObjects
{
    public:
        WaterObjects( const SDL_Renderer* renderer, const Vector2D<int>& origin, const float& dt);
        void load_level(const JsonLevelFormat& json_level_format_data);
        void update();
        void render() const;
        
    private:
        struct Context
        {
            const SDL_Renderer* renderer;
            const Vector2D<int>& origin;
            const float& dt;
        } context_;

        std::vector<WaterObject> water_objects_;

};

#endif