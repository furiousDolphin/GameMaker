

#include "Settings.hpp"
#include "EventManager.hpp"
#include "EditorGraphics.hpp"
#include "CanvaObject.hpp"



void CanvaObject::update( const EditorContext& editor_context )
{
    const auto& event_manager = editor_context.event_manager;

    if( event_manager.left_got_clicked() ) 
    {
        const auto mouse_pos = event_manager.mouse_pos();
        const auto origin    = editor_context.origin;

        if( rect_.collide_point( mouse_pos - origin ) )
        {
            grab_pos_   = mouse_pos - origin - rect_.get_pos(); 
            is_grabbed_ = true;
        }
    }

    else if( is_grabbed_ )
        if( event_manager.left_is_clicked() ) 
        {
            const auto mouse_pos = event_manager.mouse_pos();
            const auto origin    = editor_context.origin;

            rect_.set_pos( mouse_pos - origin - grab_pos_ ); 
            
        }
        else if( event_manager.left_got_unclicked() ) 
        {
            is_grabbed_ = false;
        }
}

void CanvaObject::render( const EditorContext& editor_context )
{
    const auto& origin    = editor_context.origin;
    const auto& animation = editor_context.editor_graphics.get_animation( id_ );
    const auto& texture   = animation.get_texture();

    texture->render( rect_.get_pos() + origin );
}

void CanvaObjects::update()
{
    const auto& event_manager = editor_context_.event_manager;
    const auto& origin        = editor_context_.origin;
    const auto& mouse_pos     = event_manager.mouse_pos();
    bool        any_grabbed   = false;



    if( event_manager.left_got_clicked()   || event_manager.right_got_clicked() ||
        event_manager.left_got_unclicked() || event_manager.left_is_clicked()      )
    {
        const auto&   event_manager = editor_context_.event_manager;
        const auto&   origin        = editor_context_.origin;
        const auto    id            = editor_context_.canva_id;
        const auto&   mouse_pos     = event_manager.mouse_pos();
        bool          any_grabbed   = false;
        bool          remove_need   = false;

        Vector2D<int> key_to_remove;



        for( auto& [pos, canva_object] : canva_objects_ )
        {
            if( canva_object.get_rect().collide_point( mouse_pos - origin ) )
            {
                canva_object.update( editor_context_ );

                if( !canva_object.is_grabbed() && event_manager.right_got_clicked() )
                {
                    key_to_remove = pos;
                    remove_need   = true;
                }

                if( !any_grabbed && canva_object.is_grabbed() ) { any_grabbed = true; break; }
            }
        }

        if( remove_need ) 
            canva_objects_.erase( key_to_remove );


        if( !any_grabbed )
        {
            if(event_manager.left_got_clicked() )
            {
                this->add( mouse_pos - origin, id );
            } 
        }
    }
}

bool CanvaObjects::verify_canva_object_type( int id )
{
    const auto& style = editor_context_.editor_data.at( id ).style;
    bool success      = false;


    switch( canva_objects_type_ )
    {
        case CanvaObjectType::FOREGROUND:
            if( style == "palm_fg" ) success = true;
            break;
        case CanvaObjectType::BACKGROUND:
            if( style == "palm_bg" ) success = true;
            break;
        case CanvaObjectType::ENTITIES:
            if( style == "player" ) success = true;
            break;
        default:
            break;
    }

    return success;
}

void CanvaObjects::add( Vector2D<int> pos, int id )
{

    if( this->verify_canva_object_type( id ) )
    {
        const auto& origin    = editor_context_.origin;
        const auto& animation = editor_context_.editor_graphics.get_animation( id );
        const auto& texture   = animation.get_texture();

        auto w = texture->get_width();
        auto h = texture->get_height();
            
        pos -= Vector2D<int>( w/2, h/2 );


        if( canva_objects_.find( pos ) == canva_objects_.end() )
        {
            canva_objects_.emplace( pos, CanvaObject{ { pos, w, h }, id } );
        }
    }
} 

void CanvaObjects::render()
{
    for( auto& [pos, canva_object] : canva_objects_ )
    {
        canva_object.render( editor_context_ );
    } 
}