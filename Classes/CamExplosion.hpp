//
//  CamExplosion.hpp
//  Santra
//
//  Created by Anıl Gülgör on 26/07/16.
//
//

#ifndef CamExplosion_hpp
#define CamExplosion_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AppManager.hpp"

using namespace cocos2d;

USING_NS_CC;

class CamExplosion : public cocos2d::Node {
    
public:
    
    static CamExplosion *create(Vec2 position);
    
    AppManager *appManager;
    
    void update(float dt);
    
    void setUp();
    
    void startAnimation();
    
    bool animStart = false;
    
    Vec2 _position;
    
    
private:
    
    CamExplosion(Vec2 position);
    
    int animSpeed, animCount, frameCount;
    
    
    
    Sprite *expBody;
    
};


#endif /* CamExplosion_hpp */
