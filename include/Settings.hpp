
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
#include "Animation.hpp"

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

enum class CanvaType        
{ 
    OBJECT, 
    TILE 
};

enum class WaterType        
{ 
    TOP, 
    BOTTOM 
};

enum class CanvaObjectType 
{ 
    FOREGROUND, 
    BACKGROUND, 
    ENTITIES 
};

enum class FontTypes
{
    MINECRAFT_18,
    MINECRAFT_24,
    MINECRAFT_36
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

inline static std::string EDITOR_DATA_PATH { "editor_data.json" } ;

struct EditorDataSequenceType
{
    std::string style;
    std::string type;
    std::string menu;
    std::string menu_surf;
    std::string preview;
    std::string graphics;
};

using EditorDataType       = std::unordered_map< int, EditorDataSequenceType >;
using PreviewTexturesType  = std::unordered_map< int, std::pair< CanvaType, Texture> >;
using MenuTexturesType     = std::unordered_map< std::string, std::vector<std::pair<int, Texture>> >;
using EditorAnimationsType = std::unordered_map< int, Animation >;


class EditorGraphics;
class GraphicsManager;
class EventManager;



struct AppContext
{   
    SDL_Window*      window;
    SDL_Renderer*    renderer;
    EventManager&    event_manager;
    ModeType&        mode_type;
    GraphicsManager& graphics_manager;
    std::unordered_map< FontTypes, FontManager >& fonts;
};

struct EditorContext
{
    EventManager&    event_manager;
    EditorDataType&  editor_data;
    SDL_Renderer*    renderer;
    Vector2D<int>&   origin;
    int&             canva_id;
    EditorGraphics&  editor_graphics;
};

/*-------------------------------------------------------------------------------*/





#endif