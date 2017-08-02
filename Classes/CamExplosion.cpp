//
//  CamExplosion.cpp
//  Santra
//
//  Created by Anıl Gülgör on 26/07/16.
//
//

#include "CamExplosion.hpp"


CamExplosion::CamExplosion(cocos2d::Vec2 position){
    
    _position = position;
    
}


CamExplosion *CamExplosion::create(cocos2d::Vec2 position){
    
    CamExplosion *camExplosion = new CamExplosion(position);
    
    return camExplosion;
    
    
}

void CamExplosion::setUp(){
    
    this->schedule(schedule_selector(CamExplosion::update));
    
    animSpeed = 8;
    
    animCount = animSpeed - 1;
    
    frameCount = -1;
    
    expBody = Sprite::create("campatla/Cam_00000.png");
    
    FIT_H(expBody, GET_HEIGHT);
    
    this->addChild(expBody);
    
    //expBody->setZOrder(15);
    
    //this->startAnimation();
    
}

void CamExplosion::update(float dt){
    
    if(animStart == true){
        
        animCount ++;
        
        if(animCount == animSpeed){
            
            animCount = 0;
            frameCount ++;
            
            
            if(frameCount <= 49)
            {
             
                if(frameCount + 1 < 10){
                
                    expBody->setTexture(StringUtils::format("campatla/Cam_0000%d.png",int(frameCount + 1)));
                    
                }
                else{
                    
                    expBody->setTexture(StringUtils::format("campatla/Cam_000%d.png", int(frameCount + 1)));
                    
                }
                
            }
            else {
                
                animStart = false;
                
                this->removeFromParent();
                
                this->unschedule(schedule_selector(CamExplosion::update));
                
            }
            
        }
        
        
    }
    
}

void CamExplosion::startAnimation(){
    
    animStart = true;
    
}
