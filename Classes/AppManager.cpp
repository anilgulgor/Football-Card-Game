//
//  AppManager.cpp
//  MyNewGame
//
//  Created by Anıl Gülgör on 05/10/15.
//
//

#include "AppManager.hpp"


USING_NS_CC;

using namespace cocos2d;


bool AppManager::init() {

    
    
    return true;
}

static AppManager *_sharedAppManager = NULL;

AppManager* AppManager::getInstance()
{
    if (! _sharedAppManager)
    {
        _sharedAppManager = new AppManager();
        _sharedAppManager->init();
        _sharedAppManager->isConnected = false;
        _sharedAppManager->isOnline = false;
        _sharedAppManager->username = "";
        _sharedAppManager->isUserAdmin = false;
        _sharedAppManager->kindOfGame = "";
        _sharedAppManager->levelForRoom = "";
        _sharedAppManager->goldForRoom = "";
        _sharedAppManager->myCurrentRoomId = "";
        _sharedAppManager->roomOwner = "";
        _sharedAppManager->opponentName = "";
        _sharedAppManager->roomName = "";
        _sharedAppManager->roomPassword = "";
        _sharedAppManager->avatarNumber = 0;
        _sharedAppManager->rakipAvatarNumber = 0;
        _sharedAppManager->authenticated = false;
        _sharedAppManager->gold = 0;
        _sharedAppManager->level = 0;
        _sharedAppManager->xp = 0;
        _sharedAppManager->userGames = 0;
        _sharedAppManager->userLost = 0;
        _sharedAppManager->userPoint = 0;
        _sharedAppManager->userWon = 0;
        _sharedAppManager->turnuvaName = "";
        _sharedAppManager->removeads = false;
        _sharedAppManager->currentTime = "";
        _sharedAppManager->betGold = 0;
        _sharedAppManager->betXp = 0;
        _sharedAppManager->sound = true;
        _sharedAppManager->fx = true;
        _sharedAppManager->limitGold = 0;
        _sharedAppManager->password = "";
        
    }
    
    return _sharedAppManager;
}


AppManager::~AppManager(void)
{
    
}

void AppManager::connectToAppwarp() {
    
    AppWarp::Client *warpClientRef;
    
    AppWarp::Client::initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    
    //set username sharedinstance
    
    warpClientRef->connect(_sharedAppManager->username);
    
    
    
}

void AppManager::handleRecover() {
    
    AppWarp::Client *warpClientRef;
    
    AppWarp::Client::initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(600);
    warpClientRef->setConnectionRequestListener(this);
    
}

void AppManager::disconnectFromAppwarp() {
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();
    
    warpClientRef->setConnectionRequestListener(this);
    
    warpClientRef->disconnect();
    
}

void AppManager::onDisconnectDone(int res){
    
    printf("çıktım");
    
}

void AppManager::onConnectDone(int res, int reasonCode){
    
    if(res == AppWarp::ResultCode::success){
        
        unscheduleRecover();
        printf("\nonConnectDone .. SUNUCUYA BAŞARIYLA BAĞLANDI..session=%d\n",AppWarp::AppWarpSessionID);
        
        //başarılı bağlantı
        
        isConnected = true;
        
        
        
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. SUCCESS with success_recovered..session=%d\n",AppWarp::AppWarpSessionID);
        
        isConnected = true;
        
        //başarılı bağlantı recover
        
        
    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {
        scheduleRecover();
        printf("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
        
        //tekrar deneyecek
        
    }
    else if (res==AppWarp::ResultCode::bad_request)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
        
        //uyarı ver bad request
        
    }
    else
    {
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with reasonCode=%d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
        
        //uyarı ve reason code
        
    }
    
    
    
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

bool AppManager::isIpad(){
    
    if(Helper::isThisIpad() == true){
        
        CCLOG("ipaddddd");
        
    }
    else {
        
        CCLOG("değilll");
        
    }
    
    return Helper::isThisIpad();
    
}

#endif

void AppManager::recover(float dt)
{
    printf("\nHelloWorld::recover");
    AppWarp::Client::getInstance()->recoverConnection();
    
}

void AppManager::scheduleRecover()
{
    printf("\nHelloWorld::scheduleRecover");
    
    this->schedule(schedule_selector(AppManager::recover), this, 1.0f, false);
    
    //showReconnectingLayer("Reconnecting ...");
}

void AppManager::unscheduleRecover()
{
    printf("\nHelloWorld::unscheduleRecover");
    
    this->unschedule(schedule_selector(AppManager::recover), this);
    
    Application::getInstance()->getCurrentLanguage();
    
}

int AppManager::lengthOfU(unsigned char * str)
{
    int i = 0;
    
    while(*(str++)){
        i++;
        if(i == INT_MAX)
            return -1;
    }
    
    return i;
}


bool AppManager::currentLanguage() {
    
    if(Application::getInstance()->getCurrentLanguage() == cocos2d::LanguageType::TURKISH){
        
        return false;
        
    }
    else {
        
        return true;
        
    }
    
    
}


std::string sformat(const std::string fmt_str, ...)
{
    int final_n, n = ((int)fmt_str.size()) * 2;
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]);
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
};

