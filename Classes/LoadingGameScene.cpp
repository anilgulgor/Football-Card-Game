//
//  LoadingGameScene.cpp
//  Santra
//
//  Created by Anıl Gülgör on 16/12/15.
//
//

#include "LoadingGameScene.hpp"


Scene* LoadingGameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadingGameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LoadingGameScene::init(){
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    WIDTH = GET_WIDTH;
    HEIGHT = GET_HEIGHT;
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        this->setKeypadEnabled(true);
        
    }
    
    //Background music
    
    appManager = AppManager::getInstance();
    
    if(appManager->sound == true){
    
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("loading.mp3");
        
    }
    else {
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("loading.mp3");
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        
    }
    
    
    
    levelForRoom = appManager->levelForRoom;
    goldForRoom = appManager->goldForRoom;
    roomName = appManager->roomName;
    roomPassword = appManager->roomPassword;
    
    iWillQuit = false;
    
    setScene();
    
    informLabel = Label::createWithTTF(informStr, "chinese rocks rg.ttf", 50);
    
    informLabel->setPosition(Vec2(WIDTH*.5, HEIGHT*.1));
    
    this->addChild(informLabel);
    
    if(appManager->currentLanguage() == false){
        
        this->setNameForButtons("ARANIYOR...", isimButonRakip);
        
    }
    else {
        
        this->setNameForButtons("SEARCHING...", isimButonRakip);
        
    }

    
    createAvatarForPlayer(appManager->avatarNumber);
    
    startAppwarp();
    
    
    return true;
    
}

void LoadingGameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event){
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        /*UserDefault *userDefaults = UserDefault::getInstance();
        
        userDefaults->setIntegerForKey("sessionID", NULL);
        
        userDefaults->setIntegerForKey("tempSessionID", NULL);
        
        userDefaults->flush();
        
        AppWarp::Client *warpClientRef;
        
        warpClientRef = AppWarp::Client::getInstance();
        
        warpClientRef->leaveRoom(appManager->myCurrentRoomId);
        
        warpClientRef->disconnect();
        
        Director::getInstance()->end();*/
        
        this->callCikisPanel();
        
    }
    
}


bool LoadingGameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
        
        backButton->setOpacity(128);
        
    }
    
    if(backPanelisActive == true){
        
        Point nodePoint = cikisPanel->convertToNodeSpaceAR(touch->getLocation());
        
        if(evetButton->getBoundingBox().containsPoint(nodePoint)){
            
            evetButton->setOpacity(128);
            
        }
        else if(hayirButton->getBoundingBox().containsPoint(nodePoint)){
            
            hayirButton->setOpacity(128);
            
        }
        
    }
    
    return true;

}

void LoadingGameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
        
        backButton->setOpacity(255);
        
    }
    
    if(backPanelisActive == true){
        
        Point nodePoint = cikisPanel->convertToNodeSpaceAR(touch->getLocation());
        
        if(!evetButton->getBoundingBox().containsPoint(nodePoint)){
            
            evetButton->setOpacity(255);
            
        }
        else if(!hayirButton->getBoundingBox().containsPoint(nodePoint)){
            
            hayirButton->setOpacity(255);
            
        }
        
    }
    
}

void LoadingGameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(backButton->getOpacity() == 128){
        
        backButton->setOpacity(255);
        
        if(backPanelisActive == false){
        
            callCikisPanel();
            
        }
        
    }
    
    if(backPanelisActive == true){
        
        if(evetButton->getOpacity() == 128){
            
            //oyundan çık
            
            backPanelisActive = false;
            
            quitGame();
            
        }
        else if(hayirButton->getOpacity() == 128){
            
            //bişey yapma node u yok et
            
            this->removeChild(cikisPanel);
            
            backPanelisActive = false;
            
        }
        
    }
    
}

void LoadingGameScene::quitGame() {
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();
    
    
        
        appManager->isOnline = false;
        
        iWillQuit = true;
        
        if(appManager->isUserAdmin == true){
        
            this->unschedule(schedule_selector(LoadingGameScene::adminStartGame));
            
        }
        else {
            
            this->unschedule(schedule_selector(LoadingGameScene::guestStartGame));
            
        }
        
        if(iamalone == true){
            
            //AppWarp::Client *warpClientRef;
            
            //warpClientRef = AppWarp::Client::getInstance();
            
            //warpClientRef->deleteRoom(appManager->myCurrentRoomId);
            
            //odadaki max kişi sayısını 3 yap.... böylece düzelir...
            
            std::map<std::string, std::string> tableProperty;
            std::vector<std::string> removes;
            
            tableProperty["roomName"] = roomName;
            tableProperty["roomPassword"] = roomPassword;
            tableProperty["levelForRoom"] = levelForRoom;
            tableProperty["goldForRoom"] = goldForRoom;
            tableProperty["minUser"] = StringUtils::format("%i",3);
            tableProperty["maxUser"] = StringUtils::format("%i",3);
            
            AppWarp::Client *warpClientRef;
            
            warpClientRef = AppWarp::Client::getInstance();
                        
            warpClientRef->updateRoomProperties(appManager->myCurrentRoomId, tableProperty, removes);

            
        }
        else {
            
            AppWarp::Client *warpClientRef;
            
            warpClientRef = AppWarp::Client::getInstance();
        
            warpClientRef->leaveRoom(appManager->myCurrentRoomId);
            
        }
        
        //cezalandır
        
        
    
    
}

void LoadingGameScene::onUserLeftRoom(AppWarp::room event, std::string username){
    
    
    //altın ver
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();

    warpClientRef->deleteRoom(event.roomId);
        
    
}

void LoadingGameScene::onDeleteRoomDone(AppWarp::room event){
    
    auto scene = MenuScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));

}

void LoadingGameScene::callCikisPanel() {
    
    backPanelisActive = true;
    
    cikisPanel = Node::create();
    
    cikisBg = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(cikisBg, WIDTH*.8, HEIGHT*.8);
            
        }
        else {
            
            FIT_WH(cikisBg,WIDTH*.8 ,HEIGHT*.8);
            
        }
        
    }
    else {
        
        FIT_WH(cikisBg,WIDTH*.8 ,HEIGHT*.8);
        
    }
    
 
    this->addChild(cikisPanel,10);
    
    cikisPanel->addChild(cikisBg);
    
    cikisBg->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    
    
    if(appManager->currentLanguage() == false){
    
        
        cikisLabel = Label::createWithBMFont("menuFont.fnt", "OYUNDAN CIKMAK ISTIYOR MUSUNUZ?");
        
    }
    else {

        cikisLabel = Label::createWithBMFont("menuFont.fnt", "DO YOU WANT TO LEAVE THE GAME?");
        
    }
    
    FIT_H(cikisLabel, HEIGHT*.1);
    
    cikisLabel->setPosition(Vec2(WIDTH*.5,HEIGHT*.7));
    
    cikisPanel->addChild(cikisLabel);
    
    //buttons
    
    evetButton = Sprite::create("tamamyeni.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
             FIT_H(evetButton, HEIGHT*.13);
            
        }
        else {
            
            FIT_H(evetButton, HEIGHT*.18);
            
        }
        
    }
    else {
        
         FIT_H(evetButton, HEIGHT*.18);
        
    }
    
   
    
    evetButton->setPosition(Vec2(WIDTH*.35, HEIGHT*.35));
    
    cikisPanel->addChild(evetButton);
    
    
    hayirButton = Sprite::create("tamamyeni.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(hayirButton, HEIGHT*.13);
            
        }
        else {
            
            FIT_H(hayirButton, HEIGHT*.18);
            
        }
        
        
        
    }
    else {
        
        FIT_H(hayirButton, HEIGHT*.18);
        
    }
    
    
    hayirButton->setPosition(Vec2(WIDTH*.65,HEIGHT*.35));
    
    cikisPanel->addChild(hayirButton);
    
    //labels
    
    Label *evetLabel,*hayirLabel ;
    
    if(appManager->currentLanguage() == false){
    
        evetLabel = Label::createWithTTF("Evet", "chinese rocks rg.ttf", 70);
        
    }
    else {
        
        evetLabel = Label::createWithTTF("Yes", "chinese rocks rg.ttf", 70);
        
    }
    
    evetLabel->setPosition(Vec2(evetButton->getContentSize().width/2,evetButton->getContentSize().height/2));
    
    evetLabel->setColor(Color3B::WHITE);
    
    evetButton->addChild(evetLabel);
    
    if(appManager->currentLanguage() == false){
    
        hayirLabel = Label::createWithTTF("Hayır", "chinese rocks rg.ttf", 70);
        
    }
    else {
        
        hayirLabel = Label::createWithTTF("No", "chinese rocks rg.ttf", 70);

        
    }
    
    hayirLabel->setPosition(Vec2(hayirButton->getContentSize().width/2,hayirButton->getContentSize().height/2));
    
    hayirLabel->setColor(Color3B::WHITE);
    
    hayirButton->addChild(hayirLabel);
    
}

void LoadingGameScene::startAppwarp() {
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    warpClientRef->setRoomRequestListener(this);
    warpClientRef->setZoneRequestListener(this);
    
    
    std::map<std::string, std::string> tableProperty;
    tableProperty["roomName"] = roomName;
    tableProperty["roomPassword"] = roomPassword;
    tableProperty["levelForRoom"] = levelForRoom;
    tableProperty["goldForRoom"] = goldForRoom;
    tableProperty["minUser"] = StringUtils::format("%i",1);
    tableProperty["maxUser"] = StringUtils::format("%i",1);
    
    this->getRoomWithProperties(tableProperty);
    
}

void LoadingGameScene::onConnectDone(int res, int reasonCode){
    
    if(res == AppWarp::ResultCode::success){
        
        unscheduleRecover();
        printf("\nonConnectDone .. SUNUCUYA BAŞARIYLA BAĞLANDI..session=%d\n",AppWarp::AppWarpSessionID);
        
        //başarılı bağlantı
        
        
        
        AppManager::getInstance()->isConnected = true;
        
        
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. SUCCESS with success_recovered..session=%d\n",AppWarp::AppWarpSessionID);
        
        if(waitForRecover == true){
            
            this->removeNoConnectionWarning();
            
            waitForRecover = false;
            
        }
        
        AppManager::getInstance()->isConnected = true;
        
        //başarılı bağlantı recover
        
        
    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {
        scheduleRecover();
        
        AppManager::getInstance()->isConnected = false;
        
        printf("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
        
        //tekrar deneyecek
        
        //burdapanel aç
        
    }
    else if (res==AppWarp::ResultCode::bad_request)
    {
        unscheduleRecover();
        
        appManager->isConnected = false;
        
        printf("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
        
        //uyarı ver bad request
        
    }
    else
    {
        unscheduleRecover();
        
        appManager->isConnected = false;
        
        printf("\nonConnectDone .. FAILED with reasonCode=%d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
        
        //uyarı ve reason code
        
    }
    
    
    
    
}
void LoadingGameScene::recover(float dt)
{
    
    printf("\nHelloWorld::recover");
    AppWarp::Client::getInstance()->recoverConnection();
    
    // 20 sn gibi bir süre ile bekle. hala isConnected false ise bil ki bağlantı yok. yoksa recover başarılı olabilirdi...
    
    if(waitForRecover == false){
        
        waitForRecover = true;
        
        if(appManager->currentLanguage() == false){
            
            this->noConnectionWarning("Bağlantı sorunu", "Sunucu ile tekrar bağlantı kuruluyor", "Lütfen bekleyin...");
            
        }
        else {
            
            this->noConnectionWarning("Connection Error", "Trying to connect to server again", "Please Wait...");
            
        }
        
        this->schedule(schedule_selector(LoadingGameScene::controlForConnection), 8.0f);
        
    }
    
}

void LoadingGameScene::scheduleRecover()
{
    printf("\nGameScene::scheduleRecover");
    this->schedule(schedule_selector(TurnuvaScene::recover), 1.0f);
    
    //showReconnectingLayer("Reconnecting ...");
}

void LoadingGameScene::unscheduleRecover()
{
    printf("\nGameScene::unscheduleRecover");
    this->unschedule(schedule_selector(TurnuvaScene::recover));
}



void LoadingGameScene::controlForConnection(float dt){
    
    this->unschedule(schedule_selector(MenuScene::controlForConnection));
    
    if(appManager->isConnected == false){
        
        //error ver bağlantı yok ve anasayfaya gönder.
        //wifi veya 3g bağlantınızı kontrol edin. (tekrar giriş yap)...
        
        waitForRecover = false;
        
        this->removeNoConnectionWarning();
        
        this->unscheduleRecover();
        
        if(appManager->currentLanguage() == false){
            
            this->noConnectionWarning("Sunucu ile bağlantı sorunu var","Wi-Fi veya 3G bağlantınızı kontrol edin","Giriş sayfasına yönlendiriliyorsunuz...");
            
        }
        else {
            
            this->noConnectionWarning("There is a connection error with server","Check your wi-fi or 3g connection","Navigating to the main menu");
            
        }
        
    }
    else if(appManager->isConnected == true){
        
        waitForRecover = false;
        
        this->removeNoConnectionWarning();
        
        //başarılı bağlantı...
        
    }
    
    
}


void LoadingGameScene::removeNoConnectionWarning(){
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel2);
    
}


void LoadingGameScene::noConnectionWarning(std::string st1, std::string st2, std::string st3){
    
    ayarlarPerde = Sprite::create("penaltyyBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel2 = Node::create();
    
    errorPanel2->setPosition(Vec2(0,0));
    
    this->addChild(errorPanel2);
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelBG, WIDTH*.8, HEIGHT*.8);
            
        }
        else {
            
            FIT_H(panelBG, HEIGHT*.8);
            
        }
        
    }
    else {
        
        FIT_H(panelBG, HEIGHT*.8);
        
    }
    
    panelBG->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel2->addChild(panelBG);
    
    //Label *errorLabel1 = Label::createWithTTF(st1, "chinese rocks rg.ttf", 45);
    
    //Label *errorLabel2 = Label::createWithTTF(st2, "chinese rocks rg.ttf", 45);
    
    //Label *errorLabel3 = Label::createWithTTF(st3, "chinese rocks rg.ttf", 45);
    
    Label *errorLabel1 = Label::createWithBMFont("menuFont.fnt", st1);
    
    Label *errorLabel2 = Label::createWithBMFont("menuFont.fnt", st2);
    
    Label *errorLabel3 = Label::createWithBMFont("menuFont.fnt", st3);
    
    FIT_H(errorLabel1, HEIGHT*.075);
    
    FIT_H(errorLabel2, HEIGHT*.075);
    
    FIT_H(errorLabel3, HEIGHT*.075);
    
    
    errorLabel1->setPosition(Vec2(WIDTH*.5,HEIGHT*.7));
    errorLabel2->setPosition(Vec2(WIDTH*.5,HEIGHT*.6));
    errorLabel3->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel2->addChild(errorLabel1);
    
    errorPanel2->addChild(errorLabel2);
    
    errorPanel2->addChild(errorLabel3);
    
    if(waitForRecover == false){
        
        waitForRecover = true;
        
        this->unscheduleRecover();
        
        this->schedule(schedule_selector(LoadingGameScene::goBackMainForError),5.0f);
        
    }
    
}

void LoadingGameScene::goBackMainForError(float dt){
    
    this->unschedule(schedule_selector(LoadingGameScene::goBackMainForError));
    
    //****//
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
    
    
}



void LoadingGameScene::getRoomWithProperties(std::map<std::string, std::string> tableProperty){
    
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->getRoomWithProperties(tableProperty);
    
}

void LoadingGameScene::onGetMatchedRoomsDone(AppWarp::matchedroom event){
    
    if(iWillQuit == false){
    
    if(event.roomData.size() == 0){
        
        printf("hiç oyuncu yok oyun kurulacak");
        
        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
        
        AppManager::getInstance()->isUserAdmin = true;
        
        std::map<std::string, std::string> tableProperty;
        
        tableProperty["roomName"] = roomName;
        tableProperty["roomPassword"] = roomPassword;
        tableProperty["levelForRoom"] = levelForRoom;
        tableProperty["goldForRoom"] = goldForRoom;
        tableProperty["minUser"] = StringUtils::format("%i",1);
        tableProperty["maxUser"] = StringUtils::format("%i",1);
        
        warpClientRef->createRoom(StringUtils::format("%s room", AppManager::getInstance()->username.c_str()), AppManager::getInstance()->username, 2, tableProperty);
        
        
        if(appManager->currentLanguage() == false){
            
            informStr = "Oyun Kuruluyor...";
            
        }
        else {
            
            informStr = "Game is being created...";
            
        }
        
        
        informLabel->setString(informStr);
        
    }
    else {
        
        
        for(int i=0; i<event.roomData.size(); i++){
            
            appManager->isUserAdmin = false;
            
            AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
            warpClientRef->joinRoom(event.roomData.at(i).roomId);
            break;
        
        }
        
    }
        
    }
    else {
        
        auto scene = MenuScene::createScene();
        
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));

        
    }
    
    
    
}

void LoadingGameScene::onCreateRoomDone(AppWarp::room event){
    
    if(event.result == 0){
        
        if(appManager->currentLanguage() == false){
            
            informStr = "Oyun kuruldu...";
            
        }
        else {
            
            informStr = "Game is created...";
            
        }
        
        
        informLabel->setString(informStr);
        
        appManager->myCurrentRoomId = event.roomId;
        
        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
        
        warpClientRef->joinRoom(event.roomId);
        
    }
    
}


void LoadingGameScene::onJoinRoomDone(AppWarp::room revent){
    
    backButton->setOpacity(255);
    
    if(revent.result == 0){
        
        printf("\nodaya katıldı");
        
        if(appManager->currentLanguage() == false){
            
            informStr = "Rakip bekleniyor..";
            
        }
        else {
            
            informStr = "Waiting for opponent...";
            
        }
        
        
        informLabel->setString(informStr);
        
        if(appManager->isUserAdmin == false){
            
            iamalone = false;
            
            this->sendMyAvatarNumberToOpponent(appManager->avatarNumber);

            appManager->opponentName = revent.owner;
            
            this->setNameForButtons(revent.owner, isimButonRakip);
            
        }
        
        
        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
        
        warpClientRef->subscribeRoom(revent.roomId);
        
    }
    else {
        
        std::map<std::string, std::string> tableProperty;
        
        tableProperty["roomName"] = roomName;
        tableProperty["roomPassword"] = roomPassword;
        tableProperty["levelForRoom"] = levelForRoom;
        tableProperty["goldForRoom"] = goldForRoom;
        tableProperty["minUser"] = StringUtils::format("%i",1);
        tableProperty["maxUser"] = StringUtils::format("%i",1);
        
        this->getRoomWithProperties(tableProperty);
        
        
    }
    
}

void LoadingGameScene::onSubscribeRoomDone(AppWarp::room revent){
    
    if(revent.result == 0){
        
        printf("\nsubscribe done\n");
        
        appManager->isOnline = true;
        
        if(AppManager::getInstance()->isUserAdmin == false){
            
            appManager->roomOwner = revent.owner;
            appManager->myCurrentRoomId = revent.roomId;
            
            if(iWillQuit == false){
            
                this->schedule(schedule_selector(LoadingGameScene::guestStartGame),6);
                
            }
            
        }
        
    }
    
}

void LoadingGameScene::onLeaveRoomDone(AppWarp::room event){
    
    auto scene = MenuScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));    
    
}


void LoadingGameScene::onUserJoinedRoom(AppWarp::room event, std::string username){
    
    if(appManager->sound == true){
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("matchmake.wav");
        
    }
    
    printf("%s has joined",username.c_str());
    
    
    if(appManager->currentLanguage() == false){
        
        informStr = StringUtils::format("%s oyuna katıldı",username.c_str());
        
    }
    else {
        
        informStr = StringUtils::format("%s has joined",username.c_str());
        
    }

    informLabel->setString(informStr);
    
    iamalone = false;
    
    appManager->opponentName = username;
    
    this->setNameForButtons(username, isimButonRakip);
    
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    
    std::map<std::string, std::string> tableProperty;
    std::vector<std::string> removes;
    
    tableProperty["roomName"] = roomName;
    tableProperty["roomPassword"] = roomPassword;
    tableProperty["levelForRoom"] = levelForRoom;
    tableProperty["goldForRoom"] = goldForRoom;
    tableProperty["minUser"] = StringUtils::format("%i",2);
    tableProperty["maxUser"] = StringUtils::format("%i",2);
    
    appManager->myCurrentRoomId = event.roomId;
    appManager->roomOwner = event.owner;
    
    warpClientRef->updateRoomProperties(event.roomId, tableProperty, removes);
    
    
}

void LoadingGameScene::onUpdatePropertyDone(AppWarp::liveroom event){
    
    if(event.result == 0){
        
        if(iWillQuit == false){
        
            this->schedule(schedule_selector(LoadingGameScene::adminStartGame), 5);
            
        }
        else if(iWillQuit == true){
            
            Scene *menuScene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1,menuScene));
            
        }
        
    }
    
}

void LoadingGameScene::adminStartGame(float dt) {
    
    this->unschedule(schedule_selector(LoadingGameScene::adminStartGame));
    
    if(iWillQuit == false){
    
        Scene *gameScene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1,gameScene));
        
    }
    
}

void LoadingGameScene::guestStartGame(float dt){
    
    this->unschedule(schedule_selector(LoadingGameScene::guestStartGame));
    
    if(iWillQuit == false){
    
        Scene *gameScene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1,gameScene));
        
    }

}

void LoadingGameScene::createAvatarForPlayer(int playerAvatarNumber){
    
    for(int i = 0; i < 15; i++){
        
        std::string avatarFileName = "avatar " + StringUtils::format("%d.png",(i));
        
        Sprite *avatarSprite = Sprite::create(avatarFileName);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(avatarSprite, HEIGHT*.34);
                
            }
            else {
                
                FIT_H(avatarSprite, HEIGHT*.34);
                
            }
            
            
        }
        else {
            
            FIT_H(avatarSprite, HEIGHT*.34);
            
        }
        
        
        
        AvatarsVector.pushBack(avatarSprite);
        
    }
    
    AvatarsVector.at(playerAvatarNumber)->setPosition(Vec2(avatarPlayer->getPositionX()*.98, avatarPlayer->getPositionY()*1.08));
    
   
    
    this->addChild(AvatarsVector.at(playerAvatarNumber));
    
    
}

void LoadingGameScene::createAvatarForOpponent(int opponentAvatarNumber){
    
    opponentFound = true;
    
    Vector<Sprite*>OpponentAvatarVector;
    
    for(int i=0; i < 15; i++){
        
        std::string avatarOpponentFile = "avatar " + StringUtils::format("%d.png",(i));
        
        Sprite *avatarOpponent = Sprite::create(avatarOpponentFile);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(avatarOpponent, HEIGHT*.34);
                
            }
            else {
                
                FIT_H(avatarOpponent, HEIGHT*.36);
                
            }
            
            
        }
        else {
            
            FIT_H(avatarOpponent, HEIGHT*.34);
            
        }
        
        
        
        OpponentAvatarVector.pushBack(avatarOpponent);
        
    }
    
    OpponentAvatarVector.at(opponentAvatarNumber)->setPosition(Vec2(avatarRakip->getPositionX(), avatarRakip->getPositionY()*1.08));
    
    appManager->rakipAvatarNumber = opponentAvatarNumber;
    
    this->addChild(OpponentAvatarVector.at(opponentAvatarNumber));
    
    if(appManager->isUserAdmin == true){
        
        this->sendMyAvatarNumberToOpponent(appManager->avatarNumber);
        
    }
    
}

void LoadingGameScene::sendMyAvatarNumberToOpponent(int avatarNumber){
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();
    
    //warpClientRef->sendChat(StringUtils::format("%d",avatarNumber));
    
    std::string updateStr = StringUtils::format("%d",avatarNumber);

    unsigned char *buff = new unsigned char[updateStr.length() + 1];
    strcpy((char*)buff, updateStr.c_str());

    warpClientRef->sendUpdate(buff, lengthOfU(buff));
    
}

void LoadingGameScene::onUpdatePeersReceived(AppWarp::byte *update, int len, bool isUDP){
    
    if(isOpponentAvatarReady == false){
        
        isOpponentAvatarReady = true;
        
        std::string msg((char*)update);
        
        this->createAvatarForOpponent(atoi(msg.c_str()));
        
    }
    
}


int LoadingGameScene::lengthOfU(unsigned char * str)
{
    int i = 0;
    
    while(*(str++)){
        i++;
        if(i == INT_MAX)
            return -1;
    }
    
    return i;
}


void LoadingGameScene::onChatReceived(AppWarp::chat chatevent){
    
    if(chatevent.sender != appManager->username){
        
        this->createAvatarForOpponent(atoi(chatevent.chat.c_str()));
        
    }
    
}

void LoadingGameScene::setScene(){
    
    background = Sprite::create("VSBG.png");
    FIT_WH(background, WIDTH, HEIGHT);
    background->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(background);
    
    oduller = Sprite::create("Oduller.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(oduller, HEIGHT*.12);
            
        }
        else {
            
            FIT_H(oduller, HEIGHT*.16);
            
        }
        
    }
    else {
        
        FIT_H(oduller, HEIGHT*.16);
        
    }
    
    
    
    oduller->setPosition(Vec2(WIDTH*.5, HEIGHT*.9));
    this->addChild(oduller);
    
    Label *odullerLabel;
    
    if(appManager->currentLanguage() == false){
    
        odullerLabel = Label::createWithTTF("ödüller", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        odullerLabel = Label::createWithTTF("rewards", "chinese rocks rg.ttf", 50);
        
    }
    
    odullerLabel->setColor(Color3B(34, 75, 122));
    
    odullerLabel->setPosition(Vec2(oduller->getContentSize().width/2, oduller->getContentSize().height/1.85));
    
    oduller->addChild(odullerLabel);
    
    odul1 = Sprite::create("Odul1.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(odul1, HEIGHT*.12);
            
        }
        else {
            
            FIT_H(odul1, HEIGHT*.14);
            
        }
        
        
    }
    else {
        
        FIT_H(odul1, HEIGHT*.14);
        
    }
    
    
    odul1->setPosition(Vec2(WIDTH*.41,HEIGHT*.76));
    //this->addChild(odul1);
    
    odul2 = Sprite::create("altinbitis.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(odul2, HEIGHT*.15);
            
        }
        else {
            
            FIT_H(odul2, HEIGHT*.15);
            
        }
        
    }
    else {
        
        FIT_H(odul2, HEIGHT*.15);
        
    }
    
    
    odul2->setPosition(Vec2(WIDTH*.5,HEIGHT*.76));
    this->addChild(odul2);
    
    auto goldLimitLabel = Label::createWithTTF(StringUtils::format("%.0f",appManager->limitGold), "chinese rocks rg.ttf", 60);
    
    goldLimitLabel->setColor(Color3B::WHITE);
    
    goldLimitLabel->enableShadow();
    
    goldLimitLabel->enableGlow(Color4B::BLACK);
    
    goldLimitLabel->setPosition(Vec2(odul2->getContentSize().width/1.8, odul2->getContentSize().height/2.1));
    
    odul2->addChild(goldLimitLabel);
    
    vs = Sprite::create("VS.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
               FIT_H(vs, HEIGHT*.35);
            
        }
        else {
            
            FIT_H(vs, HEIGHT*.4);
            
        }
        
        
    }
    else {
        
        FIT_H(vs, HEIGHT*.4);
        
    }
    
 
    vs->setPosition(Vec2(WIDTH*.5,HEIGHT*.46));
    this->addChild(vs);
    
    avatarPlayer = Sprite::create("AvatarVS.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(avatarPlayer, HEIGHT*.4);
            
        }
        else {
            
            FIT_H(avatarPlayer, HEIGHT*.52);
            
        }
        
    }
    else {
        
       FIT_H(avatarPlayer, HEIGHT*.52);
        
    }
    
    
    avatarPlayer->setPosition(Vec2(WIDTH*.2,HEIGHT*.5));
    this->addChild(avatarPlayer);
    
    avatarRakip = Sprite::create("AvatarVS.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(avatarRakip, HEIGHT*.4);
            
        }
        else {
            
            FIT_H(avatarRakip, HEIGHT*.52);
            
        }
        
    }
    else {
        
        FIT_H(avatarRakip, HEIGHT*.52);
        
    }
    
    avatarRakip->setPosition(Vec2(WIDTH*.8,HEIGHT*.5));
    this->addChild(avatarRakip);
    
    isimButonPlayer = Sprite::create("isimButon.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
           FIT_H(isimButonPlayer, HEIGHT*.17);
            
        }
        else {
            
            FIT_H(isimButonPlayer, HEIGHT*.2);
            
        }
        
    }
    else {
       
        FIT_H(isimButonPlayer, HEIGHT*.2);
        
    }
    
    
    isimButonPlayer->setPosition(Vec2(WIDTH*.2,HEIGHT*.15));
    this->addChild(isimButonPlayer);
    
    isimButonRakip = Sprite::create("isimButon.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(isimButonRakip, HEIGHT*.17);
            
        }
        else {
            
            FIT_H(isimButonRakip, HEIGHT*.2);
            
        }
        
    }
    else {
        
        FIT_H(isimButonRakip, HEIGHT*.2);
        
    }
    isimButonRakip->setPosition(Vec2(WIDTH*.8,HEIGHT*.15));
    this->addChild(isimButonRakip);
    
    
    backButton = Sprite::create("backButton.png");
    FIT_H(backButton, HEIGHT*.1);
    backButton->setPosition(Vec2(WIDTH*.07,HEIGHT*.86));
    backButton->setOpacity(0);
    this->addChild(backButton);
    
    this->setNameForButtons(appManager->username, isimButonPlayer);

    
}

void LoadingGameScene::setNameForButtons(std::string name, cocos2d::Sprite *isimButon){
    
    if(appManager->username != name){
    
        if(butName == NULL){
    
            butName = Label::createWithTTF(name, "chinese rocks rg.ttf", 50);
    
            butName->setPosition(Vec2(isimButon->getPositionX(), isimButon->getPositionY() - HEIGHT*.005));
        
            this->addChild(butName);
        
        }
        else {
        
            butName->setString(name);
        
        }
        
    }
    else {
        
        if(userNameBut == NULL){
            
            userNameBut = Label::createWithTTF(name, "chinese rocks rg.ttf", 50);
            
            userNameBut->setPosition(Vec2(isimButon->getPositionX(), isimButon->getPositionY() - HEIGHT*.005));
            
            this->addChild(userNameBut);
            
        }
        else {
            
            userNameBut->setString(name);
            
        }
        
    }
    
    
}

void LoadingGameScene::onEnterTransitionDidFinish() {
    
    Layer::onEnterTransitionDidFinish();
    
    this->schedule(schedule_selector(LoadingGameScene::update));
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(LoadingGameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LoadingGameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LoadingGameScene::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    
}

void LoadingGameScene::update(float dt){
    
    if(opponentFound == false){
        
        countChangeAvatar ++;
        
        if(countChangeAvatar >= 10){
            
            countChangeAvatar = 0;
            
            
            if(countAvatarNumber == 0){
                
                std::string avatarOpponentFile = "avatar " + StringUtils::format("%d.png",(countAvatarNumber));
                
                avatarChangeSprite = Sprite::create(avatarOpponentFile);
                
                if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
                    
                    if(appManager->isIpad() == true){
                        
                        FIT_H(avatarChangeSprite, HEIGHT*.34);
                        
                    }
                    else {
                        
                        FIT_H(avatarChangeSprite, HEIGHT*.36);
                        
                    }
                    
                    
                }
                else {
                    
                    FIT_H(avatarChangeSprite, HEIGHT*.34);
                    
                }
            
                avatarChangeSprite->setPosition(Vec2(avatarRakip->getPositionX(), avatarRakip->getPositionY()*1.08));
                
            
                this->addChild(avatarChangeSprite);
                
                countAvatarNumber ++;
                
                
            }
            else {
                
                if(countAvatarNumber < 15){
                    
                    std::string avatarOpponentFile = "avatar " + StringUtils::format("%d.png", (countAvatarNumber));
                    
                    avatarChangeSprite->setTexture(avatarOpponentFile);
                    
                    countAvatarNumber++;
                    
                }
                else {
                    
                    countAvatarNumber = 0;
                    
                }
                
                
            }
            
            
            
        }
        
        
    }
    
}
