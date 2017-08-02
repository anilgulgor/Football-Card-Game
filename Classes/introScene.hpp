//
//  introScene.hpp
//  Santra
//
//  Created by Anıl Gülgör on 17/02/16.
//
//

#ifndef introScene_hpp
#define introScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <cocos/ui/UIVideoPlayer.h>
#include "AppManager.hpp"
#include "HelloWorldScene.h"



using namespace cocos2d;
using namespace cocos2d::experimental::ui;

class introScene : public cocos2d::Layer {
    
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(introScene);
    
    
    void videoPlayOverCallback();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
#endif

    
private:
    
    
    VideoPlayer *introVideo;
    
    void showIntro();
    
    float WIDTH, HEIGHT;
    
    
};


#endif /* introScene_hpp */
