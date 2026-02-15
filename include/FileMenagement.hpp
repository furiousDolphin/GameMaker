
#ifndef FILEMANAGEMENT_HPP_
#define FILEMANAGEMENT_HPP_

#include <vector>
#include <string>

#include <unordered_map>
#include "Texture.hpp"
#include "Animation.hpp"


std::vector<std::vector< double >> load_csv( const std::string& path );
void                               save_csv( const std::string& path );

std::vector< std::string > get_file_names_from_folder( const std::string& folder_path );

std::vector<Texture>                      load_textures_from_folder_as_vector( const std::string& folder_path,  SDL_Renderer* renderer );
std::unordered_map<std::string, Texture > load_textures_from_folder_as_map(    const std::string& folder_path,  SDL_Renderer* renderer );

std::unordered_map<int, std::vector< std::string > > load_json( const std::string& path );


template < typename T >
void load_animations_as_enum_key_map( const std::unordered_map<T, std::string >& paths, SDL_Renderer* renderer, std::unordered_map<T, Animation >& destination )
{
    destination.clear();

    for( const auto& [ key, path ] : paths )
    {
        destination.emplace( key, Animation{ std::make_shared<std::vector<Texture>>( std::move( load_textures_from_folder_as_vector( path, renderer ) ) ), 5, false } );
    }
}

template < typename T >
void load_textures_as_enum_key_map( const std::unordered_map<T, std::string >& paths, SDL_Renderer* renderer, std::unordered_map<T, Texture >& destination )
{
    destination.clear();

    for( const auto& [ key, path ] : paths )
    {
        destination.emplace( key, Texture{ renderer, path } );
    }
}

#endif

