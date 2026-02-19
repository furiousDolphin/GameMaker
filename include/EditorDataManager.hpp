

#ifndef EDITOR_DATA_MANAGER_HPP_
#define EDITOR_DATA_MANAGER_HPP_

#include "GraphicsManager.hpp"

class EditorDataManager
{
    public:
        EditorDataManager(GraphicsManager& graphics_manager);

        struct DataSeries
        {
            std::string style;
            std::string type;
            std::string menu;

            const Texture* menu_texture;
            const Texture* preview_texture;

            GraphicsManager::FoundItems graphics;
        };

        bool is_object(int id) const;
        bool is_tile(int id) const;
        const DataSeries& get_series(int id) const;
        void update( float dt);
        

        using iterator = std::unordered_map<int, DataSeries>::iterator;
        using const_iterator = std::unordered_map<int, DataSeries>::const_iterator;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

        
    private:
        void create_data();

        struct Context
        {
            GraphicsManager& graphics_manager;
        } context_;


        std::unordered_map<int, DataSeries> data_;
        std::set<int> object_ids_;
        std::set<int> tile_ids_;

        static inline std::string data_path_ = "editor_data.json";

};

#endif