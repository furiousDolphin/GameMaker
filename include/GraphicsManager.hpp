#ifndef GRAPHICS_MANAGER_HPP_
#define GRAPHICS_MANAGER_HPP_

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>
#include <set>



#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Texture.hpp"
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
            LAND,
        };

        enum SingularTextureKey
        {

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

        const Texture& get_texture( SingularTextureKey key ) const;
        const Texture& get_texture( VectorizedTextureKey key, int index ) const;

        struct TextButtonTextures;
        const TextButtonTextures& get_texture( const std::string& text, FontKey font_key);

        struct TextButtonTextures
        {
            Texture marked;
            Texture unmarked;
        };
        

    private:
        inline static std::unordered_map<std::string, VectorizedTextureKey> vectorized_textures_as_path_key_map
        {    
            { "data/graphics/terrain/land", LAND }
        };

        inline static std::unordered_map<std::string, SingularTextureKey> singular_textures_as_path_key_map
        {

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

        std::unordered_map< SingularTextureKey, Texture > textures_as_map_;
        std::unordered_map< VectorizedTextureKey, VectorizedTextures > vectorized_textures_as_map_;

        std::unordered_map< std::pair<std::string, FontKey>, TextButtonTextures, PairHash > text_buttons_textures_as_map_;

        SDL_Renderer* renderer_;
        std::unordered_map<FontKey, FontManager> fonts_;

};

std::vector<std::string> get_file_names_from_folder(const std::string& folder_path);



#endif