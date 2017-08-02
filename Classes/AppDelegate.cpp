#include "AppDelegate.h"
#include "HelloWorldScene.h"
#ifdef SDKBOX_ENABLED
#include "PluginIAP/PluginIAP.h"
#include "PluginVungle/PluginVungle.h"
#include <PluginAdMob/PluginAdMob.h>
#endif
#include "MenuScene.hpp"
#include "MagazaScene.hpp"

USING_NS_CC;


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
    
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
#ifdef SDKBOX_ENABLED
    sdkbox::IAP::init();
    sdkbox::PluginVungle::init();
    sdkbox::PluginAdMob::init();
#endif
    
    appManager = AppManager::getInstance();
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Santra", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Santra");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    Device::setKeepScreenOn(true);

    // create a scene. it's an autorelease object
    auto scene = introScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    

    if(appManager->isOnline == true && appManager->isConnected){
        
        AppWarp::Client *warpClientRef;
        
        warpClientRef->leaveRoom(appManager->myCurrentRoomId);
        
    }
    
    /*if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        UserDefault *userdefaults = UserDefault::getInstance();
        
        userdefaults->setIntegerForKey("tempSessionID", userdefaults->getIntegerForKey("sessionID"));
        
        userdefaults->setIntegerForKey("sessionID", NULL);
        
        userdefaults->flush();
        
    }*/
    
    printf("ŞAMPİYON");
    
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    
   /* if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        UserDefault *userDefault = UserDefault::getInstance();
        
        userDefault->setIntegerForKey("sessionID", AppWarp::Client::getInstance()->getSessionID());
        
        userDefault->flush();
        
    }*/
    
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
}


void AppDelegate::applicationWillTerminate(){
    
    UserDefault *userdefaults = UserDefault::getInstance();
    
    userdefaults->setIntegerForKey("sessionID", NULL);
    
    userdefaults->flush();
    

    AppWarp::Client *warpClientRef;
        
    warpClientRef = AppWarp::Client::getInstance();
        
    warpClientRef->leaveRoom(appManager->myCurrentRoomId);
        
    warpClientRef->disconnect();
    
}

