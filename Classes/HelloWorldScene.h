#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "AppManager.hpp"
#include "cocos-ext.h"
#include "appwarp.h"
#include "App42API.h"
#include <cocos/ui/UIEditBox/UIEditBox.h>
#include <cocos/ui/UIScale9Sprite.h>
#include <cocos/ui/UIVideoPlayer.h>
#include "MenuScene.hpp"
#include "picojson.h"
#include "SimpleAudioEngine.h"


#define APPWARP_APP_KEY     "26d0bbabc7da3fb8710dab08f04edbfd4e6e1938b9fc2202bc46e627003b26a2"
#define APPWARP_SECRET_KEY  "4f9c64daa2dec09603758f664a516844b39a60118d2da90ea6f02dba965ed303"
#define GAME_NAME           "onlinegametest"
#define ROOM_ID             "1401165824"
#define ROOM_NAME           "anilsroom"
#define ROOM_OWNER          "anilgulgor"
#define MAX_PLAYER          2


using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d::experimental::ui;
using namespace std;

class HelloWorld : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate, public AppWarp::ConnectionRequestListener, public App42CallBack
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    AppManager *appManager;
    
    VideoPlayer *introVideo;
    
    void videoPlayOverCallback(); 
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
    #endif
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void onEnterTransitionDidFinish();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float dt);
    
    
    //editbox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, string &text);
    virtual void editBoxReturn(EditBox * editBox);
    
    
    //app42 service callbacks
    
    void onUserRequestCompleted(void *response);
    void onStorageRequestCompleted(void *response);
    void onUserPasswordResetCompleted(void *response);
    void onFindUserByEmailCompleted(void *response);
   
    
    //appwarp
    void connectToAppwarp();
    UserService * initApp42Service();
    void app42CreatePlayer(const char *username, const char *password, const char *email);
    void authenticateUser(const char *username, const char *password);
    void insertUserInfoToService(const char *username, int gold, int level, int avatar, int xp, int games, int won, int lost, int point);
    void updateUserProfile(const char *jsonString);
    void getUserInfosWithUserName(const char *username);
    void resetUserPassword();
    
    void sifreYenile(const char *username);
    
    void findUserByEmailId(const char *email);
    
    void autoLogin();
    
    void removeResetScene();
    
    void openErrorMenu(std::string error);
    
    
    void onConnectDone(int res, int reasonCode);
    
    void scheduleRecover();
    void unscheduleRecover();
    
    void recover(float dt);
    
    void removeErrorPanel();
    
    void removeGirisSceneForLogin();
    
    void removeGirisSceneForRegister();
    
    
    void girisLoaderAnimation();
    
    //exit android
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
    
    cocos2d::__String *usernamelabel, *passwordlabel, *emaillabel;
    
    
    Node *quitPanel;
    
    Button *yesButton, *noButton;
    
    Sprite *quitBG;
    
    void callQuitKeyAction();
    
    
private:
    
    std::string kayitUserName, kayitParola, kayitEmail;
    
    float WIDTH, HEIGHT;
    
    Sprite *menuBackground;
    
    Sprite *girisYap, *uyeOl;
    
    Sprite *tamamButton, *forgotPassButton;
    
    Sprite *loaderBack, *loader;
    
    Node *errorPanel;
    
    Sprite *blackBg;
    
    Sprite *backButton;
    
    Label *tamamLabel, *forgotLabel;
    
    //label
    
    Label *connectingLabel;
    
    const char *tempPassword;
    
    //editbox
    
    EditBox *name, *password, *email;
    
    Sprite *nameSp ,*passSp, *emailSp;
    
    Label *kullaniciAdiLabel , *passwordLabel;
    
    int actOfScene = 0;
    
    int tempScene;
    
    
    //auto login
    
    bool autoLogining;
    
    //scene actions
    
    void createMainScene();
    void girisAction();
    void uyeOlAction();
    
    void playIntro();
    void girisAnimasyon();
    
    void createGirisScene();
    void createUyeOlScene();
    
    void removeGirisScene();
    void removeUyeOlScene();
};

#endif // __HELLOWORLD_SCENE_H__
