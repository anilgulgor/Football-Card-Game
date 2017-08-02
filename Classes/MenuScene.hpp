//
//  MenuScene.hpp
//  Santra
//
//  Created by Anıl Gülgör on 08/12/15.
//
//

#ifndef MenuScene_hpp
#define MenuScene_hpp

#include <stdio.h>


#include <stdarg.h>
#include "cocos2d.h"
#include "appwarp.h"
#include "App42API.h"
#include "AppManager.hpp"
#include "HelloWorldScene.h"
#include "MagazaScene.hpp"
#include "GameScene.hpp"
#include "LoadingGameScene.hpp"
#include "TurnuvaScene.hpp"
#include <cocos/ui/UILoadingBar.h>
#include <cocos/ui/UIEditBox/UIEditBox.h>
#include <cocos/ui/UIScale9Sprite.h>
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "cocos/network/HttpClient.h"



USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocos2d::network;

class MenuScene : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate ,  public AppWarp::ConnectionRequestListener, public AppWarp::NotificationListener, public App42CallBack {
    
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(MenuScene);
    
    void onEnterTransitionDidFinish();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    //editbox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, string &text);
    virtual void editBoxReturn(EditBox * editBox);
    
    
    void onConnectDone(int res, int reasonCode);
    void onDisconnectDone(int res);
    void onUserPaused(std::string user, std::string locId, bool isLobby);
    
    void scheduleRecover();
    void unscheduleRecover();
    
    void recover(float dt);
    
    bool waitForRecover = false;
    
    void controlForConnection(float dt);
    
    void noConnectionWarning(std::string st1,std::string st2, std::string st3);
    
    void noItemWarning(std::string st1, std::string st2);
    
    void passwordErrorPanel(std::string str);
    
    void removeNoItemWarning(float dt);
    
    void removeNoConnectionWarning();
    
    void goBackMainForError(float dt);
    
    void removeChangePassword();
    
    void removeChangePasswordBefore();
    
    void removeRoomErrorPanel();
    
    //exit android
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
    
    //editbox var def
    
    EditBox *roomName, *roomPassword;
    
    EditBox *oldPasswordBox, *newPasswordBox, *newPasswordAgainBox;
    
    UserDefault *userAvatarData;
    Vector<Sprite*>avatarsVector;
    int avatarNumber;
    Sprite *userAvatar;
    
    //app42
    
    void updateUserInformation(const char *username, double gold, double xp, double level, double avatar, double games, double won, double lost, double point);
    void updateUserForAvatar(const char *username, double gold, double xp, double level, double avatar, double games, double won, double lost, double point);
    void updateUserProfile(const char *jsonString);
    void changePassword(const char *username, const char *old_password, const char *new_password);
    
    //app42 responses
    
    void onStorageRequestCompleted(void *response);
    
    void onAvatarStorageRequestComplete(void *response);
    
    void onChangePasswordRequestComplete(void *response);
    
    void onTimerServiceRequestCompleted(void *response);
    
    //visual functions
    
    void spendGolds(double goldCount);
    
    
    //HTTP Get
    
    HttpRequest *request;
    
    void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
    
    
    void getTimeFromAPI();
    
    std::string currentTime;
    
    Node *quitPanel;
    
    Button *yesButton, *noButton;
    
    Sprite *quitBG;
    
    void callQuitKeyAction();
    


private:
    
    float WIDTH, HEIGHT;
    int actOfScene = 0;
    
    AppManager *appManager;
    
    //scene actions
    
    void createTopViews();
    
    void createMainMenu();
    void removeMainMenu();
    void removeArkadasMenu();
    void createOlumMaciMenu();
    void removeOlumMaciMenu();
    
    void startOlumMaci();
    void playSuccessOlumMaci();
    void removeAltinPanelForOlumMaci();
    
    void createOynaMenu();
    void createArkadaslaOynaMenu();
    void removeOynaMenu();
    
    void openAvatarPanel();
    void createAvatarsPanel();
    
    void altinRewardPanel(std::string str);
    
    void gotoMagaza();
    
    void openAyarlar();
    void openChangePasswordPanel();
    
    void openRoomErrorPanel(std::string str);
    
    void removeEkip();
    
    void arkadasOdaKur();
    void arkadasOdaBul();
    
    void altinAnim();
    
    void endAltinAnim(Sprite *blackBG, Sprite *altinlar, Label *altinLabel, Label *altinLabel2, Sprite *altins, Button *tamamBut, Sprite *panelBG, Label *altinLabel3);
    
    Sprite *background, *backButton;
    
    Sprite *olumMaciSingleButton, *olumMaciMultiButton;
    
    Sprite *ayarlarPerde;
    
    //üst menü
    Sprite *solUstBar, *Bildirim, *Ayarlar, *Avatar;
    Sprite *sagUst, *Krampon, *altin, *arti;
    
    LoadingBar *levelBar;
    
    //avatarpanel
    
    Label *usernameLabel , *levelLabel, *goldLabel;
    
    Node *panelAvatar;
    
    Node *errorPanel;
    
    Sprite *avatarOnPanel,*avatarCikis;
    
    Node *panelAyarlar;
    
    Node *olumMaciAltinPanel;
    
    Sprite *ayarlarOnPanel, *ayarlarCikis;
    
    bool isAvatarPanelOpened = false;
    
    bool isAyarlarPanelOpened = false;
    
    Sprite *ekip, *sifreDegistir, *muzik, *ses, *xbuton;
    
    Vector<Sprite*>ayarlarButonlar;
    
    std::string temp_password, old_temp_password;
    
    
    //ana menü
    Sprite *oyna, *magaza, *magazine;
    
    //oyun menü
    Sprite *olumMaci, *turnuva , *tekKisi, *arkadasinlaOyna;
    
    //arkadaşla oyna panel
    Sprite *odaKur, *odaBul, *odaKurBulPanel;
    Sprite *odaKurtext, *odaBultext;
    Label *odaBulLabel, *odaKurLabel;
    Sprite *arkadasTamam;
    
    cocos2d::__String *roomNameLabel, *roomPasswordLabel;
    
    Label *arkadasTamamLabel;
    
};



#endif /* MenuScene_hpp */
