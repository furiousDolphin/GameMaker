
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <iomanip>

#include <filesystem>
namespace fs = std::filesystem;
#include "JsonLevelFormat.hpp"

JsonLevelFormat::JsonLevelFormat(std::string file_path) : 
    file_path_{file_path}
{
    for ( const auto& [str_key, enum_key] : string_key_map_ )
    { key_string_map_[enum_key] = str_key; }
}

void JsonLevelFormat::export_to_json()
{
    auto vec2_to_str = [](Vector2D<int> vec2)
    {return std::format("{};{}", vec2.x, vec2.y);};

    using json = nlohmann::json;
    json j;

    for ( const auto& [enum_series_key, tiles] : export_data_)
    {
        if ( key_string_map_.contains(enum_series_key) )
        {
            std::string str_series_key = key_string_map_.at(enum_series_key);
            for ( const auto& [vec2_pos, val] : tiles )
            {
                std::string str_pos = vec2_to_str(vec2_pos);
                std::visit( overloaded{
                    [&](int int_val)
                    {j[str_series_key][str_pos] = int_val;},
                    [&](Vector2D<int> vec2_val)
                    {j[str_series_key][str_pos] = vec2_to_str(vec2_val);}
                },val);  
            }
        }
    }

    std::ofstream file{file_path_};
    if (file.is_open())
    {file<<j.dump(4);}
}

void JsonLevelFormat::import_from_json()
{
    import_data_.clear();
    auto str_to_vec2 = [](const std::string& str_vec)
    {
        std::size_t sep = str_vec.find(';');
        if ( sep == std::string::npos )
        { return Vector2D<int>{0, 0}; }

        try
        {
            int x = std::stoi(str_vec.substr(0, sep));
            int y = std::stoi(str_vec.substr(sep+1));
            return Vector2D<int>{x, y};
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return Vector2D<int>{0, 0};
        }
    };

    std::ifstream file{file_path_};
    if ( !file.is_open() )
    { throw std::runtime_error("nie udalo sie otworzyc map.json"); }

    using json = nlohmann::json;
    json j;

    file >> j;

    for ( auto& [str_series_key, tiles] : j.items() )
    {
        auto enum_series_key = string_key_map_.at(str_series_key);
        for ( auto& [str_pos, val] : tiles.items() )
        {
            std::cout << std::setw(7) << str_pos << " <--> " << std::setw(3) << val << "\n";

            Vector2D<int> vec2_pos = str_to_vec2(str_pos);
            if ( val.is_number_integer() )
            { import_data_[enum_series_key][vec2_pos] = val.get<int>(); }
            else if ( val.is_string() )
            { import_data_[enum_series_key][vec2_pos] = str_to_vec2(val.get<std::string>()); }
        }
    }
}

void JsonLevelFormat::add_to_export(JsonLevelFormat::Key key, Vector2D<int> pos, std::variant<Vector2D<int>, int> val)
{ export_data_[key][pos] = val; }

const JsonLevelFormat::DataFormat& JsonLevelFormat::get_import_data() const
{ return import_data_; }