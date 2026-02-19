

#include "CanvaObject.hpp"

CanvaObject::CanvaObject( int id ): 
    id_{id}
{}

Rect& CanvaObject::get_rect()
{ return rect_; }

void CanvaObject::update( const Vector2D<int>& origin, const EventManager& event_manager )
{

}

void CanvaObject::render( const Vector2D<int>& origin, const GraphicsManager& graphics_manager ) const
{

}

CanvaObjects::CanvaObjects(
    EventManager& event_manager, 
    GraphicsManager& graphics_manager, 
    Vector2D<int>& origin, 
    EditorDataManager& editor_data_manager,
    int& canva_id    
) :
    context_{event_manager, graphics_manager, origin, canva_id, editor_data_manager}
{

}

void CanvaObjects::add()
{canva_objects_.emplace_back(context_.canva_id);}
void CanvaObjects::remove(std::vector<CanvaObject>::iterator it)
{ 
    if (grabbed_.active && grabbed_.obj_ptr == &(*it)) 
    { grabbed_.clear(); }
    canva_objects_.erase(it);
}
void CanvaObjects::remove(std::vector<CanvaObject>::reverse_iterator rit)
{
    auto normal_it = std::next(rit).base();
    this->remove(normal_it);
}

void CanvaObjects::update()
{   
    const auto& event_manager = context_.event_manager;
    const auto& editor_data_manager = context_.editor_data_manager;
    int canva_id = context_.canva_id;
    Vector2D<int> origin = context_.origin;
    Vector2D<int> mouse_pos = event_manager.mouse_pos();
    Vector2D<int> global_mouse_pos = mouse_pos-origin;
    
    if (grabbed_)
    {
        if ( event_manager.left_got_unclicked() )
        { grabbed_.clear(); }
        else if ( event_manager.left_is_clicked() && event_manager.mouse_motion() )
        {
            Vector2D<int> new_rect_pos = global_mouse_pos - grabbed_.offset;
            (*grabbed_).get_rect().set_pos(new_rect_pos);   
        }
    }
    else if ( editor_data_manager.is_object(canva_id) )
    { 
        if ( event_manager.left_got_clicked() || event_manager.right_got_clicked() )
        {
            bool any_got_grabbed = false;
            for ( auto it = canva_objects_.rbegin(); it != canva_objects_.rend(); it++ )
            {
                auto& canva_object = *it;
                if ( canva_object.get_rect().collide_point(global_mouse_pos) )
                {
                    if ( event_manager.left_got_clicked() )
                    {
                        grabbed_.set_new(&canva_object, global_mouse_pos );
                        any_got_grabbed = true;
                        break;
                    } 
                    else if ( event_manager.right_got_clicked() )
                    {
                        this->remove(it);
                        break;
                    }
                }
            }
            if ( event_manager.left_got_clicked() && !any_got_grabbed )
            { this->add(); }
        }
    }
}
void CanvaObjects::render()
{}

// bool CanvaObjects::verify_canva_object_type( int id )
// {
//     const auto& style = editor_context_.editor_data.at( id ).style;
//     bool success      = false;


//     switch( canva_objects_type_ )
//     {
//         case CanvaObjectType::FOREGROUND:
//             if( style == "palm_fg" ) success = true;
//             break;
//         case CanvaObjectType::BACKGROUND:
//             if( style == "palm_bg" ) success = true;
//             break;
//         case CanvaObjectType::ENTITIES:
//             if( style == "player" ) success = true;
//             break;
//         default:
//             break;
//     }

//     return success;
// }

// void CanvaObjects::add( Vector2D<int> pos, int id )
// {

//     if( this->verify_canva_object_type( id ) )
//     {
//         const auto& origin    = editor_context_.origin;
//         const auto& animation = editor_context_.editor_graphics.get_animation( id );
//         const auto& texture   = animation.get_texture();

//         auto w = texture->get_width();
//         auto h = texture->get_height();
            
//         pos -= Vector2D<int>( w/2, h/2 );


//         if( canva_objects_.find( pos ) == canva_objects_.end() )
//         {
//             canva_objects_.emplace( pos, CanvaObject{ { pos, w, h }, id } );
//         }
//     }
// } 

// void CanvaObjects::render()
// {
//     for( auto& [pos, canva_object] : canva_objects_ )
//     {
//         canva_object.render( editor_context_ );
//     } 
// }