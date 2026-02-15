#include <iostream>
#include <fstream>
#include <sstream>

#include <filesystem>
#include <cmath>
#include <algorithm>  
#include <cctype>      

namespace fs = std::filesystem;

#include "FileMenagement.hpp"

std::vector<std::vector< double >> load_csv( const std::string& path )
{
    std::ifstream file{ path };

    if( !file.is_open() )
        throw std::invalid_argument( "podano zla sciezke do pliku csv ->" + path );

    std::string line;
    std::getline( file, line );

    std::vector<std::vector< double >> destination;

    while( std::getline( file, line ) )
    {
        std::istringstream iss{ line };
        std::string token;
        int col = 0;

        while( std::getline( iss, token, ',' ) )
        {
            if( destination.size() <= col )
                destination.push_back( {} );
            
            if( !token.empty() )
                destination[col].push_back( std::stod(token ) );
            else    
                destination[col].push_back( 0.0 );
            col++;
        }
    }
    return destination;
}

void save_csv( const std::string& path, const std::vector<std::vector< double >>& data )
{
    std::ofstream file{ path };
    if( !file.is_open() )
        throw std::invalid_argument( "nie udalo sie otworzyc pliku dla sciezki -> " + path );

    int n_rows = 0;
    int n_cols = data.size();


    for( int col = 0; col < n_cols; col++ )
    {
        file << "col_" << ( col+1 ) << ( ( col == n_cols - 1 ) ? "\n" : "," );
        n_rows = std::max( static_cast<int>( data[col].size() ), n_rows );
    }



    for( int row = 0; row < n_rows; row++ )
    {
        for( int col = 0; col < n_cols; col++ )
        {
            file << data[col][row] << ( ( col == n_cols - 1 ) ? "\n" : "," );
        }  
    }
}

std::vector< std::string > get_file_names_from_folder( const std::string& folder_path )
{

    std::vector<std::string> result;

    try
    {
        {
            for( const auto& entry : fs::directory_iterator( folder_path ) )
            {
                if( entry.is_regular_file() && entry.path().extension() == ".png" )
                {
                    result.push_back( entry.path().filename().string() );
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::sort(result.begin(), result.end(), [](const std::string& a, const std::string& b) 
                                            {
                                                return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),
                                                [](char c1, char c2)
                                                { 
                                                    return std::tolower(c1) < std::tolower(c2); 
                                                });
                                            } );

    return result;
}

std::vector< Texture > load_textures_from_folder_as_vector( const std::string& folder_path, SDL_Renderer* renderer )
{
    std::vector< Texture > destination;

    const auto& texture_names = get_file_names_from_folder( folder_path );

    for( const auto& texture_name : texture_names )
    {
        destination.push_back( Texture{ renderer, folder_path + "/" + texture_name } );
    }
    return destination;
}

std::unordered_map<std::string, Texture > load_textures_from_folder_as_map( const std::string& folder_path,  SDL_Renderer* renderer )
{
    std::unordered_map<std::string, Texture > destination;

    const auto& texture_names = get_file_names_from_folder( folder_path );

    for( const auto& texture_name : texture_names )
    {
        std::size_t dot_pos = texture_name.rfind( "." );

        if( dot_pos != std::string::npos )
        {
            destination.emplace( texture_name.substr( 0, dot_pos ), Texture{ renderer, folder_path + "/" + texture_name } );
        }
    }
    return destination;
}

std::unordered_map<int, std::vector< std::string > > load_json( const std::string& path )
{
    std::unordered_map<int, std::vector< std::string > > destination;
    return destination;
}