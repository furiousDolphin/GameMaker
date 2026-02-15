
#include <cmath>
#include <utility>

#include "Animation.hpp"


void Animation::update( float dt )
{
    if( !done_ && animation_duration_ )
    {
        current_frame_ = std::fmod( current_frame_ + 30*dt, animation_duration_ );

        if( !loop_ && current_frame_ - 1 > animation_duration_ ) done_ = true;
    }
}