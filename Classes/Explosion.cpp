//
//  Explosion.cpp
//  Santra
//
//  Created by Anıl Gülgör on 27/03/16.
//
//

#include "Explosion.hpp"

Explosion::Explosion(cocos2d::Vec2 position){
    
    _position = position;
    
}


Explosion *Explosion::create(cocos2d::Vec2 position){
    
    Explosion *explosion = new Explosion(position);
    
    return explosion;
    
    
}

void Explosion::setUp(){
    
    this->schedule(schedule_selector(Explosion::update));
    
    animSpeed = 8;
    
    animCount = animSpeed - 1;

    frameCount = -1;
    
    expBody = Sprite::create("patlatefekt/p1.png");
    
    FIT_H(expBody, GET_HEIGHT*.8);
    
    this->addChild(expBody);
    
    expBody->setZOrder(15);
    
    this->startAnimation();
    
}

void Explosion::update(float dt){
    
    if(animStart == true){
        
        animCount ++;
        
        if(animCount == animSpeed){
            
            animCount = 0;
            frameCount ++;
            
            
            if(frameCount <= 13)
            {
                
                expBody->setTexture(StringUtils::format("patlatefekt/p%d.png",int(frameCount + 1)));
                
            }
            else {
                
                animStart = false;
                
                this->removeFromParent();
                
                this->unschedule(schedule_selector(Explosion::update));
                
            }
            
        }

        
    }
    
}

void Explosion::startAnimation(){
    
    animStart = true;
    
}
