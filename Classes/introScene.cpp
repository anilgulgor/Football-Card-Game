//
//  introScene.cpp
//  Santra
//
//  Created by Anıl Gülgör on 17/02/16.
//
//

#include "introScene.hpp"

Scene* introScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = introScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}



bool introScene::init(){
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    WIDTH = GET_WIDTH;
    HEIGHT = GET_HEIGHT;
    
    showIntro();
        
    return true;
    
}


void introScene::showIntro() {
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        introVideo = VideoPlayer::create();
        
        introVideo->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
        introVideo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        introVideo->setContentSize(Size(Vec2(WIDTH,HEIGHT)));
        introVideo->setHighlighted(false);
        introVideo->setFullScreenEnabled(true);
        introVideo->setBrightStyle(cocos2d::ui::Widget::BrightStyle::NONE);
        introVideo->setLayoutComponentEnabled(false);
        this->addChild(introVideo,500);
        
        if(introVideo){
            
            introVideo->setFileName("Intro Santra Final.mp4");
            introVideo->play();
            
        }
        
    
        
        introVideo->addEventListener(CC_CALLBACK_2(introScene::videoEventCallback, this));
        
    }
    
    
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

void introScene::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
    switch (eventType) {
        case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
            
            if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
                
                introVideo->resume();
                
            }
            
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
            videoPlayOverCallback();
            break;
        default:
            break;
    }
}

#endif

void introScene::videoPlayOverCallback(){
    
    printf("video bitti");
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene, Color3B::BLACK));
    
}


