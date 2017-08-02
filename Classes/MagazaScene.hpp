//
//  MagazaScene.hpp
//  Santra
//
//  Created by Anıl Gülgör on 08/12/15.
//
//

#ifndef MagazaScene_hpp
#define MagazaScene_hpp

#include <stdio.h>
#include <PluginIAP/PluginIAP.h>
#include "PluginVungle/PluginVungle.h"
#include "cocos2d.h"
#include "AppManager.hpp"
#include "MenuScene.hpp"
#include "cocos-ext.h"
#include "appwarp.h"
#include "App42API.h"
#include "picojson.h"
#include "CamExplosion.hpp"



using namespace cocos2d;
using namespace cocos2d::extension;
using namespace sdkbox;

USING_NS_CC;

class MagazaScene: public cocos2d::Layer , public AppWarp::ConnectionRequestListener,public AppWarp::NotificationListener, public AppWarp::RoomRequestListener, public AppWarp::ZoneRequestListener, public sdkbox::IAPListener ,  public App42CallBack, public sdkbox::VungleListener {

    
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    
    void onEnterTransitionDidFinish();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float dt);
    
    
    // a selector callback
    void onRequestIAP(cocos2d::Ref* sender);
    void onRestoreIAP(cocos2d::Ref* sender);
    void onIAP(cocos2d::Ref* sender);
    
    void updateIAP(const std::vector<sdkbox::Product>& products);
    virtual void onInitialized(bool ok) override;
    virtual void onSuccess(sdkbox::Product const& p) override;
    virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
    virtual void onCanceled(sdkbox::Product const& p) override;
    virtual void onRestored(sdkbox::Product const& p) override;
    virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
    virtual void onProductRequestFailure(const std::string &msg) override;
    void onRestoreComplete(bool ok, const std::string &msg) override;
    
    // a selector vungle callbacks
    
    void onVungleCacheAvailable();
    void onVungleStarted();
    void onVungleFinished();
    void onVungleAdViewed(bool isComplete);
    void onVungleAdReward(const std::string& name);
    
    
    //app42
    
    void updateUserInformation(const char *username, double gold, double xp, double level, double avatar, double games, double won, double lost, double point);
    void updateUserProfile(const char *jsonString);
    
    //app42 responses
    
    void onStorageRequestCompleted(void *response);
    
    
    //appwarp control server
    
    void controlForConnection(float dt);
    
    bool waitForRecover = false;
    
    //appwarp
    
    //exit android
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
    
    
    void connectToAppwarp();
    void onConnectDone(int res, int reasonCode);
    
    void noConnectionWarning(std::string st1, std::string st2, std::string st3);
    
    void removeNoConnectionWarning();
    
    void recover(float dt);
    
    void goBackMainForError(float dt);
    
    void scheduleRecover();
    void unscheduleRecover();


    Node *quitPanel;
    
    cocos2d::ui::Button *yesButton, *noButton;
    
    Sprite *quitBG;
    
    void callQuitKeyAction();
    
    
    CREATE_FUNC(MagazaScene);

    
    
private:
    
    AppManager *appManager;
    
    
    CamExplosion *camExp;
    
    Sprite *removeAdsButton, *restoreButton;
    
    float WIDTH, HEIGHT;
    
    Sprite *background, *redBar;
    
    Sprite *altin,*krampon,*kart,*video, *backButton;
    
    Sprite *blackBg, *loaderBack, *loader;
    
    Label *connectingLabel;
    
    Label *goldLabel;
    
    float distanceOfItems;
    
    void addLoader();
    
    void removeLoader();
    
    void removeRewardPanel();
    
    void openVideoRewardPanel();
    
    std::vector<sdkbox::Product> _products;
    
    //appwarp ***
    
    Sprite *ayarlarPerde,*ayarlarPerde2;
    
    Node *errorPanel, *errorPanel2;
    
    void openRewardPanelThread();
    
    //methods
    
    void createButtons();
    void createItems();
    
    void openRewardPanel(std::string str);
    
    //vec methods
    
    cocos2d::Rect getRect(cocos2d::Node* node);
    
    void addItemsToScreen(ssize_t vecCount);
    void removeItemsFromScreen(ssize_t vecCount);
    
    //vectors
    Vector<Sprite*> altinVec;
    Vector<Sprite*> kramponVec;
    Vector<Sprite*> kartVec;
    
    Vector<Sprite*> sliderVec;
    Node *itemNode;
    
    
    bool canSlide = true;
    
    //bools
    bool isTouchDown;
    
    float initialTouchPos, currentTouchPos;
    
    double dist, speed, accelerate;
    
    
    
};


#endif /* MagazaScene_hpp */
