//
//  Explosion.hpp
//  Santra
//
//  Created by Anıl Gülgör on 27/03/16.
//
//

#ifndef Explosion_hpp
#define Explosion_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AppManager.hpp"

using namespace cocos2d;

USING_NS_CC;

class Explosion : public cocos2d::Node {
    
public:
    
    static Explosion *create(Vec2 position);
    
    AppManager *appManager;
    
    void update(float dt);
    
    void setUp();
    
    void startAnimation();
    
    bool animStart = false;
    
    Vec2 _position;
    
    
private:
    
    Explosion(Vec2 position);
    
    int animSpeed, animCount, frameCount;
    
    
    
    Sprite *expBody;
    
};

#endif /* Explosion_hpp */
