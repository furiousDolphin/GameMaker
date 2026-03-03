

#include "WaterCanvaObject.hpp"



WaterCanvaObject::SlideRect::SlideRect(
    Vector2D<int> center, 
    Vector2D<int> shape, 
    SlideRect::Axis axis, 
    std::function<Vector2D<int>(void)> getter,  
    std::function<void(int)> setter
) :
    Rect{center.x-shape.x/2, center.y-shape.y/2, shape.x, shape.y}, 
    axis_{axis},
    getter_{getter},
    setter_{setter}
{

}

Vector2D<int> WaterCanvaObject::SlideRect::get_pos() const
{
    auto [x, y] = getter_();
    return {x - rect_.w/2, y-rect_.h/2};

}

void WaterCanvaObject::SlideRect::set_pos(const Vector2D<int>& new_top_left)
{
    auto [x, y] = new_top_left;
    switch ( axis_ )
    {
        case Axis::HORIZONTAL:
            setter_(x+rect_.w/2);
            break;
        case Axis::VERTICAL:
            setter_(y+rect_.h/2);
            break;
    }
}

bool WaterCanvaObject::SlideRect::collide_point(const Vector2D<int>& v) const
{
    auto [x, y] = this->get_pos();
    return !( 
        x >= v.x || 
        v.x >= x + rect_.w || 
        y >= v.y || 
        v.y >= y + rect_.h );
}

WaterCanvaObject::WaterCanvaObject(Vector2D<int> top_left, Vector2D<int> grid_shape) :
    main_r_{top_left, grid_shape.x*TILE_SIZE, grid_shape.y*TILE_SIZE},
    boundary_r_{main_r_.get_pos()-main_r_.get_shape()/8, main_r_.get_w()+main_r_.get_w()/4, main_r_.get_h()+main_r_.get_h()/4}
{
    auto snap = [](int val) { return (val / TILE_SIZE) * TILE_SIZE; };
    //auto snap = [&](int val) { return static_cast<int>(val / TILE_SIZE) - (val < 0 ? 1 : 0); };

    //lewy
    slide_rects_.push_back(std::make_unique<SlideRect>(
        Vector2D<int>{main_r_.get_left(), main_r_.get_centery()}, 
        Vector2D<int>{1, 1}*TILE_SIZE/4, 
        SlideRect::Axis::HORIZONTAL, 
        [this]()
        {return Vector2D<int>{main_r_.get_left(), main_r_.get_centery()};}, 
        [this, snap](int arg)
        {
            int new_arg = std::min(arg, main_r_.get_right()-TILE_SIZE);
            main_r_.set_w(main_r_.get_right() - new_arg);
            main_r_.set_left(new_arg);
        }));

    //prawy
    slide_rects_.push_back(std::make_unique<SlideRect>(
        Vector2D<int>{main_r_.get_right(), main_r_.get_centery()}, 
        Vector2D<int>{1, 1}*TILE_SIZE/4, 
        SlideRect::Axis::HORIZONTAL, 
        [this]()
        {return Vector2D<int>{main_r_.get_right(), main_r_.get_centery()};}, 
        [this, snap](int arg)
        {
            int new_arg = std::max(arg, main_r_.get_left()+TILE_SIZE);
            main_r_.set_w(new_arg - main_r_.get_left());
            main_r_.set_right(new_arg);
        }));

    //gora
    slide_rects_.push_back(std::make_unique<SlideRect>(
        Vector2D<int>{main_r_.get_centerx(), main_r_.get_top()}, 
        Vector2D<int>{1, 1}*TILE_SIZE/4, 
        SlideRect::Axis::VERTICAL, 
        [this]()
        {return Vector2D<int>{main_r_.get_centerx(), main_r_.get_top()};}, 
        [this, snap](int arg)
        {
            int new_arg = std::min(arg, main_r_.get_bottom()-TILE_SIZE);
            main_r_.set_h(main_r_.get_bottom() - new_arg);
            main_r_.set_top(new_arg);
        })); 

    //dol
    slide_rects_.push_back(std::make_unique<SlideRect>(
        Vector2D<int>{main_r_.get_centerx(), main_r_.get_bottom()}, 
        Vector2D<int>{1, 1}*TILE_SIZE/4, 
        SlideRect::Axis::VERTICAL, 
        [this]()
        {return Vector2D<int>{main_r_.get_centerx(), main_r_.get_bottom()};}, 
        [this, snap](int arg)
        {
            int new_arg = std::max(arg, main_r_.get_top()+TILE_SIZE);
            main_r_.set_h(new_arg - main_r_.get_top());
            main_r_.set_bottom(new_arg);
        })); 
}

Vector2D<int> WaterCanvaObject::get_pos() const
{ return main_r_.get_pos(); }
Vector2D<int> WaterCanvaObject::get_shape() const
{ return {main_r_.get_w(), main_r_.get_h()}; }

Rect* WaterCanvaObject::get_colliding_rect_ptr(Vector2D<int> p)
{
    if ( boundary_r_.collide_point(p) )
    {
        for ( auto& slide_rect_ptr : slide_rects_ )
        {
            if ( slide_rect_ptr->collide_point(p) )
            { return slide_rect_ptr.get(); }
        }

        if ( main_r_.collide_point(p) )
        { return &main_r_; }
    }  
    return nullptr;
}

void WaterCanvaObject::update()
{ 
    if ( main_r_.get_w()%TILE_SIZE == 0 && main_r_.get_h()%TILE_SIZE == 0 )
    { main_r_.set_pos(main_r_.get_pos().to_grid(TILE_SIZE)*TILE_SIZE); }
    else
    {
        auto snap_floor = [](int val) 
        { return static_cast<int>(std::floor(static_cast<float>(val) / TILE_SIZE)) * TILE_SIZE; };

        auto snap_ceil = [](int val) 
        { return static_cast<int>(std::ceil(static_cast<float>(val) / TILE_SIZE)) * TILE_SIZE; };


        int dirty_left   = main_r_.get_left();
        int dirty_top    = main_r_.get_top();
        int dirty_right  = main_r_.get_right();
        int dirty_bottom = main_r_.get_bottom();

        int clean_left   = snap_ceil(dirty_left);
        int clean_top    = snap_ceil(dirty_top);
        int clean_right  = snap_floor(dirty_right);
        int clean_bottom = snap_floor(dirty_bottom);

        if (clean_right <= clean_left) 
        { clean_right = clean_left + TILE_SIZE; }

        if (clean_bottom <= clean_top) 
        { clean_bottom = clean_top + TILE_SIZE; }

        main_r_.set_x(clean_left);
        main_r_.set_y(clean_top);
        main_r_.set_w(clean_right - clean_left);
        main_r_.set_h(clean_bottom - clean_top);
    }

    

    boundary_r_.set_pos(main_r_.get_pos() - main_r_.get_shape() / 8); 

    Vector2D<int> new_shape = {
        main_r_.get_w() + main_r_.get_w() / 4,
        main_r_.get_h() + main_r_.get_h() / 4
    };
    boundary_r_.set_w(new_shape.x);
    boundary_r_.set_h(new_shape.y); 
}
void WaterCanvaObject::render(SDL_Renderer* renderer, Vector2D<int> origin) const
{
    SDL_Rect rect{ 
        main_r_.get_x() + origin.x, 
        main_r_.get_y() + origin.y, 
        main_r_.get_w(), 
        main_r_.get_h() };

    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
    for ( const auto& slide_rect_ptr : slide_rects_  )
    { 
        auto [x, y] = slide_rect_ptr->get_pos();
        SDL_Rect rect{ 
            x + origin.x, 
            y + origin.y, 
            slide_rect_ptr->get_w(), 
            slide_rect_ptr->get_h() };

        SDL_RenderFillRect(renderer, &rect); 
    }
}

WaterCanvaObjects::WaterCanvaObjects(
    SDL_Renderer* renderer,
    EventManager& event_manager, 
    Vector2D<int>& origin, 
    EditorDataManager& editor_data_manager,
    int& canva_id    
) :
    context_{renderer, event_manager, origin, canva_id, editor_data_manager}
{

}

void WaterCanvaObjects::add(Vector2D<int> pos, Vector2D<int> grid_shape)
{
    const auto& series = context_.editor_data_manager.get_series(context_.canva_id);
    const auto& style = series.style;
    if ( style == "water" )
    { water_canva_objects_.emplace_back(pos.to_grid(TILE_SIZE)*TILE_SIZE, grid_shape); }
}
void WaterCanvaObjects::remove(std::list<WaterCanvaObject>::iterator it)
{ water_canva_objects_.erase(it); }
void WaterCanvaObjects::remove(std::list<WaterCanvaObject>::reverse_iterator rit)
{
    auto normal_it = std::next(rit).base();
    this->remove(normal_it);
}

void WaterCanvaObjects::export_data(JsonLevelFormat& json_level_format_data) const
{
    for (const auto& water_canva_object : water_canva_objects_)
    { 
        json_level_format_data.add_to_export(
            JsonLevelFormat::WATER, 
            water_canva_object.get_pos(), 
            water_canva_object.get_shape().to_grid(TILE_SIZE)); 
    }    
}

void WaterCanvaObjects::import_data(const JsonLevelFormat& json_level_format_data)
{
    this->clear();
    const auto& data = json_level_format_data.get_import_data();

    auto it = data.find(JsonLevelFormat::WATER);
    if ( it != data.end() )
    {
        for ( const auto& [vec2_pos, val] : it->second )
        {
            if ( const auto* grid_shape_ptr = std::get_if<Vector2D<int>>(&val) )
            { water_canva_objects_.emplace_back(vec2_pos, *grid_shape_ptr); } 
        }
    }
}

void WaterCanvaObjects::clear()
{
    grabbed_.clear();
    water_canva_objects_.clear();
}

void WaterCanvaObjects::update()
{   
    const auto& event_manager = context_.event_manager;
    const auto& editor_data_manager = context_.editor_data_manager;
    int canva_id = context_.canva_id;
    Vector2D<int> origin = context_.origin;
    Vector2D<int> mouse_pos = event_manager.mouse_pos();
    Vector2D<int> global_mouse_pos = mouse_pos-origin;

    // for ( auto& water_canva_object : water_canva_objects_ )
    // { water_canva_object.update(); }

    if (grabbed_)
    {
        if ( event_manager.left_got_unclicked() )
        { 
            (*grabbed_).update();
            grabbed_.clear();
        }
        else if ( event_manager.left_is_clicked() && event_manager.mouse_motion() )
        { grabbed_.update(global_mouse_pos); }
    }
    else if ( editor_data_manager.is_water_object(canva_id) )
    { 
        if ( event_manager.left_got_clicked() || event_manager.right_got_clicked() )
        {
            bool any_got_grabbed = false;
            for ( auto it = water_canva_objects_.rbegin(); it != water_canva_objects_.rend(); it++ )
            {
                auto& water_canva_object = *it;
                Rect* grabbed_rect_ptr = water_canva_object.get_colliding_rect_ptr(global_mouse_pos);
                if ( grabbed_rect_ptr )
                {
                    if ( event_manager.left_got_clicked() )
                    {
                        grabbed_.set_new(&water_canva_object, grabbed_rect_ptr, global_mouse_pos );
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
            { this->add(global_mouse_pos); }
        }
    }
}
void WaterCanvaObjects::render()
{
    for ( const auto& water_canva_object : water_canva_objects_ )
    { water_canva_object.render(context_.renderer, context_.origin); }
}

WaterCanvaObjects::iterator WaterCanvaObjects::begin()
{return water_canva_objects_.begin();}
WaterCanvaObjects::iterator WaterCanvaObjects::end()
{return water_canva_objects_.end();}
WaterCanvaObjects::const_iterator WaterCanvaObjects::begin() const
{return water_canva_objects_.cbegin();}
WaterCanvaObjects::const_iterator WaterCanvaObjects::end() const
{return water_canva_objects_.cend();}
WaterCanvaObjects::const_iterator WaterCanvaObjects::cbegin() const
{return water_canva_objects_.cbegin();}
WaterCanvaObjects::const_iterator WaterCanvaObjects::cend() const
{return water_canva_objects_.cend();}


WaterCanvaObjects::GrabbedState::GrabbedState() :
    active_{false},
    obj_ptr_{nullptr},
    rect_ptr_{nullptr}      
{}          

WaterCanvaObject& WaterCanvaObjects::GrabbedState::operator*() const 
{ return *obj_ptr_; }
WaterCanvaObjects::GrabbedState::operator bool() const 
{ return active_; }
void WaterCanvaObjects::GrabbedState::clear() 
{
    active_ = false;
    obj_ptr_ = nullptr;
    rect_ptr_ = nullptr;
}
void WaterCanvaObjects::GrabbedState::set_new(WaterCanvaObject* new_obj_ptr, Rect* new_rect_ptr, Vector2D<int> global_mouse_pos) 
{   
    obj_ptr_ = new_obj_ptr; 
    rect_ptr_ = new_rect_ptr;
    active_ = true;
    offset_ = global_mouse_pos - new_rect_ptr->get_pos();
}
void WaterCanvaObjects::GrabbedState::update(Vector2D<int> global_mouse_pos)
{   
    if (active_)
    { 
        rect_ptr_->set_pos(global_mouse_pos-offset_);
    }
}
