
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;
#include <iostream>
#include <iomanip>

#include "FileManagement.hpp"

int main()
{
    std::vector<char> sides;
    char first = 'A';
    char last = 'H' + 1;
    sides.reserve(last - first);
    for ( char c = first; c < last; c++ )
    { sides.push_back(c); }

    std::string source_folder_path = "graphics/";
    std::vector<LoadingLandData> loading_land_datas;

    for ( const auto& entry : fs::directory_iterator(source_folder_path) )
    {
        if ( entry.is_regular_file() && entry.path().extension() == ".png" )
        { 
            LoadingLandData file_data;
            file_data.path = entry.path().string();
            std::string stem_file_name = entry.path().stem().string();

            std::size_t sep = stem_file_name.find('_');
            if ( sep != std::string::npos )
            { 
                std::string str_required_sides = stem_file_name.substr(0, sep);
                for ( char c : str_required_sides )
                { file_data.required_sides.insert(c); }

                std::string str_forbidden_sides = stem_file_name.substr(sep+1);
                for ( char c : str_forbidden_sides )
                { file_data.forbidden_sides.insert(c); }
            }
            loading_land_datas.push_back(file_data);
        }
    }

    auto combinations = get_combinations<char>(sides.begin(), sides.end());
    
    for (const auto& combination : combinations) 
    {
        std::set<char> comb_set(combination.begin(), combination.end());

        for (const auto& file_data : loading_land_datas) 
        {
            bool match = true;

            for (char req : file_data.required_sides) 
            {
                if (!comb_set.contains(req)) 
                {
                    match = false;
                    break;
                }
            }

            if (!match) 
            { continue; }

            for (char forb : file_data.forbidden_sides) 
            {
                if (comb_set.contains(forb)) 
                {
                    match = false;
                    break;
                }
            }

            if (match) 
            {
                std::string new_name = "";
                auto sorted_comb = combination;
                std::sort(sorted_comb.begin(), sorted_comb.end());

                for (char c : sorted_comb) 
                { new_name += c; }
                if (new_name.empty()) 
                { new_name = "X"; }

                fs::path destination_folder = "data/graphics/terrain/land/";
                fs::path destination_file_path = destination_folder / (new_name + ".png");

                try 
                {
                    if (!fs::exists(destination_folder)) 
                    { fs::create_directories(destination_folder); }

                    fs::copy(file_data.path, destination_file_path, fs::copy_options::overwrite_existing);
                    break; 
                } 
                catch (const fs::filesystem_error& e) 
                { std::cerr << new_name << " -> " << e.what() << std::endl; }
            }
        }
    }
}
