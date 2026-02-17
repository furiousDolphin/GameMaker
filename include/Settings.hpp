
#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

#include "Vector2D.hpp"
#include "Texture.hpp"
#include "SDL_Management.hpp"



/*-------------------------------------------------------------------------------*/

inline constexpr int WIDTH     = 960;
inline constexpr int HEIGHT    = 540;
inline constexpr int TILE_SIZE = 64;

/*-------------------------------------------------------------------------------*/

enum class ModeType 
{ 
    GAME, 
    EDITOR 
};



namespace std 
{
    template<>
    struct hash<ModeType> 
    {
        size_t operator()(const ModeType& mt) const noexcept 
        {
            return std::hash<int>()(static_cast<int>(mt));
        }
    };
}

/*-------------------------------------------------------------------------------*/

class EventManager;
class GraphicsManager;

struct AppContext
{   
    SDL_Window*      window;
    SDL_Renderer*    renderer;
    EventManager&    event_manager;
    GraphicsManager& graphics_manager;
    ModeType&        mode_type;
};

struct EditorContext
{
    SDL_Renderer*    renderer;
    EventManager&    event_manager;
    GraphicsManager& graphics_manager;
    Vector2D<int>&   origin;
};

/*-------------------------------------------------------------------------------*/





#endif