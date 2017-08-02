//
//  TurnuvaScene.hpp
//  Santra
//
//  Created by Anıl Gülgör on 16/12/15.
//
//

#ifndef TurnuvaScene_hpp
#define TurnuvaScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AppManager.hpp"
#include "Opponent.hpp"
#include "Player.hpp"
#include "appwarp.h"
#include "App42API.h"
#include "MenuScene.hpp"
#include "MagazaScene.hpp"
#include "LoadingGameScene.hpp"
#include <ui/UILoadingBar.h>


USING_NS_CC;
using namespace cocos2d;

class TurnuvaScene : public cocos2d::Layer, public App42CallBack, public AppWarp::ConnectionRequestListener, public AppWarp::NotificationListener, public AppWarp::RoomRequestListener, public AppWarp::ZoneRequestListener{
    
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    
    void onEnterTransitionDidFinish();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float dt);
    
    
    CREATE_FUNC(TurnuvaScene);
    
    //app42
    
    void updateUserInformation(const char *username, double gold, double xp, double level, double avatar,double games, double won , double lost, double point);
    void updateUserProfile(const char *jsonString);
    void updateUserForAvatar(const char *username, double gold, double xp, double level, double avatar,double games, double won , double lost, double point);
    
    void onConnectDone(int res, int reasonCode);
    
    void noConnectionWarning(std::string st1, std::string st2, std::string st3);
    
    void removeNoConnectionWarning();
    
    void goBackMainForError(float dt);
    
    void scheduleRecover();
    void unscheduleRecover();
    
    void recover(float dt);

    void controlForConnection(float dt);
    
    
    void getTopRankers(int max, std::string gameName);
    
    //app42 responses
    
    void onStorageRequestCompleted(void *response);
    
    void onScoreboardRequestCompleted(void *response);
    
    void onAvatarStorageRequestComplete(void *response);
    
    void openTurnuvaScene(double limitGold, double limitLevel, std::string turnuvaName);
    
    void removeTurnuvaScene();
    
    void openWarningPanel(std::string warning);
    
    void openPricePanel(double limitGold, double lig);
    
    void closeWarningSchedule(float dt);
    
    void openAvatarPanel();
    void createAvatarsPanel();
    
    //exit android
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
    
    Sprite *currentAvatar;
    
    int leftSlide;
    int rightSlide;

    
    
private:
    
    double limitLevel;
    double limitGold;
    
    double lig;
    
    int actOfScene;
    
    AppManager *appManager;
    
    float distanceOfItems;
    
    bool buttonTouch = true;
    
    bool waitForRecover = false;
    
    //assets
    Sprite *background,*top;
    Sprite *backButton;
    
    Sprite *ayarlarPerde;
    
    Node *itemNode;
    
    Node *errorPanel, *errorPanel2, *duyuruPanel;
    
    Node *quitPanel;
    
    Button *yesButton, *noButton;
    
    Sprite *quitBG;
    
    void callQuitKeyAction();
    
    
    Node *ligTableNode;
    Sprite *ligTable, *playButton, *userStatisticTable;
    
    //üst menü
    Sprite *solUstBar, *Bildirim, *Ayarlar, *Avatar;
    Sprite *sagUst, *Krampon, *altin, *arti;
    
    cocos2d::ui::LoadingBar *levelBar;
    
    bool isAvatarPanelOpened = false;
    
    
    Sprite *avatarOnPanel,*avatarCikis;
    
    Label *usernameLabel , *levelLabel, *goldLabel;
    Sprite *userAvatar;
    
    Node *panelAvatar;

    
    
    UserDefault *userAvatarData;
    Vector<Sprite*>avatarsVector;
    int avatarNumber;

    
    //vars
    float initialTouchPos, currentTouchPos;
    
    double dist, speed, touchedLigId;
    
    float WIDTH, HEIGHT;

    bool touchMoved = false;
    
    //vectors
    
    Vector<Sprite*>ligButtonsVector;
  
    
    //functions
    
    void setScene();
    void createItems();
    
    void createTopViews();
    
    
    //vars
    
    Vector<String*>enoughLevelsForLeague;
    Vector<String*>enoughGoldForLeague;

    
};


#endif /* TurnuvaScene_hpp */
