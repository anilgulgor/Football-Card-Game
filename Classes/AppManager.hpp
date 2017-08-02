//
//  AppManager.hpp
//  MyNewGame
//
//  Created by Anıl Gülgör on 05/10/15.
//
//

#ifndef AppManager_hpp
#define AppManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include <stdarg.h>
#include <memory>
#include "appwarp.h"
#include <CCScheduler.h>
#include "picojson.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Helper.h"
#endif


using namespace cocos2d;



USING_NS_CC;



class AppManager : public AppWarp::ConnectionRequestListener , public Scheduler  {
    
#define GET_WIDTH cocos2d::Director::getInstance()->getVisibleSize().width
#define GET_HEIGHT cocos2d::Director::getInstance()->getVisibleSize().height
    
#define FIT_WH(__node__,__width__,__height__) __node__->setScale(__width__/__node__->getContentSize().width,__height__/__node__->getContentSize().height)
    
#define FIT_W(__node__,__width__) __node__->setScale(__width__/__node__->getContentSize().width)
    
#define FIT_H(__node__,__height__) __node__->setScale(__height__/__node__->getContentSize().height)
    
    
#define TimeApiURL "http://www.timeapi.org/utc/now/"
    
    
#define APPWARP_APP_KEY     "26d0bbabc7da3fb8710dab08f04edbfd4e6e1938b9fc2202bc46e627003b26a2"
#define APPWARP_SECRET_KEY  "4f9c64daa2dec09603758f664a516844b39a60118d2da90ea6f02dba965ed303"
#define GAME_NAME           "onlinegametest"
#define ROOM_ID             "1401165824"
#define ROOM_NAME           "anilsroom"
#define ROOM_OWNER          "anilgulgor"
#define MAX_PLAYER          2

    
public:
    
    static AppManager* getInstance(void);

    
    bool isConnected, isOnline;
    
    std::string username;
    double gold;
    double level;
    double xp;
    double betGold;
    double betXp;
    
    double limitGold;
    
    std::string password;
    
    double avatarNumber;
    double rakipAvatarNumber;
    
    bool sound, fx;
    
    
    bool removeads;
    
    bool isUserAdmin;
    
    std::string myCurrentRoomId;
    std::string roomOwner;
    
    std::string kindOfGame;
    
    std::string opponentName;
    
    std::string levelForRoom;
    std::string goldForRoom;
    std::string roomName;
    std::string roomPassword;
    
    double userWon;
    double userLost;
    double userPoint;
    double userGames;
    
    std::string turnuvaName;
    
    std::string sformat(const std::string fmt_str, ...);
    
    bool authenticated;
    
    //appwarp
    void connectToAppwarp();
    void disconnectFromAppwarp();
    void onConnectDone(int res, int reasonCode);
    void onDisconnectDone(int res);
    
    //void onDisconnectDone(int result);
    
    void scheduleRecover();
    void unscheduleRecover();
    
    void recover(float dt);
    
    void handleRecover();
    
    int lengthOfU(unsigned char * str);
    
    std::string currentTime;
    
    bool isIpad();
    
    bool currentLanguage(); //false for turkish, true for english
    
    

    
private:
    
    virtual ~AppManager();
    static bool init(void);

    
    
};

#endif /* AppManager_hpp */
