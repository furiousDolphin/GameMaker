#ifndef GRAPHICS_MANAGER_HPP_
#define GRAPHICS_MANAGER_HPP_

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>
#include <set>
#include <variant>



#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Texture.hpp"
#include "Animation.hpp"
#include "Settings.hpp"


struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1); 
    }
};


class GraphicsManager
{
    public:
        GraphicsManager( SDL_Renderer* renderer );
        ~GraphicsManager() = default;

        enum VectorizedTextureKey
        {
            LAND
        };

        enum SingularTextureKey
        {
            CLOUD_SMALL,
            CLOUD_MIDDLE,
            CLOUD_BIG,
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

        enum TextButtonKey
        {
            GAME,
            MENU
        };

        enum FontKey
        {
            MINECRAFT_18 = 18,
            MINECRAFT_24 = 24,
            MINECRAFT_36 = 36
        };

        struct TextButtonTextures
        {
            Texture marked;
            Texture unmarked;
        };

        
        class VectorizedTextures
        {
            public:
                VectorizedTextures(SDL_Renderer* renderer, const std::string& folder_path);
                const Texture& operator[](int index) const;
                std::optional<int> get_index_from_name(const std::string& texture_name) const;
            private:
                std::vector< Texture > textures_;
                std::unordered_map< std::string, int > name_to_index_translator_;
        };

        Animation copy_animation( AnimationKey key ) const;

        const Texture& get_texture( SingularTextureKey key ) const;
        const VectorizedTextures& get_vectorized_textures( VectorizedTextureKey key) const;

        const TextButtonTextures* get_text_button_textures_ptr( const std::string& text, FontKey font_key) const;
        const Texture* get_dynamic_texture_ptr(const std::string& file_path) const;

        using FoundItems = std::variant<const Texture*, const VectorizedTextures*, Animation>;

        FoundItems find_items_by_path(const std::string& path) const;

        
        
    private:
        inline static std::unordered_map<std::string, VectorizedTextureKey> vectorized_textures_as_path_key_map
        {    
            { "data/graphics/terrain/land", LAND }
        };

        inline static std::unordered_map<std::string, SingularTextureKey> singular_textures_as_path_key_map
        {
            { "data/graphics/clouds/Small Cloud 1.png", CLOUD_SMALL },
            { "data/graphics/clouds/Small Cloud 2.png", CLOUD_MIDDLE },
            { "data/graphics/clouds/Small Cloud 3.png", CLOUD_BIG },

            { "data/graphics/cursors/mouse.png",  CURSOR_MOUSE },
            { "data/graphics/cursors/handle.png", CURSOR_HANDLE },

            { "data/graphics/enemies/pearl/pearl.png",   PEARL },

            { "data/graphics/terrain/water/water_bottom.png", WATER_BOTTOM }
        };  

        inline static std::unordered_map<std::string, AnimationKey> animations_as_path_key_map
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

        std::unordered_map< AnimationKey, Animation > animations_as_map_;
        std::unordered_map< SingularTextureKey, Texture > textures_as_map_;
        std::unordered_map< VectorizedTextureKey, VectorizedTextures > vectorized_textures_as_map_;

        mutable std::unordered_map< std::pair<std::string, FontKey>, TextButtonTextures, PairHash > text_buttons_textures_as_map_;

        mutable std::unordered_map<std::string, Texture> dynamic_textures_;

        SDL_Renderer* renderer_;
        std::unordered_map<FontKey, FontManager> fonts_;

        template <typename KeyMap, typename StorageMap>
        auto get_ptr_by_path(const std::string& path, const KeyMap& key_map, const StorageMap& storage_map) const 
            -> const typename StorageMap::mapped_type*
        {
            auto it_key = key_map.find(path);
            if (it_key != key_map.end()) 
            {
                auto it_storage = storage_map.find(it_key->second);
                if (it_storage != storage_map.end()) 
                {
                    return &(it_storage->second);
                }
            }
            return nullptr;
        }

};

std::vector<std::string> get_file_names_from_folder(const std::string& folder_path);



#endif