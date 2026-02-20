

#include "CanvaObject.hpp"

CanvaObject::CanvaObject( Vector2D<int> pos, int id, const GraphicsManager::FoundItems* graphics ): 
    pos_{pos},
    id_{id},
    graphics_{graphics}
{}

void CanvaObject::set_pos(Vector2D<int> new_pos)
{ pos_ = new_pos; }
Vector2D<int> CanvaObject::get_pos() const
{ return pos_; }
const Rect& CanvaObject::get_rect() const
{ return rect_; }

void CanvaObject::update()
{
    std::visit(overloaded{
        [&](std::monostate)
        {},
        [&](const GraphicsManager::VectorizedTextures* vect_textures_ptr)
        {
            auto w = TILE_SIZE;
            auto h = TILE_SIZE;            
            rect_ = Rect{pos_.x - w/2, pos_.y - h/2, w, h};
        },
        [&](const Texture* texture_ptr)
        {
            auto w = texture_ptr->get_width();
            auto h = texture_ptr->get_height();
            rect_ = Rect{pos_.x - w/2, pos_.y - h/2, w, h};
        },
        [&](const Animation& animation)
        {
            const Texture* texture_ptr = animation.get_texture();
            auto w = texture_ptr->get_width();
            auto h = texture_ptr->get_height();
            rect_ = Rect{pos_.x - w/2, pos_.y - h/2, w, h};
        }
    }, *graphics_);  
}
void CanvaObject::render() const
{
    std::visit(overloaded{ 
        [&](std::monostate)
        {},
        [&](const Texture* texture_ptr)
        {texture_ptr->render(rect_.get_pos());},
        [&](const GraphicsManager::VectorizedTextures* vec_textures_ptr)
        {},
        [&](const Animation& animation)
        {animation.get_texture()->render(rect_.get_pos());}
    }, *graphics_);
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

void CanvaObjects::add(Vector2D<int> pos, int id)
{
    const auto& editor_data_manager = context_.editor_data_manager;
    const auto& graphics = editor_data_manager.get_series(id).graphics;
    canva_objects_.emplace_back(pos, id, &graphics);
}
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

    for ( auto& canva_object : canva_objects_ )
    { canva_object.update(); }

    if (grabbed_)
    {
        if ( event_manager.left_got_unclicked() )
        { grabbed_.clear(); }
        else if ( event_manager.left_is_clicked() && event_manager.mouse_motion() )
        {
            Vector2D<int> new_pos = global_mouse_pos - grabbed_.offset;

            (*grabbed_).set_pos(new_pos);   
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
            { this->add(global_mouse_pos, context_.canva_id); }
        }
    }
}
void CanvaObjects::render()
{
    for ( const auto& canva_object : canva_objects_ )
    { canva_object.render(); }
}

CanvaObjects::iterator CanvaObjects::begin()
{return canva_objects_.begin();}
CanvaObjects::iterator CanvaObjects::end()
{return canva_objects_.end();}
CanvaObjects::const_iterator CanvaObjects::begin() const
{return canva_objects_.cbegin();}
CanvaObjects::const_iterator CanvaObjects::end() const
{return canva_objects_.cend();}
CanvaObjects::const_iterator CanvaObjects::cbegin() const
{return canva_objects_.cbegin();}
CanvaObjects::const_iterator CanvaObjects::cend() const
{return canva_objects_.cend();}