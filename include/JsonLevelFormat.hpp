

#ifndef JSON_LEVEL_FORMAT_HPP_
#define JSON_LEVEL_FORMAT_HPP_

#include "Settings.hpp"

class JsonLevelFormat
{
    public:
        JsonLevelFormat(std::string file_path);
        enum Key
        {
            TERRAIN,
            ENTITIES,
            PLAYER,
            WATER
        };

        using DataFormat = std::unordered_map<Key, std::unordered_map<Vector2D<int>, std::variant<Vector2D<int>, int>>>;

        void add_to_export(Key key, Vector2D<int> pos, std::variant<Vector2D<int>, int> val);
        void export_to_json();
        void import_from_json();
        const DataFormat& get_import_data() const;
    private:
        static inline std::unordered_map<std::string, Key> string_key_map_
        {
            {"terrain", TERRAIN},
            {"entities", ENTITIES},
            {"player", PLAYER},
            {"water", WATER}

        };
        std::unordered_map<Key, std::string> key_string_map_;

        DataFormat export_data_;
        DataFormat import_data_;
        std::string file_path_;

};

#endif