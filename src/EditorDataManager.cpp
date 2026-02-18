
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <filesystem>
namespace fs = std::filesystem;

#include "EditorDataManager.hpp"

EditorDataManager::EditorDataManager(GraphicsManager& graphics_manager) :
    context_{graphics_manager}
{
    this->create_data();
}

const EditorDataManager::DataSeries& EditorDataManager::get_series(int id) const
{ return data_.at(id); }

void EditorDataManager::create_data()
{
    char* base_path = SDL_GetBasePath();
    if (!base_path) 
        throw std::invalid_argument( "nie wyszlo z base_path" ); 


    fs::path exe_path(base_path);   
    SDL_free(base_path);     
    fs::path project_root = exe_path.parent_path().parent_path(); 
    fs::path file_path = project_root / data_path_;


    std::ifstream file{ file_path };
    if (!file.is_open())
        throw std::invalid_argument("nie moge otworzyc sciezki -> " + data_path_);

    nlohmann::json j;
    file >> j;

    
    auto safe_get_string = [](
        const nlohmann::json& obj,
        const std::string& key,
        const std::string& def) -> std::string
    {
        if (obj.contains(key) && !obj[key].is_null())
        { return obj[key].get<std::string>(); }
        return def;
    };

    for (const auto& [key, val] : j.items())
    {
        
        if (!std::all_of(key.begin(), key.end(), ::isdigit))
            continue; 

        int id = std::stoi(key);
        DataSeries data_series 
        {
            safe_get_string(val, "style", ""),
            safe_get_string(val, "type", ""),
            safe_get_string(val, "menu", ""),
            context_.graphics_manager.get_dynamic_texture_ptr(safe_get_string(val, "menu_surf", "")),
            context_.graphics_manager.get_dynamic_texture_ptr(safe_get_string(val, "preview", "")),
            context_.graphics_manager.find_items_by_path(safe_get_string(val, "graphics", ""))
        };

        data_.emplace(id, std::move(data_series));
    }
}

EditorDataManager::iterator EditorDataManager::begin()
{ return data_.begin(); }
EditorDataManager::iterator EditorDataManager::end()
{ return data_.end(); }
EditorDataManager::const_iterator EditorDataManager::begin() const
{ return data_.cbegin(); }
EditorDataManager::const_iterator EditorDataManager::end() const
{ return data_.cend(); }
EditorDataManager::const_iterator EditorDataManager::cbegin() const
{ return data_.cbegin(); }
EditorDataManager::const_iterator EditorDataManager::cend() const
{ return data_.cend(); }