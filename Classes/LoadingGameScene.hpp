//
//  LoadingGameScene.hpp
//  Santra
//
//  Created by Anıl Gülgör on 16/12/15.
//
//

#ifndef LoadingGameScene_hpp
#define LoadingGameScene_hpp

#include <stdio.h>
#include "AppManager.hpp"
#include "Player.hpp"
#include "Opponent.hpp"
#include "appwarp.h"
#include "cocos-ext.h"
#include "GameScene.hpp"
#include "cocos2d.h"
#include "MenuScene.hpp"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocos2d;

class LoadingGameScene : public cocos2d::Layer , public AppWarp::NotificationListener, public AppWarp::ZoneRequestListener, public AppWarp::ConnectionRequestListener, public AppWarp::RoomRequestListener {
    
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void onEnterTransitionDidFinish();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float dt);
    
     void goBackMainForError(float dt);
    
    CREATE_FUNC(LoadingGameScene);

    
    
private:

    AppManager *appManager;
    
    float WIDTH, HEIGHT;
    
    Sprite *background;
    
    Sprite *oduller,*vs,*odul1,*odul2,*isimButonPlayer,*isimButonRakip,*avatarPlayer,*avatarRakip;
    
    Sprite *backButton;
    
    bool backPanelisActive = false;
    
    
    //functions
    
    void setScene();
    
    void setNameForButtons(std::string name, Sprite *isimButon);
    
    void callCikisPanel();
    
    void quitGame();
    
    //nodes
    
    Sprite *ayarlarPerde;
    
    Node *errorPanel2;

    
    Node *cikisPanel;
    Sprite *evetButton, *hayirButton, *cikisBg;
    Label *cikisLabel;
    
    bool iamalone = true;
    //appwarp server
    
    void getRoomsInUserRange(int minUser,int maxUser);
    void getRoomWithProperties(std::map<std::string, std::string> tableProperty);
    void createRoom ( std :: string  name ,  std :: string  owner , int  maxUsers);

    
    void startAppwarp();
    
    void onChatReceived(AppWarp::chat chatevent);
    void sendMyAvatarNumberToOpponent(int avatarNumber);
    void onConnectDone(int res, int reasonCode);
    void onUserLeftRoom (AppWarp::room event , std::string username);
    void onLeaveRoomDone(AppWarp::room event);
    void onDeleteRoomDone(AppWarp::room event);
    void onJoinRoomDone(AppWarp::room revent);
    void onSubscribeRoomDone(AppWarp::room revent);
    void onGetMatchedRoomsDone(AppWarp::matchedroom event);
    void onCreateRoomDone(AppWarp::room event);
    void onUserJoinedRoom(AppWarp::room event , std::string username);
    void onUpdatePropertyDone(AppWarp::liveroom event);
    void onUpdatePeersReceived(AppWarp::byte *update, int len, bool isUDP);
    
    bool waitForRecover = false;
    
    
    void controlForConnection(float dt);
    void noConnectionWarning(std::string st1, std::string st2, std::string st3);
    
    void removeNoConnectionWarning();
    
   
    
    void scheduleRecover();
    void unscheduleRecover();
    
    void recover(float dt);
    
    //exit android
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
    
    bool baslaChat = false;
    
    void SendMyDataToStartGame();
    
    void adminStartGame(float dt);
    void guestStartGame(float dt);
    
    int lengthOfU(unsigned char * str);
    
    bool opponentFound = false;
    int countChangeAvatar = 10;
    int countAvatarNumber = 0;
    Sprite *avatarChangeSprite;
    
    Label *informLabel;
    
    Label *butName, *userNameBut;
    
    std::string informStr;
    
    bool iWillQuit = false;
    
    bool isOpponentAvatarReady = false;
    
    //room properties for game
    
    std::string levelForRoom;
    std::string goldForRoom;
    std::string roomName;
    std::string roomPassword;

    //Vector for avatars
    
    Vector<Sprite *>AvatarsVector;
    
    void createAvatarForPlayer(int playerAvatarNumber);
    
    void createAvatarForOpponent(int opponentAvatarNumber);
    
};

#endif /* LoadingGameScene_hpp */
