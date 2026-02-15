
#include <stdexcept>
#include "FileMenagement.hpp"
#include "GraphicsManager.hpp"

GraphicsManager::GraphicsManager( SDL_Renderer* renderer )
{
    for( const auto& [ path, key ] : textures_as_vector_paths )
    {
        auto textures = load_textures_from_folder_as_vector( path, renderer );

        std::unordered_map< std::string, int > translator;

        int i = 0;

        for( auto& file_name : get_file_names_from_folder( path ) )
        {
            std::cout << file_name << "\n";

            std::size_t dot_pos;

            dot_pos = file_name.rfind( '.' );
            if( dot_pos != std::string::npos )
            {
                file_name = file_name.substr( 0, dot_pos );
            }

            translator.emplace( file_name, i++ );
        }

        textures_as_vector.emplace( key, TexturesAsVectorType{ std::move( textures ), std::move( translator ) } );
    }

    for( const auto& [ path, key ] : textures_as_map_paths )
    {
        std::cout << path << "\n";
        textures_as_map.emplace( key, Texture{ renderer, path } );
    }

    for( const auto& [ path, key ] : animation_paths )
    {
        std::cout << path << "\n";
        auto textures = std::make_shared< std::vector<Texture> >( load_textures_from_folder_as_vector( path, renderer ) );
        animations.emplace( key, Animation{ std::move( textures ), 5, false } );

    }
}

Animation GraphicsManager::copy_animation( AnimationKey key ) const
{
    auto it = animations.find( key );

    if( it != animations.end() ) return it->second.copy();
    else
        throw std::out_of_range( "zly klucz do animations" );
}

Animation GraphicsManager::copy_animation( const std::string& path ) const
{
    auto it = animation_paths.find( path );

    if( it != animation_paths.end() ) 
        return this->copy_animation( it->second );
    else
        throw std::out_of_range( "nie mozna dopasowac sciezki do animacji -> " );
}

const Texture& GraphicsManager::get_texture( TextureKey key ) const
{
    auto it = textures_as_map.find( key );

    if( it != textures_as_map.end() ) return it->second;
    else
        throw std::out_of_range( "zly klucz do textures" );
}

const Texture& GraphicsManager::get_texture( const std::string& path ) const
{
    auto it = textures_as_map_paths.find( path );

    if( it != textures_as_map_paths.end() ) 
        return this->get_texture( it->second );
    else
        throw std::out_of_range( "nie mozna dopasowac sciezki do tekstury -> ");
}

const Texture& GraphicsManager::get_texture( TextureKey key, int index ) const
{
    auto it = textures_as_vector.find( key );
    if( it != textures_as_vector.end() )
        return it->second.textures[ index ];
    else
        throw std::out_of_range( "index podany do textures_as_vector nie pasuje" );
} 

const std::unordered_map< std::string, int >& GraphicsManager::get_translator( TextureKey key ) const
{
    auto it = textures_as_vector.find( key );
    if( it != textures_as_vector.end() )
        return it->second.translator;
    else
        throw std::out_of_range( "klucz po ktorym szukam translatora chyba nie istnieje upsi" );    
}

std::optional<int> GraphicsManager::translate( TextureKey key, const std::string& string_type_key ) const
{
    const auto& translator = this->get_translator( key );
    auto it = translator.find( string_type_key );
    if( it != translator.end() )
        return it->second;
    else
        return std::nullopt;
    
}