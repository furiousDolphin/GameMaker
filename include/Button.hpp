
#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include <memory>
#include <functional>
#include <string>
#include <vector>

#include "Texture.hpp"
#include "Vector2D.hpp"
#include "Rect.hpp"
#include "Settings.hpp"
#include "EventManager.hpp"
#include "GraphicsManager.hpp"
#include "SDL_Management.hpp"

class Button
{
    public:
        Button( Vector2D<int> pos, std::function< void(void) > func );

        virtual ~Button() = default;

        virtual bool update( const EventManager& event_manager ) = 0;
        virtual void render() const = 0;
        
        Vector2D<int> get_pos();
        void set_pos( Vector2D<int> pos );

    protected:
        std::function< void(void) > func_;
        Rect rect_;

};


class TextButton : public Button
{
    public:
        TextButton( 
            Vector2D<int> pos, 
            std::function< void(void) > func,
            const GraphicsManager::TextButtonTextures& textures
        );

        bool update( const EventManager& event_manager ) override;
        void render() const override;

    private:
       const GraphicsManager::TextButtonTextures& textures_;
       bool is_marked_;

};

struct ButtonsContext
{
    EventManager& event_manager;
    GraphicsManager& graphics_manager;
};

class Buttons
{
    public:
        Buttons(EventManager& event_manager, GraphicsManager& graphics_manager);
        bool update();
        void render() const;
        void add( std::unique_ptr<Button> button );
    private:
        std::vector<std::unique_ptr<Button>> buttons_;

        struct Context
        {
            EventManager& event_manager;
            GraphicsManager& graphics_manager;
        } context_;
};

#endif