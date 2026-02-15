#ifndef GRAPHICS_MANAGER_HPP_
#define GRAPHICS_MANAGER_HPP_

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>


#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Texture.hpp"
#include "Animation.hpp"




class GraphicsManager
{
    public:
        GraphicsManager( SDL_Renderer* renderer );
        ~GraphicsManager() = default;

        enum TextureKey
        {
            CLOUD_SMALL,
            CLOUD_MIDDLE,
            CLOUD_BIG,
            LAND,
            CURSOR_MOUSE,
            CURSOR_HANDLE,
            PEARL,
            WATER_BOTTOM
        };

        enum AnimationKey
        {
            WATER_TOP,
            SPIKES,
            LEFT_SHELL_ATTACK,
            LEFT_SHELL_IDLE,
            RIGHT_SHELL_ATTACK,
            RIGHT_SHELL_IDLE,
            TOOTH_IDLE,
            TOOTH_RUN_LEFT,
            TOOTH_RUN_RIGHT,
            PALM_LARGE_BG,
            PALM_LARGE_FG,
            PALM_LEFT_BG,
            PALM_LEFT_FG,
            PALM_RIGHT_BG,
            PALM_RIGHT_FG,
            PALM_SMALL_BG,
            PALM_SMALL_FG,
            PLAYER_FALL_LEFT,
            PLAYER_FALL_RIGHT,
            PLAYER_IDLE_LEFT,
            PLAYER_IDLE_RIGHT,
            PLAYER_JUMP_LEFT,
            PLAYER_JUMP_RIGHT,
            PLAYER_RUN_LEFT,
            PLAYER_RUN_RIGHT,
            DIAMOND,
            GOLD,
            SILVER,
            PARTICLE
        };

        Animation copy_animation( AnimationKey key        ) const;
        Animation copy_animation( const std::string& path ) const;

        const Texture& get_texture( const std::string& path   ) const;
        const Texture& get_texture( TextureKey key            ) const;
        const Texture& get_texture( TextureKey key, int index ) const;

        const std::unordered_map< std::string, int >& get_translator( TextureKey key ) const;
        std::optional<int>                            translate(      TextureKey key, const std::string& string_type_key ) const;

    private:
        inline static std::unordered_map<std::string, TextureKey> textures_as_vector_paths
        {    
            { "data/graphics/terrain/land", LAND }
        };

        inline static std::unordered_map<std::string, TextureKey> textures_as_map_paths
        {
            { "data/graphics/clouds/Small Cloud 1.png", CLOUD_SMALL },
            { "data/graphics/clouds/Small Cloud 2.png", CLOUD_MIDDLE },
            { "data/graphics/clouds/Small Cloud 3.png", CLOUD_BIG },

            { "data/graphics/cursors/mouse.png",  CURSOR_MOUSE },
            { "data/graphics/cursors/handle.png", CURSOR_HANDLE },

            { "data/graphics/enemies/pearl/pearl.png",   PEARL },

            { "data/graphics/terrain/water/water_bottom.png", WATER_BOTTOM }
        };

        inline static std::unordered_map<std::string, AnimationKey> animation_paths
        {
            { "data/graphics/terrain/water/animation", WATER_TOP },
            { "data/graphics/enemies/spikes", SPIKES },

            { "data/graphics/enemies/shell_left/attack", LEFT_SHELL_ATTACK },
            { "data/graphics/enemies/shell_left/idle",   LEFT_SHELL_IDLE },

            { "data/graphics/enemies/shell_right/attack", RIGHT_SHELL_ATTACK },
            { "data/graphics/enemies/shell_right/idle",   RIGHT_SHELL_IDLE },

            { "data/graphics/enemies/tooth/idle",      TOOTH_IDLE },
            { "data/graphics/enemies/tooth/run_left",  TOOTH_RUN_LEFT },
            { "data/graphics/enemies/tooth/run_right", TOOTH_RUN_RIGHT },

            { "data/graphics/terrain/palm/large_bg", PALM_LARGE_BG },
            { "data/graphics/terrain/palm/large_fg", PALM_LARGE_FG },
            { "data/graphics/terrain/palm/left_bg",  PALM_LEFT_BG },
            { "data/graphics/terrain/palm/left_fg",  PALM_LEFT_FG },
            { "data/graphics/terrain/palm/right_bg", PALM_RIGHT_BG },
            { "data/graphics/terrain/palm/right_fg", PALM_RIGHT_FG },
            { "data/graphics/terrain/palm/small_bg", PALM_SMALL_BG },
            { "data/graphics/terrain/palm/small_fg", PALM_SMALL_FG },

            { "data/graphics/player/fall_left",  PLAYER_FALL_LEFT },
            { "data/graphics/player/fall_right", PLAYER_FALL_RIGHT },
            { "data/graphics/player/idle_left",  PLAYER_IDLE_LEFT },
            { "data/graphics/player/idle_right", PLAYER_IDLE_RIGHT },
            { "data/graphics/player/jump_left",  PLAYER_JUMP_LEFT },
            { "data/graphics/player/jump_right", PLAYER_JUMP_RIGHT },
            { "data/graphics/player/run_left",   PLAYER_RUN_LEFT },
            { "data/graphics/player/run_right",  PLAYER_RUN_RIGHT },

            { "data/graphics/items/diamond",  DIAMOND },
            { "data/graphics/items/gold",     GOLD },
            { "data/graphics/items/silver",   SILVER },
            { "data/graphics/items/particle", PARTICLE }
        };



        // template <typename Container, typename Key>
        // auto find_item(const Container& container, const Key& key)
        // {
        //     if constexpr (requires { container.find(key); })
        //         return container.find(key);
        //     else 
        //         return std::find(container.begin(), container.end(), key);
        // }



        std::unordered_map< TextureKey,   Texture   > textures_as_map;
        std::unordered_map< AnimationKey, Animation > animations;

        struct TexturesAsVectorType
        {
            std::vector< Texture >                 textures;
            std::unordered_map< std::string, int > translator;
        };

        std::unordered_map< TextureKey, TexturesAsVectorType > textures_as_vector;



    
};

#endif