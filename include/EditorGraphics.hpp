

#include "Settings.hpp"

#include "GraphicsManager.hpp"


class EditorGraphics
{
    public:
        EditorGraphics( const AppContext&     app_context,
                        const EditorDataType& editor_data );

        const Animation&                       get_animation( int id ) const;
        const std::pair< CanvaType, Texture >& get_preview(   int id ) const;

        CanvaType get_preview_canva_type( int id ) const;
        
        void update_animations( float dt );

        const Texture& get_texture( GraphicsManager::TextureKey key ) const
        {
            return graphics_manager_.get_texture( key );
        }
        
        const Texture& get_texture( GraphicsManager::TextureKey key, int index ) const 
        { 
            return graphics_manager_.get_texture( key, index); 
        }

        std::optional<int> get_texture_index( GraphicsManager::TextureKey key, const std::string& string_type_key ) const
        {
            return graphics_manager_.translate( key, string_type_key );
        }

        const MenuTexturesType&     get_menu_textures() const { return menu_textures_;    }
        const EditorAnimationsType& get_animations()    const { return animations_;       }
              EditorAnimationsType& get_animations()          { return animations_;       }


    private:
        PreviewTexturesType    previews_;
        MenuTexturesType       menu_textures_;
        EditorAnimationsType   animations_;
        const GraphicsManager& graphics_manager_;
};