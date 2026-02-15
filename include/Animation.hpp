
#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include <vector>
#include <memory>

#include "Texture.hpp"

class Animation
{
    public:
        Animation():
            textures_          { nullptr },
            texture_duration_  { 1       },
            animation_duration_{ 0       },
            current_frame_     { 0       },
            loop_              { false   },
            done_              { false   }
        {}

        Animation( std::shared_ptr<std::vector<Texture>> textures, int texture_duration, bool loop ):
            Animation()
        {
            textures_           = textures;
            texture_duration_   = texture_duration;
            animation_duration_ = texture_duration * textures->size();  
            loop_               = loop;      
        }   

        Animation( const Animation& other )
        {
            textures_           = other.textures_;
            texture_duration_   = other.texture_duration_;
            animation_duration_ = other.animation_duration_;
            current_frame_      = other.current_frame_; 

            loop_ = other.loop_;
            done_ = other.done_;
        }

        const Animation& operator=( const Animation& other )
        {
            textures_           = other.textures_;
            texture_duration_   = other.texture_duration_;
            animation_duration_ = other.animation_duration_;
            current_frame_      = other.current_frame_; 
            
            loop_ = other.loop_;
            done_ = other.done_;

            return *this;
        }


        
        Animation copy() const { return Animation( textures_, texture_duration_, loop_ ); }
        
        void update( float dt );
        const Texture* get_texture() const { return &textures_->at( static_cast<int>( current_frame_/texture_duration_ ) ); };
        
    private:
        std::shared_ptr<std::vector<Texture>> textures_;
        int   texture_duration_;
        int   animation_duration_;
        float current_frame_;  

        bool loop_;
        bool done_;
};

#endif