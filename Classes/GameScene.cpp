//
//  GameScene.cpp
//  Santra
//
//  Created by Anıl Gülgör on 16/12/15.
//
//

#include "GameScene.hpp"
#include "time.h"

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GameScene::init(){
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        this->setKeypadEnabled(true);
        
    }
    
    WIDTH = GET_WIDTH;
    HEIGHT = GET_HEIGHT;
    
    //background music
    
    appManager = AppManager::getInstance();
    
    if(appManager->sound == true){
    
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("oyunicimuzik.mp3",true);
        
    }
    else {
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("oyunicimuzik.mp3",true);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();

        
    }
    
    winState = false;
    
    timeOfGame = 0;
    
    
    
    setAllFootballers();

    setGameTypeWithIdentifier(appManager->kindOfGame);
    
    isAyarlarOpen = false;
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
        
    
    return true;
    
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event){
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
       /* UserDefault *userDefaults = UserDefault::getInstance();
        
        userDefaults->setIntegerForKey("sessionID", NULL);
        
        userDefaults->setIntegerForKey("tempSessionID", NULL);
        
        userDefaults->flush();
        
        AppWarp::Client *warpClientRef;
        
        warpClientRef = AppWarp::Client::getInstance();
        
        warpClientRef->leaveRoom(appManager->myCurrentRoomId);
        
        warpClientRef->disconnect();
        
        Director::getInstance()->end();*/
        
        this->callQuitKeyAction();
        
    }
    
}



void GameScene::onConnectDone(int res, int reasonCode){
    
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
void GameScene::recover(float dt)
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
        
        this->schedule(schedule_selector(GameScene::controlForConnection), 8.0f);
        
    }
    
}

void GameScene::controlForConnection(float dt){
    
    this->unschedule(schedule_selector(GameScene::controlForConnection));
    
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


void GameScene::removeNoConnectionWarning(){
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel);
    
}


void GameScene::noConnectionWarning(std::string st1, std::string st2, std::string st3){
    
    ayarlarPerde = Sprite::create("penaltyyBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0,0));
    
    this->addChild(errorPanel);
    
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
    
    errorPanel->addChild(panelBG);
    
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
    
    errorPanel->addChild(errorLabel1);
    
    errorPanel->addChild(errorLabel2);
    
    errorPanel->addChild(errorLabel3);
    
    if(waitForRecover == false){
        
        waitForRecover = true;
        
        this->unscheduleRecover();
        
        this->schedule(schedule_selector(GameScene::goBackMainForError),5.0f);
        
    }
    
}

void GameScene::goBackMainForError(float dt){
    
    this->unschedule(schedule_selector(MenuScene::goBackMainForError));
    
    //****//
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
    
    
}

void GameScene::scheduleRecover()
{
    printf("\nGameScene::scheduleRecover");
    this->schedule(schedule_selector(GameScene::recover), 1.0f);
    
    //showReconnectingLayer("Reconnecting ...");
}

void GameScene::unscheduleRecover()
{
    printf("\nGameScene::unscheduleRecover");
    this->unschedule(schedule_selector(GameScene::recover));
}


void GameScene::setAvatarsOlumMaci(std::string playerName, std::string opponentName){
    
    opponentName = "CPU";
    
    appManager->opponentName = opponentName;
    
    avatarPlayer = Sprite::create("AvatarVS.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(avatarPlayer, HEIGHT*.21);
            
        }
        else {
            
            FIT_H(avatarPlayer, HEIGHT*.25);
            
        }

        
    }
    else {
    
        FIT_H(avatarPlayer, HEIGHT*.25);
        
    }
    avatarPlayer->setPosition(Vec2(WIDTH*.93,HEIGHT*.25));
    this->addChild(avatarPlayer);
    
    
    Vector<Sprite*>PlayerAvatarVector;
    
    for(int i = 0; i < 15; i++){
        
        std::string avatarPlayerFile = "avatar " + StringUtils::format("%d.png",(i));
        
        Sprite *avataruser = Sprite::create(avatarPlayerFile);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(avataruser, HEIGHT*.16);
                
            }
            else {
                
                FIT_H(avataruser, HEIGHT*.19);
                
            }
            
        }
        else {
        
            FIT_H(avataruser, HEIGHT*.19);
            
        }
        
        PlayerAvatarVector.pushBack(avataruser);
        
    }
    
    PlayerAvatarVector.at(appManager->avatarNumber)->setPosition(Vec2(avatarPlayer->getPositionX(),avatarPlayer->getPositionY() + HEIGHT*.02));
    this->addChild(PlayerAvatarVector.at(appManager->avatarNumber));
    
    avatarOpponent = Sprite::create("AvatarVS.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(avatarOpponent, HEIGHT*.21);
            
        }
        else {
            
            FIT_H(avatarOpponent, HEIGHT*.25);
            
        }
        
    }
    else {
        
        FIT_H(avatarOpponent, HEIGHT*.25);
        
    }
    avatarOpponent->setPosition(Vec2(WIDTH*.93,HEIGHT*.75));
    this->addChild(avatarOpponent);
    

    Sprite *robotAvatar = Sprite::create("avatar robot.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(robotAvatar, HEIGHT*.16);
            
        }
        else {
            
            FIT_H(robotAvatar, HEIGHT*.19);
            
        }
        
    }
    else {
        
        FIT_H(robotAvatar, HEIGHT*.19);
        
    }
    
    robotAvatar->setPosition(Vec2(avatarOpponent->getPositionX(), avatarOpponent->getPositionY() + HEIGHT*.02));
    this->addChild(robotAvatar);
    
    
    playerNameLabel = Label::createWithTTF(playerName, "chinese rocks rg.ttf", 35);
    playerNameLabel->setColor(Color3B(255, 255, 255));
    playerNameLabel->enableGlow(Color4B(0, 0, 0, 255));
    playerNameLabel->setPosition(Vec2(WIDTH*.93, HEIGHT*.1));
    this->addChild(playerNameLabel);
    
    opponentNameLabel = Label::createWithTTF(opponentName, "chinese rocks rg.ttf", 35);
    opponentNameLabel->setColor(Color3B(255, 255, 255));
    opponentNameLabel->enableGlow(Color4B(0, 0, 0, 255));
    opponentNameLabel->setPosition(Vec2(WIDTH*.93, HEIGHT*.9));
    this->addChild(opponentNameLabel);

    
}

void GameScene::setAvatars(std::string playerName, std::string opponentName) {
    
    avatarPlayer = Sprite::create("AvatarVS.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(avatarPlayer, HEIGHT*.21);
            
        }
        else {
            
            FIT_H(avatarPlayer, HEIGHT*.25);
            
        }
        
    }
    else {
        
        FIT_H(avatarPlayer, HEIGHT*.25);
        
    }
    
    avatarPlayer->setPosition(Vec2(WIDTH*.93,HEIGHT*.25));
    this->addChild(avatarPlayer);
    
    
    Vector<Sprite*>PlayerAvatarVector;
    
    for(int i = 0; i < 15; i++){
        
        std::string avatarPlayerFile = "avatar " + StringUtils::format("%d.png",(i));
        
        Sprite *avataruser = Sprite::create(avatarPlayerFile);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(avataruser, HEIGHT*.16);
                
            }
            else {
                
                FIT_H(avataruser, HEIGHT*.19);
                
            }
            
        }
        else {
            
            FIT_H(avataruser, HEIGHT*.19);
            
        }
        
        
        PlayerAvatarVector.pushBack(avataruser);
        
    }
    
    PlayerAvatarVector.at(appManager->avatarNumber)->setPosition(Vec2(avatarPlayer->getPositionX(),avatarPlayer->getPositionY() + HEIGHT*.02));
    this->addChild(PlayerAvatarVector.at(appManager->avatarNumber));
    
    
    avatarOpponent = Sprite::create("AvatarVS.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(avatarOpponent, HEIGHT*.21);
            
        }
        else {
            
            FIT_H(avatarOpponent, HEIGHT*.25);
            
        }
        
    }
    else {
        
        FIT_H(avatarOpponent, HEIGHT*.25);
        
    }
    
    avatarOpponent->setPosition(Vec2(WIDTH*.93,HEIGHT*.75));
    this->addChild(avatarOpponent);
    
    //opponent avatar
    
    Vector<Sprite*>OpponentAvatarVector;
    
    for(int i=0; i < 15; i++){
        
        std::string avatarOpponentFile = "avatar " + StringUtils::format("%d.png",(i));
        
        Sprite *avatarOpponent = Sprite::create(avatarOpponentFile);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(avatarOpponent, HEIGHT*.16);
                
            }
            else {
                
                FIT_H(avatarOpponent, HEIGHT*.19);
                
            }
            
        }
        else {
            
            FIT_H(avatarOpponent, HEIGHT*.19);
            
        }
        
        OpponentAvatarVector.pushBack(avatarOpponent);
        
    }
    
    int rakipAvatarNumber = 0;
   
        
    rakipAvatarNumber = appManager->rakipAvatarNumber;
        
    
        
    
    OpponentAvatarVector.at(rakipAvatarNumber)->setPosition(Vec2(avatarOpponent->getPositionX(), avatarOpponent->getPositionY() + HEIGHT*.02));
    
    this->addChild(OpponentAvatarVector.at(appManager->rakipAvatarNumber));

    
    //playerName = "anilgulgor";
    
    playerNameLabel = Label::createWithTTF(playerName, "chinese rocks rg.ttf", HEIGHT*.037);
    playerNameLabel->setColor(Color3B(255, 255, 255));
    playerNameLabel->enableGlow(Color4B(0, 0, 0, 255));
    playerNameLabel->setPosition(Vec2(WIDTH*.93, HEIGHT*.1));
    this->addChild(playerNameLabel);
    
    opponentNameLabel = Label::createWithTTF(opponentName, "chinese rocks rg.ttf", HEIGHT*.037);
    opponentNameLabel->setColor(Color3B(255, 255, 255));
    opponentNameLabel->enableGlow(Color4B(0, 0, 0, 255));
    opponentNameLabel->setPosition(Vec2(WIDTH*.93, HEIGHT*.9));
    this->addChild(opponentNameLabel);
    
}

void GameScene::setSure(){
    
    playerCurrentScore = 0;
    opponentCurrentScore = 0;
    
    surePanel = Sprite::create("Sure.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(surePanel, HEIGHT*.14);
            
        }
        else {
            
            FIT_H(surePanel, HEIGHT*.15);
            
        }
        
    }
    else {
        
        FIT_H(surePanel, HEIGHT*.15);
        
    }
    
    surePanel->setPosition(Vec2(WIDTH*.93,HEIGHT*.5));
    this->addChild(surePanel);
    
    sureLabel = Label::createWithTTF(StringUtils::format("%d",timeOfGame), "DS-DIGI.TTF", 90);
    surePanel->addChild(sureLabel);
    sureLabel->setPosition(Vec2(surePanel->getContentSize().width/2,surePanel->getContentSize().height/2));
    sureLabel->enableGlow(Color4B::GREEN);
    
    //playerScore
    
    if(appManager->currentLanguage() == false){
    
        playerScoreLabel = Label::createWithTTF(StringUtils::format("Skor:%d",int(playerCurrentScore)), "chinese rocks rg.ttf", HEIGHT*.04);
        
    }
    else {
        
        playerScoreLabel = Label::createWithTTF(StringUtils::format("Score:%d", int(playerCurrentScore)),"chinese rocks rg.ttf" , HEIGHT*.04);
        
    }
    
    playerScoreLabel->setPosition(Vec2(surePanel->getPositionX(), surePanel->getPositionY() - HEIGHT*.1));
    
    playerScoreLabel->enableGlow(Color4B::GREEN);
    
    this->addChild(playerScoreLabel);
    
    //opponentScore
    
    if(appManager->currentLanguage() == false){
    
        opponentScoreLabel = Label::createWithTTF(StringUtils::format("Skor:%d",int(opponentCurrentScore)), "chinese rocks rg.ttf", HEIGHT*.04);
    
    }
    else {
        
        opponentScoreLabel = Label::createWithTTF(StringUtils::format("Score:%d",int(opponentCurrentScore)), "chinese rocks rg.ttf", HEIGHT*.04);

        
    }
    
    opponentScoreLabel->setPosition(Vec2(surePanel->getPositionX(), surePanel->getPositionY() + HEIGHT*.1));
    
    opponentScoreLabel->enableGlow(Color4B::GREEN);
    
    this->addChild(opponentScoreLabel);
    
}

void GameScene::updateScoreLabels(float playersc, float opponentsc){
    
    if(appManager->currentLanguage() == false){
    
        playerScoreLabel->setString(StringUtils::format("Skor:%d", int(playersc)));
    
        opponentScoreLabel->setString(StringUtils::format("Skor:%d", int(opponentsc)));
        
    }
    else {
        
        playerScoreLabel->setString(StringUtils::format("Score:%d", int(playersc)));
        
        opponentScoreLabel->setString(StringUtils::format("Score:%d", int(opponentsc)));

    }
    
}

void GameScene::setGameTypeWithIdentifier(std::string kindOfGame){
    
    desteDagitiliyor = true;
    
    if(kindOfGame.compare("olumMaci") == 0){
        
        //olumMaci
        actOfGame = 0;
        backGround = Sprite::create("olumMaciBG.png");
        FIT_WH(backGround, WIDTH, HEIGHT);
        backGround->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
        this->addChild(backGround);
        
        setOlumMaci();
        
    }
    else {
        
        actOfGame = 1;
        backGround = Sprite::create("sahaBG.png");
        FIT_WH(backGround, WIDTH, HEIGHT);
        backGround->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
        this->addChild(backGround);
        
        setMac();
        createImlec(AppManager::getInstance()->isUserAdmin);
        
    }
    
    setAyarlarButton();
    setMyInventory();
    
}

void GameScene::openWarningPanel(std::string warning){
    
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(errorPanel, 100);
    
    Sprite *panel = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panel, WIDTH*.9, HEIGHT*.9);
            
        }
        else {
            
            FIT_H(panel, HEIGHT*.9);
            
        }
        
    }
    else {
        
        FIT_H(panel, HEIGHT*.9);
        
    }
    
    errorPanel->addChild(panel,100);
    
    panel->setPosition(Vec2(0,0));
    
    Label *warnLabel = Label::createWithBMFont("menuFont.fnt", warning);
    
    FIT_H(warnLabel, HEIGHT*.1);
    
    warnLabel->setPosition(Vec2(0,0));
    
    errorPanel->addChild(warnLabel,100);
    
    this->schedule(schedule_selector(GameScene::closeWarningSchedule), 3);
    
}


void GameScene::closeWarningSchedule(float dt){
    

    this->unschedule(schedule_selector(GameScene::closeWarningSchedule));
    
    this->removeChild(errorPanel);
    
}


void GameScene::setAyarlarButton(){
    
    ayarlarNode = Node::create();
    this->addChild(ayarlarNode);
    ayarlarNode->setPosition(Vec2(-WIDTH*.009,HEIGHT*.2));
    
    ayarlarNode->setZOrder(4);
    
    ayarlarPanel = Sprite::create("Ayarlar Panel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(ayarlarPanel, HEIGHT*.4);
            
        }
        else {
            
            FIT_H(ayarlarPanel, HEIGHT*.43);
            
        }
        
    }
    else {
        
        FIT_H(ayarlarPanel, HEIGHT*.43);
        
    }
    
    ayarlarNode->addChild(ayarlarPanel);
    
    ayarlarBut = Sprite::create("AyarlarOyun.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(ayarlarBut, HEIGHT*.16);
            
        }
        else {
            
            FIT_H(ayarlarBut, HEIGHT*.18);
            
        }
        
        
    }
    else {
        
        FIT_H(ayarlarBut, HEIGHT*.18);
        
    }
    
    ayarlarBut->setPosition(Vec2(WIDTH*.05,-HEIGHT*.118));
    ayarlarNode->addChild(ayarlarBut);
    
    quitGameButton = Sprite::create("Cikis.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(quitGameButton, HEIGHT*.2);
            
        }
        else {
            
            FIT_H(quitGameButton, HEIGHT*.2);
            
        }
        
    }
    else {
        
        FIT_H(quitGameButton, HEIGHT*.2);
        
    }
    
    
    quitGameButton->setPosition(Vec2(-WIDTH*.04,-HEIGHT*.120));
    ayarlarNode->addChild(quitGameButton);
    
    if(appManager->fx == false){
        
        soundButton = Sprite::create("Ses Ac Kapa Kapali.png");
        
    }
    else {
    
        soundButton = Sprite::create("Ses Ac Kapa.png");
        
    }
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(soundButton, HEIGHT*.2);
            
        }
        else {
            
            FIT_H(soundButton, HEIGHT*.2);
            
        }
        
    }
    else {
        
        FIT_H(soundButton, HEIGHT*.2);
        
    }
    
    soundButton->setPosition(Vec2(-WIDTH*.04,0));
    ayarlarNode->addChild(soundButton);
    
    if(appManager->sound == false){
        
        musicButton = Sprite::create("Muzik Ac Kapa Kapali.png");
        
    }
    else {
    
        musicButton = Sprite::create("Muzik Ac Kapa.png");
        
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(musicButton, HEIGHT*.2);
            
        }
        else {
            
            FIT_H(musicButton, HEIGHT*.2);
            
        }
        
    }
    else {
        
        FIT_H(musicButton, HEIGHT*.2);
        
    }
    
    musicButton->setPosition(Vec2(-WIDTH*.04,HEIGHT*.120));
    ayarlarNode->addChild(musicButton);
    
    
}

void GameScene::openAyarlarMenu(bool isOpen){
    
    if(isOpen == false){
        
        auto anim = MoveTo::create(.5, Vec2(WIDTH*.07,ayarlarNode->getPositionY()));
        auto ease = EaseElasticInOut::create(anim, .5);
        
        ayarlarNode->runAction(ease);
        
        isAyarlarOpen = true;
        
    }
    else if(isOpen == true){
        
        auto anim = MoveTo::create(.5, Vec2(-WIDTH*.009,ayarlarNode->getPositionY()));
        auto ease = EaseElasticInOut::create(anim, .5);
        
        ayarlarNode->runAction(ease);
        
        isAyarlarOpen = false;

        
    }
    
    
}

void GameScene::setMyInventory(){
    
    
    
}

void GameScene::setAllFootballers() {
    
    footballer = Footballer::create("Joker", "TURKIYE", 100);
    footballerVector.pushBack(footballer);
    
    //efsaneler
    footballer = Footballer::create("Pelle","BREZILYA", 97);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Maridonna", "ARJANTIN", 96);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("R9", "BREZILYA", 95);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Zindane", "FRANSA", 95);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Ronaldinyo", "BREZILYA", 94); // isim güç
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Backhem", "INGILTERE", 94);
    footballerVector.pushBack(footballer);
    
    //aslar gold
    footballer = Footballer::create("Nessi", "ARJANTIN", 93);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Rinaldo", "PORTEKIZ", 93);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Robbin", "HOLLANDA", 90);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Z.Egovic", "ISVEC", 90);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Sauraz", "URUGUAY", 89);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Riberinstein", "FRANSA", 88);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Hizart", "BELCIKA", 88);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Levangolski", "POLONYA", 87);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Hell Beyl", "GALLER", 87);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Medrac", "HIRVATISTAN", 87);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Rodrigo", "KOLOMBIYA", 86);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Agustero", "ARJANTIN" , 86);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("M.Ozul","ALMANYA", 86);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Rues", "ALMANYA", 86);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Niymor", "BREZILYA", 86);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Benzeya", "FRANSA", 85);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Hope Birlon", "ITALYA", 85);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Ardea Touran", "TURKIYE", 84);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Pagbo", "FRANSA", 86);
    footballerVector.pushBack(footballer);
    
    //aslar silver
    footballer = Footballer::create("Daglos Kasto", "BREZILYA", 81); // isim
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Shrooney", "INGILTERE", 84);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Diergo Kostok", "ISPANYA", 84);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Fan Porsi", "HOLLANDA" , 80);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Sniper", "HOLLANDA", 84);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Halk", "BREZILYA", 84);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Abomeygang","GABON", 84);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Sanchertez", "SILI", 84);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Oska", "BREZILYA", 84);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Rocketic", "HIRVATISTAN", 83);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Tiviz", "ARJANTIN", 83);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Di Marina", "ARJANTIN", 83);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Baloelli", "ITALYA", 82);
    footballerVector.pushBack(footballer);
    
    
    //aslar bronz
    footballer = Footballer::create("Defoy", "HOLLANDA", 80);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("M.Gimes", "ALMANYA", 80);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Walback", "INGILTERE", 80);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Nano", "PORTEKIZ", 80);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Ato", "KAMERUN", 80);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Gervirinyo", "FILDISI", 79);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Sambrly", "INGILTERE", 79);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Kardazzo", "PARAGUAY", 78);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Cikirito", "MEKSIKA" , 77);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Gariod", "FRANSA", 77);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Martinal", "FRANSA" , 76);
    footballerVector.pushBack(footballer);
    footballer = Footballer::create("Fellaini", "BELCIKA" , 79); //isim
    footballerVector.pushBack(footballer);
    
    
}

void GameScene::createDeck(int kartNum){
    
    srand(time(NULL));

    for(int i= 0; i<kartNum; i++){
        
        kart = Kart::create();
        
        kart->drawKartWith(footballerVector.at(i)->getUlke(), footballerVector.at(i)->getName(), footballerVector.at(i)->getPower());
        
        kart->setPosition(Vec2(-WIDTH*.4,0));
        
        deckVec.pushBack(kart);
        
    }
    
}


void GameScene::createPlayer(){
    
    Vector<Kart*>playerKarts;
    
    for(int i=0; i<5; i++){
        
        Kart *pkart = deckVec.getRandomObject();
        playerKarts.pushBack(pkart);
        deckVec.eraseObject(pkart);
        
    }
    
    player = Player::create(AppManager::getInstance()->username, playerKarts, AppManager::getInstance()->isUserAdmin);
    
    if(player->getIsAdmin() == true){
        
        letMeThrow = true;
        
    }
    else {
        
        letMeThrow = false;
        
    }
    
    this->showPlayerCardsOnScreen(player);
    
    this->createDesteForOnline();
    
    //detect that player is admin or not.
    
    isMyTurn = player->getIsAdmin();
    
    if(isMyTurn == true){
        
         this->startThrowCounter();
        
    }
    
}

void GameScene::createImlec(bool isMyTurn){
    
    imlec = ImlecNode::create("okimlec.png");
    
    imlec->addImageToNode();
    
    imlec->setZOrder(2);
    
    this->addChild(imlec);
    
    if(isMyTurn == true){
    
        imlec->setPosition(Vec2(WIDTH*.1,HEIGHT*.15));
        
    }
    else {
        
        imlec->setPosition(Vec2(WIDTH*.1,HEIGHT*.85));
        
    }
    
}

void GameScene::createDesteForOnline() {
  
    for(int i = 0; i < 50; i ++){
        
        Sprite *unplayedDesteKart = Sprite::create("KartArka.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(unplayedDesteKart, HEIGHT*.27);
                
            }
            else {
                
                FIT_H(unplayedDesteKart, HEIGHT*.32);
                
            }
        }
        else {
            
            FIT_H(unplayedDesteKart, HEIGHT*.32);
            
        }
        
        int rn = rand()%2;
        
        if(rn == 0){
            
            float kartAngle = rand()%10 + 10;
            
            unplayedDesteKart->setRotation(kartAngle);
            unplayedDesteKart->setPosition(Vec2(WIDTH*.08, HEIGHT*.58));
            desteCardOnline.pushBack(unplayedDesteKart);
            unplayedDesteKart->setZOrder(5);
            this->addChild(unplayedDesteKart);
            
        }
        else{
            
            float kartAngle = rand()%10 + 340;
            
            unplayedDesteKart->setRotation(kartAngle);
            unplayedDesteKart->setPosition(Vec2(WIDTH*.08, HEIGHT*.58));
            desteCardOnline.pushBack(unplayedDesteKart);
            unplayedDesteKart->setZOrder(5);
            this->addChild(unplayedDesteKart);
            
        }
        
    }

}


void GameScene::createPlayerForDeathMatch() {
    
    for(int i=0; i < 20; i++){
        
        Sprite *unplayedDesteKart = Sprite::create("KartArka.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(unplayedDesteKart, HEIGHT*.27);
                
            }
            else {
                
                FIT_H(unplayedDesteKart, HEIGHT*.32);
                
            }
        }
        else {
            
            FIT_H(unplayedDesteKart, HEIGHT*.32);
            
        }
        
        int rn = rand()%2;
        
        if(rn == 0){
            
            float kartAngle = rand()%25 + 20;
            
            unplayedDesteKart->setRotation(kartAngle);
            unplayedDesteKart->setPosition(Vec2(WIDTH*.15, HEIGHT*.5));
            this->addChild(unplayedDesteKart);
            
        }
        else{
            
            float kartAngle = rand()%25 + 270;
            
            unplayedDesteKart->setRotation(kartAngle);
            unplayedDesteKart->setPosition(Vec2(WIDTH*.15, HEIGHT*.5));
            this->addChild(unplayedDesteKart);
            
        }
        
    }
    
    
    for(int i=0; i < 3; i++){
        
        Kart *pkart = deckVec.getRandomObject();
        playerKarts.pushBack(pkart);
        deckVec.eraseObject(pkart);
        
        //draw unplayed kart
        
        Sprite *unplayedKart = Sprite::create("KartArka.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(unplayedKart, HEIGHT*.25);
                
            }
            else {
                
                FIT_H(unplayedKart, HEIGHT*.3);
                
            }
        }
        else {
            
            FIT_H(unplayedKart, HEIGHT*.3);
            
        }
        
        int rn = rand()%2;
        
        if(rn == 0){
            
            float kartAngle = rand()%25;
            
            unplayedKart->setRotation(kartAngle);
            
        }
        else{
            
            float kartAngle = rand()%25 + 270;
            
            unplayedKart->setRotation(kartAngle);
            
        }

        
        
        deathMatchCardArray.pushBack(unplayedKart);
        
        olumMaciDesteCards.pushBack(unplayedKart);
        
        //unplayedKart->setPosition(Vec2(WIDTH*.35 + (i)*WIDTH*.15, HEIGHT*.15));
        
        unplayedKart->setPosition(Vec2(WIDTH*.15, HEIGHT*.5));
        
        this->addChild(unplayedKart);
        
    }
    
    player = Player::create(AppManager::getInstance()->username, playerKarts, AppManager::getInstance()->isUserAdmin);
    
}

void GameScene::showOpponentCardsForDeathMatch() {
    
    
    for (int i = 0; i < 3; i++){
        
        Kart *cpuKart = deckVec.getRandomObject();
        opponentKarts.pushBack(cpuKart);
        deckVec.eraseObject(cpuKart);
        
        //draw
        
        Sprite *unplayedKart = Sprite::create("KartArka.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(unplayedKart, HEIGHT*.26);
                
            }
            else {
                
                FIT_H(unplayedKart, HEIGHT*.3);
                
            }
            
        }
        else {
            
            FIT_H(unplayedKart, HEIGHT*.3);
            
        }
        
        int rn = rand()%2;
        
        if(rn == 0){
            
            float kartAngle = rand()%25;
            
            unplayedKart->setRotation(kartAngle);
            
        }
        else{
            
            float kartAngle = rand()%25 + 270;
            
            unplayedKart->setRotation(kartAngle);
            
        }

        
        
        deathMathchOpponentArray.pushBack(unplayedKart);
        
        olumMaciDesteCards.pushBack(unplayedKart);
        
        unplayedKart->setPosition(Vec2(WIDTH*.15,HEIGHT*.5));
        
        //unplayedKart->setPosition(Vec2(WIDTH*.35 + (i)*WIDTH*.15, HEIGHT*.85));
        
        this->addChild(unplayedKart);
        
    }
    
    
    opponent = Opponent::create("CPU", opponentKarts);
    
}

void GameScene::createOpponent(){
    
    Vector<Kart*>opponentKarts;
    
    for(int i=0 ; i<5; i++){
        
        Kart *dusmanKarti = Kart::create();
        opponentKarts.pushBack(dusmanKarti);
        dusmanKarti->drawUnplayedKart();
        dusmanKarti->setPosition(Vec2(WIDTH/4.8 + WIDTH*.2*i, HEIGHT*.9));
        
    }
    
    opponent = Opponent::create("opponent", opponentKarts);
    this->showOpponentCardsOnScreen(opponent);
    
}

int GameScene::getLowestKartWithOrder(){
    
    int minKart, order;
    
    for(int i = 0; i < player->getPlayerKarts().size(); i++){
        
        if(i < player->getPlayerKarts().size() - 1){
            
            if(i == 0){
                
                minKart = player->getPlayerKarts().at(i)->getKartPower();
                
                order = i;
                
            }
            
            if(minKart > player->getPlayerKarts().at(i+1)->getKartPower()){
                
                minKart = player->getPlayerKarts().at(i+1)->getKartPower();
                
                order = i + 1;
                
                
            }
            
            
        }
        
        
    }
    
   
    
    return order;
    
}

void GameScene::startThrowCounter(){
    
    
    /*this->schedule([=](float delta){
        
    }, 0.1f, "updateLoadingBar");*/
    
    canThrow = true;
    
    sureBos = Sprite::create("barbos.png");
    
    sureDolu = Sprite::create("bardol.png");
    
    FIT_H(sureBos, HEIGHT*.38);
    
    FIT_H(sureDolu, HEIGHT*.38);
    
    sureBos->setPosition(Vec2(WIDTH*.14, HEIGHT*.18));
    
    //sureBos->setPosition(Vec2(WIDTH*.14, HEIGHT*.18));
    
    sureBos->setZOrder(10);
    
    this->addChild(sureBos);
    
    //*** progress

    progressTimer = ProgressTimer::create(sureDolu);
    
    auto progressTo = ProgressFromTo::create(20.0f, 0.0f, 100.0f);
    
    FIT_H(progressTimer, HEIGHT*.38);
    
    progressTimer->setPosition(Vec2(WIDTH*.14, HEIGHT*.18));
    
    this->addChild(progressTimer, 11);
    
    progressTimer->setType(cocos2d::ProgressTimer::Type::BAR);
    
    progressTimer->setBarChangeRate(Vec2(0.5, 1));
    
    progressTimer->setMidpoint(Vec2(0.5, 0));
    
    //*** anim
    
    auto callBack = CallFunc::create([&]{
        
        //throwcard min
        
        canThrow = false;
        
        CCLOG("throw card");
        
        if(appManager->fx == true){
        
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
        }
        
        controlCard = Kart::create();
        
        int order = getLowestKartWithOrder();
        
        controlCard = player->getPlayerKarts().at(order);
        
        cardPool.pushBack(controlCard);
        
        sendDataToOpponent(player->getPlayerKarts().at(order), order);
        
        playKart(player->getPlayerKarts().at(order));
        
        this->removeChild(sureBos);
        
        this->removeChild(progressTimer);
        
        if(cardPool.size() == 2){
            
            this->removeSignalsOnCards();
            
            if(cardPool.at(0)->getKartPower() < cardPool.at(1)->getKartPower()){
                
                printf("kazandım");
                
                playerCurrentScore += cardPool.at(0)->getKartPower() + cardPool.at(1)->getKartPower();
                
                this->updateScoreLabels(playerCurrentScore, opponentCurrentScore);
                
                this->schedule(schedule_selector(GameScene::wonPerThrow),2.5);
                
                this->startThrowCounter();
                
            }
            else if(cardPool.at(0)->getKartPower() == cardPool.at(1)->getKartPower() || cardPool.at(0)->getUlke().c_str() == cardPool.at(1)->getUlke().c_str()){
                
                printf("beraber");
                this->schedule(schedule_selector(GameScene::penalties),1);
                
            }
            else {
                
                printf("kaybettim");
                
                opponentCurrentScore += cardPool.at(0)->getKartPower() +cardPool.at(1)->getKartPower();
                
                this->updateScoreLabels(playerCurrentScore, opponentCurrentScore);
                
                
                this->schedule(schedule_selector(GameScene::losePerThrow),2.5);
                
                this->startThrowCounter();
                
            }
            
            
            
            
        }
        else {
            
            AppManager::getInstance()->isUserAdmin = false;
            
            player->setIsAdmin(AppManager::getInstance()->isUserAdmin);
            
            this->imlec->setPosition(Vec2(imlec->getPositionX(),HEIGHT*.85));
            
            
        }

        
        
    });
    
    auto seq = Sequence::create(progressTo, callBack, NULL);
    
    seq->setTag(1);
    
    progressTimer->runAction(seq);
    
}

void GameScene::showPlayerCardsOnScreen(Player *playerRef){
    
    Vector<Kart*>RefKarts = playerRef->getPlayerKarts();
    
    for(int i=0; i<RefKarts.size(); i++){
        
        RefKarts.at(i)->setPosition(Vec2(WIDTH/4.4 + WIDTH*.14*i, HEIGHT*.2));
        this->addChild(RefKarts.at(i));
        
    }
    
}

void GameScene::showOpponentCardsOnScreen(Opponent *opponentRef){
    
    Vector<Kart*>OpponentRefKarts = opponentRef->getOpponentKarts();
    
    for(int i=0; i<OpponentRefKarts.size(); i++){
        
        OpponentRefKarts.at(i)->setPosition(Vec2(WIDTH/4.4 + WIDTH*.14*i, HEIGHT*.92));
        this->addChild(OpponentRefKarts.at(i));
        
    }
    
}

void GameScene::startCounter(int order) {
    
    Sprite *one;
    
    one = Sprite::create("3.png");
    
    this->addChild(one);
    
    FIT_H(one, HEIGHT*.6);
    
    one->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    float scaleX = one->getScaleX();
    float scaleY = one->getScaleY();
    
    auto animScale1 = ScaleBy::create(1, .5, .5);
    auto animFade1 = FadeOut::create(1);
    
    auto spawn1 = Spawn::createWithTwoActions(animScale1, animFade1);
    
    auto callFade1 = CallFunc::create([one,scaleX,scaleY](){
        one->setTexture("2.png");
        one->setOpacity(255);
        one->setScale(scaleX, scaleY);
    });
    
    auto callFade2 = CallFunc::create([one,scaleX,scaleY](){
        one->setTexture("1.png");
        one->setOpacity(255);
        one->setScale(scaleX, scaleY);
    });
    
    auto callBack = CallFunc::create([this,order,one]{
        
        finishDeathMatch(order);
        
        this->removeChild(one);
        
    });
    
    auto seq = Sequence::create(spawn1,callFade1,spawn1,callFade2,spawn1,callBack, NULL);
  
    
    one->runAction(seq);
    
    
}

bool GameScene::finishDeathMatch(int order) {
    
    /*this->addChild(playerKarts.at(order));
    playerKarts.at(order)->setPosition(deathMatchCardArray.at(order)->getPosition());
    this->removeChild(deathMatchCardArray.at(order));
    
    //let cpu choose a random kart
    int random;
 
    random = rand()%opponentKarts.size();
        
    this->addChild(opponentKarts.at(random));
    opponentKarts.at(random)->setPosition(deathMathchOpponentArray.at(random)->getPosition());
    this->removeChild(deathMathchOpponentArray.at(random));
    
    
    glb_order = order;
    glb_random = random;
    
    //kazandın kaybettin
    
    this->schedule(schedule_selector(GameScene::decideDeathMatchResult), 2);*/
    
    //First
    playerKarts.at(order)->setPosition(deathMatchCardArray.at(order)->getPosition());
    playerKarts.at(order)->setScaleX(0);
    
    float myKartScaleX = 0.9;
    float myKartScaleY = 0.9;
    
    float newScaleX, newScaleY;
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        newScaleX = myKartScaleX*1;
        newScaleY = myKartScaleY*1;
        
    }
    else{
        
        newScaleX = myKartScaleX;
        newScaleY = myKartScaleY;
        
    }

    auto scaleAnim = ScaleTo::create(cardFlipTime/2.0, 0, deathMatchCardArray.at(order)->getScaleY());
    auto scaleAnimReverse = ScaleTo::create(cardFlipTime/2.0, newScaleX, newScaleY);
    
    
    auto target1 = TargetedAction::create(deathMatchCardArray.at(order), scaleAnim);
    auto target2 = TargetedAction::create(playerKarts.at(order), scaleAnimReverse);
    auto callbackForTarget2 = CallFunc::create([&,order]{
        
        this->removeChild(deathMatchCardArray.at(order));
        this->addChild(playerKarts.at(order));
        
    });
    
    auto seq = Sequence::create(target1, callbackForTarget2, target2, NULL);
    
    runAction(seq);
    
    //let cpu choose a random kart
    int random;
    
    random = rand()%opponentKarts.size();

    glb_order = order;
    glb_random = random;
    
    //Second
    opponentKarts.at(random)->setPosition(deathMathchOpponentArray.at(random)->getPosition());
    opponentKarts.at(random)->setScaleX(0);
    
    float oppKartScaleX = 0.9;
    float oppKartScaleY = 0.9;
    
    float oppnewScaleX, oppnewScaleY;
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        oppnewScaleX = oppKartScaleX*1;
        oppnewScaleY = oppKartScaleY*1;
        
    }
    else {
        
        oppnewScaleX = oppKartScaleX;
        oppnewScaleY = oppKartScaleY;
        
    }
    
    auto scaleAnim2 = ScaleTo::create(cardFlipTime/2.0, 0, deathMathchOpponentArray.at(random)->getScaleY());
    auto scaleAnimReverse2 = ScaleTo::create(cardFlipTime/2.0, oppnewScaleX, oppnewScaleY);
    
    
    auto opptarget1 = TargetedAction::create(deathMathchOpponentArray.at(random), scaleAnim2);
    auto opptarget2 = TargetedAction::create(opponentKarts.at(random), scaleAnimReverse2);
    auto oppcallbackForTarget2 = CallFunc::create([&,random]{
        
        this->removeChild(deathMathchOpponentArray.at(random));
        this->addChild(opponentKarts.at(random));
        
    });
    
    auto finishCallback = CallFunc::create([&]{
        
        this->schedule(schedule_selector(GameScene::decideDeathMatchResult), 1.5);

        
    });
    
    auto seq2 = Sequence::create(opptarget1, oppcallbackForTarget2, opptarget2, finishCallback, NULL);
    
    runAction(seq2);

    return true;
    
}

void GameScene::getLastScoreByUsername(std::string username, std::string gamename){
    
    
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    
    ScoreBoardService *scoreboardService = App42API::BuildScoreBoardService();
    
    scoreboardService->GetLastScoreByUser(gamename.c_str(), username.c_str() , app42callback(GameScene::onLastScoreRequestCompleted, this));
    
    
}

void GameScene::updateLeaderboardScore(double point, std::string gameName){
    
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    
    ScoreBoardService *scoreboardService = App42API::BuildScoreBoardService();
    
    
    //game name, user name
    
    scoreboardService->SaveUserScore(gameName.c_str(), appManager->username.c_str(), point , app42callback(GameScene::onScoreBoardRequestCompleted, this));
    
}

void GameScene::onLastScoreRequestCompleted(void *response) {
    
    App42GameResponse *scoreResponse = (App42GameResponse*)response;
    printf("\ncode=%d",scoreResponse->getCode());
    printf("\nResponse Body=%s",scoreResponse->getBody().c_str());
    if (scoreResponse->isSuccess)
    {
        for(std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
        {
            printf("\n CreatedAt=%s",it->getCreatedOn().c_str());
            printf("\n ScoreId=%s\n",it->getScoreId().c_str());
            printf("\n ScoreValue=%f\n",it->getScoreValue());
            printf("\n UserName=%s\n",it->getUserName().c_str());
            
            this->updateLeaderboardScore(it->getScoreValue() + 3, appManager->turnuvaName.c_str());
            
        }
    }
    else
    {
        
        this->updateLeaderboardScore(3, appManager->turnuvaName.c_str());
        
        printf("\nerrordetails:%s",scoreResponse->errorDetails.c_str());
        printf("\nerrorMessage:%s",scoreResponse->errorMessage.c_str());
        printf("\nappErrorCode:%d",scoreResponse->appErrorCode);
        printf("\nhttpErrorCode:%d",scoreResponse->httpErrorCode);
        
        
    }
    
}

void GameScene::onScoreBoardRequestCompleted(void *response){
    
    App42GameResponse *scoreResponse = (App42GameResponse*)response;
    printf("\ncode=%d",scoreResponse->getCode());
    printf("\nResponse Body=%s",scoreResponse->getBody().c_str());
    if (scoreResponse->isSuccess)
    {
        for(std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
        {
            printf("\n CreatedAt=%s",it->getCreatedOn().c_str());
            printf("\n Rank=%f\n",it->getRank());
            printf("\n ScoreId=%s\n",it->getScoreId().c_str());
            printf("\n ScoreValue=%f\n",it->getScoreValue());
            printf("\n UserName=%s\n",it->getUserName().c_str());
        }
    }
    else
    {
        printf("\nerrordetails:%s",scoreResponse->errorDetails.c_str());
        printf("\nerrorMessage:%s",scoreResponse->errorMessage.c_str());
        printf("\nappErrorCode:%d",scoreResponse->appErrorCode);
        printf("\nhttpErrorCode:%d",scoreResponse->httpErrorCode);
    }
    
}

void GameScene::updateUserInformation(const char *username, double gold, double xp, double level, double avatar, double games, double won, double lost, double point){
    
    const char *db_name = "Santra";
    const char *collection_name = "userInfo";
    
    const char *key = "username";
    const char *value = username;
    
    App42Object *object = new App42Object();
    
    object->setObject("username", username);
    object->setObject("gold", gold);
    object->setObject("xp", xp);
    object->setObject("level", level);
    object->setObject("avatar", avatar);
    object->setObject("games", games);
    object->setObject("won", won);
    object->setObject("lost", lost);
    object->setObject("point", point);

    
    
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    
    StorageService *storageService = App42API::BuildStorageService();
    
    storageService->UpdateDocumentByKeyValue(db_name, collection_name, key, value, object, app42callback(GameScene::onStorageRequestCompleted, this));
    
}

void GameScene::onStorageRequestCompleted(void *response){
    
    App42StorageResponse *storageResponse = (App42StorageResponse *)response;
    
    printf("\n response body = %s", storageResponse->getBody().c_str());
    
    
    if(storageResponse->isSuccess){
        
        for(std::vector<App42Storage>::iterator it = storageResponse->storages.begin(); it != storageResponse->storages.end(); ++it){
            
            for(std::vector<JSONDocument>::iterator iit = it->jsonDocArray.begin(); iit != it->jsonDocArray.end(); ++iit){
                
                
                printf("\n jsonDoc = %s", iit->getJsonDoc().c_str());
                
                updateUserProfile(iit->getJsonDoc().c_str());
                
                
            }
            
        }
        
    }
    else {
        
        printf("\n errordetails = %s", storageResponse->errorDetails.c_str());
        printf("\n errormessage = %s", storageResponse->errorMessage.c_str());
        
    }
    
    
    
}

void GameScene::updateUserProfile(const char *jsonString){
    
    picojson::value v;
    
    std::string error = picojson::parse(v, jsonString, jsonString + strlen(jsonString));
    
    picojson::object object = v.get<picojson::object>();
    
    
    std::string &username = object["username"].get<string>();
    double avatarNumber = object["avatar"].get<double>();
    double gold = object["gold"].get<double>();
    double level = object["level"].get<double>();
    double xp = object["xp"].get<double>();
    double games = object["games"].get<double>();
    double won = object["won"].get<double>();
    double lost = object["lost"].get<double>();
    double point = object["point"].get<double>();
    
    appManager = AppManager::getInstance();

    appManager->username = username;
    appManager->avatarNumber = avatarNumber;
    appManager->gold = gold;
    appManager->level = level;
    appManager->xp = xp;
    appManager->userGames = games;
    appManager->userWon = won;
    appManager->userLost = lost;
    appManager->userPoint = point;

    
    if(appManager->level == 1){
        
        if(appManager->xp >= 200){
            
            appManager->level ++;
            
            auto scheduler = Director::getInstance()->getScheduler();
            
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(GameScene::levelAnim,this));

            
            
            this->updateUserInformation(appManager->username.c_str(), appManager->gold, appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
            
        }
        
        
    }
    else {
        
        if(appManager->xp >= 200*appManager->level + (200*appManager->level*.2)){
            
            appManager->level ++;
            
            auto scheduler = Director::getInstance()->getScheduler();
            
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(GameScene::levelAnim,this));
            

            this->updateUserInformation(appManager->username.c_str(), appManager->gold, appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
            
        }
        
    }
    
    
}

void GameScene::levelAnim(){
    
    Sprite *blackBG = Sprite::create("blackBG.png");
    
    FIT_WH(blackBG,WIDTH, HEIGHT);
    
    blackBG->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(blackBG);
    

    Label *altinLabel = Label::createWithTTF(StringUtils::format("LEVEL %.0f", appManager->level), "chinese rocks rg.ttf", 100);
    
    altinLabel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    altinLabel->enableGlow(Color4B::BLACK);
    
    this->addChild(altinLabel);
    
    
    altinLabel->setScale(0);
    
    altinLabel->setOpacity(0);
    
    
    for(int i = 0; i < 3; i++){
    
        if(i == 0){
        
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("fireworkparticle.plist");
    
            p->setPosition(Vec2(WIDTH*.2, HEIGHT*.5));
            
            auto anim = MoveBy::create(4, Vec2(WIDTH*.4, HEIGHT*.9));
            
            p->runAction(anim);
            
            this->addChild(p);
        
        }
        else if(i == 1){
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("fireworkparticle.plist");
            
            p->setPosition(Vec2(WIDTH*.7, HEIGHT*.5));
            
            auto anim = MoveBy::create(4, Vec2(WIDTH*.3, HEIGHT*.85));
            
            p->runAction(anim);
            
            this->addChild(p);
            
        }
        else if(i == 2){
            
            CCParticleSystemQuad* p = CCParticleSystemQuad::create("fireworkparticle.plist");
            
            p->setPosition(Vec2(WIDTH*.5, HEIGHT*.2));
            
            auto anim = MoveBy::create(4, Vec2(WIDTH*.5, HEIGHT*.94));
            
            p->runAction(anim);
            
            this->addChild(p);
            
        }
        
    }
    
    
    auto fadeAnim = FadeIn::create(4);
    
    auto scaleAnim = ScaleTo::create(4, 3);
    
    auto spawnAnim = Spawn::create(fadeAnim,scaleAnim, NULL);
    
    auto callback = CallFunc::create([this, altinLabel, blackBG]{
        
        this->removeChild(altinLabel);
        
        this->removeChild(blackBG);
        
    });
    
    auto seq = Sequence::create(spawnAnim, callback, NULL);
    
    altinLabel->runAction(seq);
    
}


void GameScene::decideDeathMatchResult(float dt){
    
    this->unschedule(schedule_selector(GameScene::decideDeathMatchResult));
    
    auto animWin = MoveTo::create(.5, Vec2(WIDTH*.4, HEIGHT*.5));
    auto animLose = MoveTo::create(.5, Vec2(WIDTH*.6, HEIGHT*.5));
    
    auto beatAnim = MoveTo::create(.5, Vec2(WIDTH*.5, HEIGHT*.5));
    auto growAnim = ScaleTo::create(.2, 1.1);
    
    auto deadAnim = MoveTo::create(.2, Vec2(WIDTH*.5, HEIGHT*.5));
    
    auto spawn = Spawn::createWithTwoActions(beatAnim, growAnim);
    
    auto callBack = CallFunc::create([this]{
        
        this->openOthers();
        
    });
    
    auto wonCallBack = CallFunc::create([this]{
        
        this->playerWon();
        
    });
    
    auto loseCallBack = CallFunc::create([this]{
        
        this->playerLose();
        
    });
    
    
    auto loseSeq = Sequence::create(animLose,deadAnim, NULL);
    
    int powerOfplayerKart = playerKarts.at(glb_order)->getKartPower();
    int powerOfopponentKart = opponentKarts.at(glb_random)->getKartPower();

    
    if(powerOfplayerKart > powerOfopponentKart){
        
        printf("kazandın");
        
        appManager->betGold = 180;
        appManager->betXp = 0;
        
        this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 180), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
        
        auto winSeq = Sequence::create(animWin,spawn,callBack,wonCallBack,NULL);
        
        playerKarts.at(glb_order)->setZOrder(1);
        playerKarts.at(glb_order)->runAction(winSeq);
        opponentKarts.at(glb_random)->runAction(loseSeq);
    
        
    }
    else if(powerOfplayerKart == powerOfopponentKart){
        
        printf("esit");
        //deathMatchStart = false;
        playerKarts.erase(glb_order);
        opponentKarts.erase(glb_random);
        deathMatchCardArray.erase(glb_order);
        deathMathchOpponentArray.erase(glb_random);
        
        if(appManager->currentLanguage() == false){
        
            this->createDrawPanel("BERABERE! YENİ KART SEÇİN");
            
        }
        else {
            
            this->createDrawPanel("DRAW! CHOOSE NEW CARD");
            
        }
        
    }
    else {
        
        appManager->betGold = 0;
        appManager->betXp = 0;
        
        printf("kaybettin");
        
        auto winSeq = Sequence::create(animWin,spawn,callBack,loseCallBack,NULL);

        
        opponentKarts.at(glb_random)->setZOrder(1);
        playerKarts.at(glb_order)->runAction(loseSeq);
        opponentKarts.at(glb_random)->runAction(winSeq);
        
        
    }
    
}

void GameScene::playerWon(){
    
    winState = true;
    
    Sprite *kazandin;
    
    if(appManager->currentLanguage() == false){
    
        kazandin = Sprite::create("Kazandin.png");
    
    }
    else {
        
        kazandin = Sprite::create("Won1.png");
        
    }
    
    kazandin->setZOrder(1000);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(kazandin, HEIGHT*.3);
            
        }
        else {
            
            FIT_H(kazandin, HEIGHT*.4);
            
        }
        
    }
    else {
        
        FIT_H(kazandin, HEIGHT*.4);
        
    }
    
    
    kazandin->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    this->addChild(kazandin);
    
    auto growScale = ScaleBy::create(2, 1.5);
    
    auto callGameEndMenu = CallFunc::create([this]{
        
        this->gameEnd();
        
    });
    
    auto seq = Sequence::create(growScale, RemoveSelf::create() ,callGameEndMenu,NULL);
    
    kazandin->runAction(seq);
    
}

void GameScene::playerLose(){
    
    winState = false;
    
    Sprite *kaybettin;
    
    if(appManager->currentLanguage() == false){
    
        kaybettin = Sprite::create("Kaybettin.png");
        
    }
    else {
        
        kaybettin = Sprite::create("Lose1.png");
        
    }
    
    kaybettin->setZOrder(1000);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(kaybettin, HEIGHT*.3);
            
        }
        else {
            
            FIT_H(kaybettin, HEIGHT*.4);
            
        }
        
    }
    else {
        
        FIT_H(kaybettin, HEIGHT*.4);
        
    }
    
    kaybettin->setZOrder(50);
    
    
    kaybettin->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    this->addChild(kaybettin);
    
    auto growScale = ScaleBy::create(2, 1.5);
    
    auto callGameEndMenu = CallFunc::create([this]{
        
        this->gameEnd();
        
    });
    
    auto seq = Sequence::create(growScale, RemoveSelf::create(), callGameEndMenu, NULL);
    
    kaybettin->runAction(seq);
    
}

void GameScene::gameEnd(){
    
    printf("oyun sonlandı");
    
    this->createEndPanel(winState);
    
}

void GameScene::createEndPanel(bool winstate){
    
    panelNode = Node::create();
    
    panelNode->setZOrder(50);
    
    auto blackBG = Sprite::create("penaltyyBG.png");
    FIT_WH(blackBG, WIDTH, HEIGHT);
    blackBG->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    panelNode->addChild(blackBG);
    
    panel = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panel, WIDTH*.95, HEIGHT*.95);
            
        }
        else {
            
            FIT_WH(panel, WIDTH*.95, HEIGHT*.95);
            
        }

    }
    else {
        
        FIT_WH(panel, WIDTH*.95, HEIGHT*.95);
        
    }
    
    panel->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    panelNode->addChild(panel);
    
    //
    panelAvatarPlayer = Sprite::create("AvatarVS.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(panelAvatarPlayer, HEIGHT*.31);
            
        }
        else {
            
            FIT_H(panelAvatarPlayer, HEIGHT*.45);
            
        }
        
    }
    else {
        
        FIT_H(panelAvatarPlayer, HEIGHT*.45);
        
    }
    
    panelAvatarPlayer->setPosition(Vec2(WIDTH*.24,HEIGHT*.5));
    panelAvatarPlayer->setAnchorPoint(Vec2(.5,.5));
    panelNode->addChild(panelAvatarPlayer);
    
    panelAvatarOpponent = Sprite::create("AvatarVS.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(panelAvatarOpponent, HEIGHT*.31);
            
        }
        else {
            
            FIT_H(panelAvatarOpponent, HEIGHT*.45);
            
        }
        
    }
    else {
        
        FIT_H(panelAvatarOpponent, HEIGHT*.45);
        
    }
    
    panelAvatarOpponent->setPosition(Vec2(WIDTH*.76,HEIGHT*.5));
    panelNode->addChild(panelAvatarOpponent);
    panelAvatarOpponent->setAnchorPoint(Vec2(.5,.5));
    
    
    Vector<Sprite*>PlayerAvatarVector;
    
    for(int i = 0; i < 15; i++){
        
        std::string avatarPlayerFile = "avatar " + StringUtils::format("%d.png",(i));
        
        Sprite *avataruser = Sprite::create(avatarPlayerFile);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(avataruser, HEIGHT*.2);
                
            }
            else {
                
                FIT_H(avataruser, HEIGHT*.3);
                
            }
            
        }
        else {
            
            FIT_H(avataruser, HEIGHT*.3);
            
        }
        
        PlayerAvatarVector.pushBack(avataruser);
        
    }
    
    panelNode->addChild(PlayerAvatarVector.at(appManager->avatarNumber));
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            PlayerAvatarVector.at(appManager->avatarNumber)->setPosition(Vec2(panelAvatarPlayer->getPositionX(), panelAvatarPlayer->getPositionY() + HEIGHT*.048));

        }
        else {
            
            
            PlayerAvatarVector.at(appManager->avatarNumber)->setPosition(Vec2(panelAvatarPlayer->getPositionX(), panelAvatarPlayer->getPositionY() + HEIGHT*.048));
            
            
        }
        
    }
    else {
        
        
        PlayerAvatarVector.at(appManager->avatarNumber)->setPosition(Vec2(panelAvatarPlayer->getPositionX(), panelAvatarPlayer->getPositionY() + HEIGHT*.048));

        
    }
    
   /* avatarOpponent = Sprite::create("AvatarVS.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(avatarOpponent, HEIGHT*.21);
            
        }
        else {
            
            FIT_H(avatarOpponent, HEIGHT*.25);
            
        }
        
    }
    else {
        
        FIT_H(avatarOpponent, HEIGHT*.25);
        
    }
    
    avatarOpponent->setPosition(Vec2(WIDTH*.93,HEIGHT*.75));
    this->addChild(avatarOpponent);*/
    
    //opponent avatar
    
    if(appManager->kindOfGame.compare("olumMaci") != 0){
    
    Vector<Sprite*>OpponentAvatarVector;
    
    for(int i=0; i < 15; i++){
        
        std::string avatarOpponentFile = "avatar " + StringUtils::format("%d.png",(i));
        
        Sprite *avatarOpponent = Sprite::create(avatarOpponentFile);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(avatarOpponent, HEIGHT*.2);
                
            }
            else {
                
                FIT_H(avatarOpponent, HEIGHT*.3);
                
            }
            
        }
        else {
            
            FIT_H(avatarOpponent, HEIGHT*.3);
            
        }
        
        
        OpponentAvatarVector.pushBack(avatarOpponent);
        
    }
    
    int rakipAvatarNumber = 0;
    
    rakipAvatarNumber = appManager->rakipAvatarNumber;
        
    
    panelNode->addChild(OpponentAvatarVector.at(appManager->rakipAvatarNumber));
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                OpponentAvatarVector.at(appManager->rakipAvatarNumber)->setPosition(Vec2(panelAvatarOpponent->getPositionX(), panelAvatarOpponent->getPositionY() + HEIGHT*.048));
                
            }
            else {
                
                OpponentAvatarVector.at(appManager->rakipAvatarNumber)->setPosition(Vec2(panelAvatarOpponent->getPositionX(), panelAvatarOpponent->getPositionY() + HEIGHT*.048));
                
                
            }
            
            
        }
        else {
            
            OpponentAvatarVector.at(appManager->rakipAvatarNumber)->setPosition(Vec2(panelAvatarOpponent->getPositionX(), panelAvatarOpponent->getPositionY() + HEIGHT*.048));

            
        }
    
    
    }
    else {
        
        Sprite *robotAvatar = Sprite::create("avatar robot.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(robotAvatar, HEIGHT*.2);
                
                robotAvatar->setPosition(panelAvatarOpponent->getPositionX(), panelAvatarOpponent->getPositionY() + HEIGHT*.048);

                
            }
            else {
                
                FIT_H(robotAvatar, HEIGHT*.3);
                
                
                robotAvatar->setPosition(panelAvatarOpponent->getPositionX(), panelAvatarOpponent->getPositionY() + HEIGHT*.048);
                
            }
            
        }
        else {
            
            FIT_H(robotAvatar, HEIGHT*.3);
            
            
            robotAvatar->setPosition(panelAvatarOpponent->getPositionX(), panelAvatarOpponent->getPositionY() + HEIGHT*.048);
            
        }
        
        panelNode->addChild(robotAvatar);
        
        
    }
    
    //
    if(winstate == true){
        
        if(appManager->currentLanguage() == false){
        
            gameTitle = Sprite::create("KazandinPanel.png");
            
        }
        else {
            
            gameTitle = Sprite::create("Won2.png");
            
        }

    }
    else {
        
        if(appManager->currentLanguage() == false){
        
            gameTitle = Sprite::create("KaybettinPanel.png");
        
        }
        else {
            
            gameTitle = Sprite::create("Lose2.png");
            
        }
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(gameTitle, HEIGHT*.28);
            
        }
        else {
            
            FIT_H(gameTitle, HEIGHT*.3);
            
        }
        
    }
    else {
        
        FIT_H(gameTitle, HEIGHT*.3);
        
    }
    
    gameTitle->setPosition(Vec2(WIDTH*.5,HEIGHT*.87));
    panelNode->addChild(gameTitle);
    
    
    
    odulBut2 = Sprite::create("altinbitis.png");
    odulBut3 = Sprite::create("xpbitis.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(odulBut2, HEIGHT*.12);
            
            odulBut2->setPosition(Vec2(WIDTH*.5,HEIGHT*.6));
            
        }
        else {
            
            FIT_H(odulBut2, HEIGHT*.15);
            
            odulBut2->setPosition(Vec2(WIDTH*.5,HEIGHT*.6));
            
        }
        
    }
    else {
        
         FIT_H(odulBut2, HEIGHT*.15);
        
        odulBut2->setPosition(Vec2(WIDTH*.5,HEIGHT*.6));
        
    }
    
    
    
    panelNode->addChild(odulBut2);

    Label *altinLabel = Label::createWithTTF(StringUtils::format("%.0f", appManager->betGold), "chinese rocks rg.ttf", HEIGHT*.045);
    altinLabel->setColor(Color3B::WHITE);
    altinLabel->enableGlow(Color4B::BLACK);
    altinLabel->setPosition(Vec2(WIDTH*.52, HEIGHT*.588));
    panelNode->addChild(altinLabel);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(odulBut3, HEIGHT*.12);
            
            odulBut3->setPosition(Vec2(WIDTH*.5,HEIGHT*.4));
            
        }
        else {
            
            FIT_H(odulBut3, HEIGHT*.15);
            
            odulBut3->setPosition(Vec2(WIDTH*.5,HEIGHT*.45));
            
        }
        
    }
    else {
        
        FIT_H(odulBut3, HEIGHT*.15);
        
        odulBut3->setPosition(Vec2(WIDTH*.5,HEIGHT*.45));
        
    }
    
    panelNode->addChild(odulBut3);
    
    Label *xpLabel = Label::createWithTTF(StringUtils::format("%.0f", appManager->betXp), "chinese rocks rg.ttf", HEIGHT*.045);
    xpLabel->setColor(Color3B::WHITE);
    xpLabel->enableGlow(Color4B::BLACK);
    xpLabel->setPosition(Vec2(WIDTH*.52, HEIGHT*.438));
    panelNode->addChild(xpLabel);
    
    yenidenOynaBut = Sprite::create("Yeniden Oyna Butonu.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(yenidenOynaBut, HEIGHT*.13);
            
            yenidenOynaBut->setPosition(Vec2(WIDTH*.57,HEIGHT*.22));
            
        }
        else {
            
            FIT_H(yenidenOynaBut, HEIGHT*.13);
            
            yenidenOynaBut->setPosition(Vec2(WIDTH*.57,HEIGHT*.2));
            
        }
        
    }
    else {
    
        FIT_H(yenidenOynaBut, HEIGHT*.13);
        
        yenidenOynaBut->setPosition(Vec2(WIDTH*.57,HEIGHT*.2));
        
    }
    
    if(appManager->kindOfGame.compare("olumMaci") == 0){
    
        panelNode->addChild(yenidenOynaBut);
        
    }
    
    
    geriBut = Sprite::create("Geri Butonu.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(geriBut, HEIGHT*.13);
            
            if(appManager->kindOfGame.compare("olumMaci") == 0){
            
                geriBut->setPosition(Vec2(WIDTH*.43,HEIGHT*.22));
                
            }
            else {
                
                geriBut->setPosition(Vec2(WIDTH*.5, HEIGHT*.22));
                
            }
            
        }
        else {
            
            FIT_H(geriBut, HEIGHT*.13);
            
            if(appManager->kindOfGame.compare("olumMaci") == 0){
            
                geriBut->setPosition(Vec2(WIDTH*.43,HEIGHT*.2));
                
            }
            else {
                
                geriBut->setPosition(Vec2(WIDTH*.5, HEIGHT*.2));
                
            }
            
        }
        
    }
    else {
    
        FIT_H(geriBut, HEIGHT*.13);
        
        if(appManager->kindOfGame.compare("olumMaci") == 0){
        
            geriBut->setPosition(Vec2(WIDTH*.43,HEIGHT*.2));
            
        }
        else {
            
            geriBut->setPosition(Vec2(WIDTH*.5,HEIGHT*.2));
            
        }
        
    }
    
    panelNode->addChild(geriBut);
    
    isimPlayerLabel = Sprite::create("isimButon.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(isimPlayerLabel, HEIGHT*.1);
            
            
            isimPlayerLabel->setPosition(Vec2(WIDTH*.24,HEIGHT*.22));
            
        }
        else {
            
            FIT_H(isimPlayerLabel, HEIGHT*.15);
            
            
            isimPlayerLabel->setPosition(Vec2(WIDTH*.24,HEIGHT*.2));
            
        }
        
    }
    else {
    
        FIT_H(isimPlayerLabel, HEIGHT*.15);
        
        
        isimPlayerLabel->setPosition(Vec2(WIDTH*.24,HEIGHT*.2));
        
    }
    panelNode->addChild(isimPlayerLabel);
    
    Label *isimLabel = Label::createWithTTF(StringUtils::format("%s", appManager->username.c_str()), "chinese rocks rg.ttf", HEIGHT*.045);
    isimLabel->setColor(Color3B::WHITE);
    isimLabel->enableGlow(Color4B::BLACK);
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            isimLabel->setPosition(Vec2(WIDTH*.24, HEIGHT*.22));
            
        }
        else {
            
            isimLabel->setPosition(Vec2(WIDTH*.24, HEIGHT*.19));
            
        }
        
    }
    else {
    
        isimLabel->setPosition(Vec2(WIDTH*.24, HEIGHT*.19));
        
    }
    panelNode->addChild(isimLabel);
    
    isimOpponentLabel = Sprite::create("isimButon.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(isimOpponentLabel, HEIGHT*.1);
            
            isimOpponentLabel->setPosition(Vec2(WIDTH*.76,HEIGHT*.22));
            
        }
        else {
            
            FIT_H(isimOpponentLabel, HEIGHT*.15);
            
            isimOpponentLabel->setPosition(Vec2(WIDTH*.76,HEIGHT*.2));
            
        }
        
    }
    else {
    
        FIT_H(isimOpponentLabel, HEIGHT*.15);
        
        isimOpponentLabel->setPosition(Vec2(WIDTH*.76,HEIGHT*.2));
        
    }
    
    
    panelNode->addChild(isimOpponentLabel);
    
    if(appManager->kindOfGame.compare("olumMaci") == 0){
    
        Label *isimOpLabel = Label::createWithTTF(StringUtils::format("%s", "CPU"), "chinese rocks rg.ttf", HEIGHT*.045);
        isimOpLabel->setColor(Color3B::WHITE);
        isimOpLabel->enableGlow(Color4B::BLACK);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                
                isimOpLabel->setPosition(Vec2(WIDTH*.76, HEIGHT*.22));
                
            }
            else {
                
                
                isimOpLabel->setPosition(Vec2(WIDTH*.76, HEIGHT*.19));
                
            }
            
            
        }
        else {
            
            
            isimOpLabel->setPosition(Vec2(WIDTH*.76, HEIGHT*.19));
            
        }
     
        panelNode->addChild(isimOpLabel);
        
    }
    else {
        
        Label *isimOpLabel = Label::createWithTTF(StringUtils::format("%s", appManager->opponentName.c_str()), "chinese rocks rg.ttf", HEIGHT*.045);
        isimOpLabel->setColor(Color3B::WHITE);
        isimOpLabel->enableGlow(Color4B::BLACK);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                
                isimOpLabel->setPosition(Vec2(WIDTH*.76, HEIGHT*.22));
                
            }
            else {
                
                
                isimOpLabel->setPosition(Vec2(WIDTH*.76, HEIGHT*.19));
                
            }
            
            
        }
        else {
            
            
            isimOpLabel->setPosition(Vec2(WIDTH*.76, HEIGHT*.19));
            
        }
        
        panelNode->addChild(isimOpLabel);

        
    }
    
    this->addChild(panelNode);
    
    panelNode->setPosition(Vec2(0, HEIGHT*1.2));
    
    auto anim = MoveTo::create(0.5, Vec2(0,0));
    //auto ease = EaseBounceInOut::create(anim);
    

    panelNode->runAction(anim);
    
    
    
    //admob
    
    
    
    
    if(rand()%10 < 5){
        
        
        if(appManager->removeads == false){
        
            if(!sdkbox::PluginAdMob::isAvailable("gameover")){
            
                sdkbox::PluginAdMob::cache("gameover");
                sdkbox::PluginAdMob::show("gameover");
            
            }
            else {
                sdkbox::PluginAdMob::cache("gameover");
                sdkbox::PluginAdMob::show("gameover");
            
            }
        
        }
        
    }
    
    
}

void GameScene::openOthers() {
    
    playerKarts.erase(glb_order);
    opponentKarts.erase(glb_random);
    
    //
    deathMatchCardArray.erase(glb_order);
    deathMathchOpponentArray.erase(glb_random);
    
    for(int i = 0; i < playerKarts.size(); i++){
        
    /*    opponentKarts.at(i)->setPosition(deathMathchOpponentArray.at(i)->getPosition());
        
        
        this->addChild(opponentKarts.at(i));
        
    
        opponentKarts.at(i)->setScale(opponentKarts.at(i)->getScale()*.9);
      */
        
        playerKarts.at(i)->setPosition(deathMatchCardArray.at(i)->getPosition());
        playerKarts.at(i)->setScaleX(0);
        
        float myKartScaleX = 0.9;
        float myKartScaleY = 0.9;
        
        float newScaleX, newScaleY;
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
            newScaleX = myKartScaleX*1;
            newScaleY = myKartScaleY*1;
            
        }
        else{
            
            newScaleX = myKartScaleX;
            newScaleY = myKartScaleY;
            
        }
        
        
        auto scaleAnim = ScaleTo::create(cardFlipTime/2.0, 0, deathMatchCardArray.at(i)->getScaleY());
        auto scaleAnimReverse = ScaleTo::create(cardFlipTime/2.0, newScaleX, newScaleY);
        
        
        auto target1 = TargetedAction::create(deathMatchCardArray.at(i), scaleAnim);
        auto target2 = TargetedAction::create(playerKarts.at(i), scaleAnimReverse);
        auto callbackForTarget2 = CallFunc::create([&,i]{
            
            this->removeChild(deathMatchCardArray.at(i));
            this->addChild(playerKarts.at(i));
            
        });
        
        auto seq = Sequence::create(target1, callbackForTarget2, target2, NULL);
        
        this->runAction(seq);

        
        //
        
        
        opponentKarts.at(i)->setPosition(deathMathchOpponentArray.at(i)->getPosition());
        opponentKarts.at(i)->setScaleX(0);
        
        float oppKartScaleX = 0.9;
        float oppKartScaleY = 0.9;
        
        float oppnewScaleX, oppnewScaleY;
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
            oppnewScaleX = oppKartScaleX*1;
            oppnewScaleY = oppKartScaleY*1;
            
        }
        else {
            
            oppnewScaleX = oppKartScaleX;
            oppnewScaleY = oppKartScaleY;
            
        }
        
        auto scaleAnim2 = ScaleTo::create(cardFlipTime/2.0, 0, deathMatchCardArray.at(i)->getScaleY());
        auto scaleAnimReverse2 = ScaleTo::create(cardFlipTime/2.0, oppnewScaleX, oppnewScaleY);
        
        
        auto opptarget1 = TargetedAction::create(deathMatchCardArray.at(i), scaleAnim2);
        auto opptarget2 = TargetedAction::create(opponentKarts.at(i), scaleAnimReverse2);
        auto oppcallbackForTarget2 = CallFunc::create([&,i]{
            
            this->removeChild(deathMathchOpponentArray.at(i));
            this->addChild(opponentKarts.at(i));
            
        });
    
        
        auto seq2 = Sequence::create(opptarget1, oppcallbackForTarget2, opptarget2, NULL);
        
        this->runAction(seq2);
        
    }
    
}

void GameScene::setOlumMaci(){
    
    desteDagitiliyor = true;
    
    createDeck(51);
    
    this->createPlayerForDeathMatch();
    
    this->showOpponentCardsForDeathMatch();
    
    deathMatchStart = false;
    
    //setAvatarsOlumMaci(AppManager::getInstance()->username, "CPU");
    
    UserDefault *userDefaults = UserDefault::getInstance();
    
    if(userDefaults->getBoolForKey("tutorial") == false){
        
        if(appManager->currentLanguage() == false){
        
            this->createTutorial("ÖNÜNE GELEN 3 KARTTAN BİRİNİ SEÇ\n SEÇTİĞİN KART RAKİBİNDEN BÜYÜKSE\n OYUNU KAZAN!\n ALTINLARI KAP!");
        
        }
        else {
            
            this->createTutorial("CHOOSE A CARD FROM YOUR DECK\n IF YOUR CARDS POWER IS BIGGER\n THEN WIN THE GAME\n GET THE GOLDS!");
            
        }
    }
    else if(userDefaults->getBoolForKey("tutorial") == true) {
        
        this->scheduleOnce(schedule_selector(GameScene::desteDagit), 1);
        
    }
    

}

void GameScene::removeTutorial(){
    
    this->removeChild(tutorialNode);
    
}

void GameScene::removeDraw(){
    
    this->removeChild(tutorialNode);
    
}

void GameScene::createDrawPanel(std::string drawString){
    
    tutorialNode = Node::create();
    
    tutorialNode->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(tutorialNode);
    
    //panel sprite
    
    Sprite *tutorialPanel = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tutorialPanel, HEIGHT*.8);
            
        }
        else {
            
            FIT_H(tutorialPanel, HEIGHT*.9);
            
        }
        
    }
    else {
        
        FIT_H(tutorialPanel, HEIGHT*.9);
        
    }
    
    tutorialPanel->setPosition(Vec2(0,0));
    
    tutorialNode->addChild(tutorialPanel);
    
    //label
    
    auto tutorialLabel = Label::createWithTTF(drawString, "chinese rocks rg.ttf", HEIGHT*.5);
    
    tutorialLabel->setPosition(Vec2(0,HEIGHT*.1));
    
    tutorialLabel->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
    tutorialNode->addChild(tutorialLabel);
    
    //button tamam
    
    auto tamamButton = ui::Button::create("tamamyeni.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tamamButton, HEIGHT*.15);
            
            tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
            
        }
        else {
            
            FIT_H(tamamButton, HEIGHT*.16);
            
            tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
            
        }
        
    }
    else {
        
        FIT_H(tamamButton, HEIGHT*.16);
        
        tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
        
    }
    
    tutorialNode->addChild(tamamButton);
    
    tamamButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        
        switch (type) {
                
            case ui::Widget::TouchEventType::BEGAN:
                
                break;
                
            case ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case ui::Widget::TouchEventType::ENDED:
                
                deathMatchStart = false;
                
                this->removeDraw();
                
                break;
                
            case ui::Widget::TouchEventType::MOVED:
                
                break;
                
                
        }
        
        
    });
    
    
    Label *tamamLabel;
    
    if(appManager->currentLanguage() == false){
        
        tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 55);
        
    }
    else {
        
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 55);
        
    }
    
    tamamLabel->setPosition(Vec2(tamamButton->getContentSize().width/2, tamamButton->getContentSize().height/2));
    
    tamamButton->addChild(tamamLabel);
    

    
}

void GameScene::createTutorial(std::string tutorialString){
    
    
    
    tutorialNode = Node::create();
    
    tutorialNode->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(tutorialNode);
    
    //panel sprite
    
    Sprite *tutorialPanel = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tutorialPanel, HEIGHT*.8);
            
        }
        else {
            
            FIT_H(tutorialPanel, HEIGHT*.9);
            
        }
        
    }
    else {
        
        FIT_H(tutorialPanel, HEIGHT*.9);
        
    }
    
    tutorialPanel->setPosition(Vec2(0,0));
    
    tutorialNode->addChild(tutorialPanel);
    
    //label
    
    auto tutorialLabel = Label::createWithTTF(tutorialString, "chinese rocks rg.ttf", HEIGHT*.05);
    
    tutorialLabel->setPosition(Vec2(0,HEIGHT*.1));
    
    tutorialLabel->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
    tutorialNode->addChild(tutorialLabel);
    
    //button tamam
    
    auto tamamButton = ui::Button::create("tamamyeni.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tamamButton, HEIGHT*.15);
            
            tamamButton->setPosition(Vec2(-WIDTH*.12, -HEIGHT*.27));
            
        }
        else {
            
            FIT_H(tamamButton, HEIGHT*.16);
            
            tamamButton->setPosition(Vec2(-WIDTH*.12, -HEIGHT*.27));
            
        }
        
    }
    else {
        
        FIT_H(tamamButton, HEIGHT*.16);
        
        tamamButton->setPosition(Vec2(-WIDTH*.12, -HEIGHT*.27));
        
    }
    
    tutorialNode->addChild(tamamButton);
    
    tamamButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        
        switch (type) {
            
            case ui::Widget::TouchEventType::BEGAN:
                
                break;
            
            case ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case ui::Widget::TouchEventType::ENDED:
                
                this->removeTutorial();
                
                this->scheduleOnce(schedule_selector(GameScene::desteDagit), 1);
                
                break;
        
            case ui::Widget::TouchEventType::MOVED:
                
                break;
                
                
        }
        
        
    });
    
  
    Label *tamamLabel;
    
    if(appManager->currentLanguage() == false){
        
        tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 55);
        
    }
    else {
        
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 55);
        
    }
    
    tamamLabel->setPosition(Vec2(tamamButton->getContentSize().width/2, tamamButton->getContentSize().height/2));
    
    tamamButton->addChild(tamamLabel);
    
    //dont show button
    
    auto dontShowButton = ui::Button::create("tamamyeni.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(dontShowButton, HEIGHT*.15);
            
            dontShowButton->setPosition(Vec2(WIDTH*.12, -HEIGHT*.27));
            
        }
        else {
            
            FIT_H(dontShowButton, HEIGHT*.16);
            
            dontShowButton->setPosition(Vec2(WIDTH*.12, -HEIGHT*.27));
            
        }
        
    }
    else{
        
        FIT_H(dontShowButton, HEIGHT*.16);
        
        dontShowButton->setPosition(Vec2(WIDTH*.12, -HEIGHT*.27));
        
    }
    
    tutorialNode->addChild(dontShowButton);
   
    dontShowButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
       
        UserDefault *userDefaults = UserDefault::getInstance();
        
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                
                break;
                
            case ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case ui::Widget::TouchEventType::ENDED:
                
                userDefaults->setBoolForKey("tutorial", true);
                
                userDefaults->flush();
                
                this->removeTutorial();
                
                this->scheduleOnce(schedule_selector(GameScene::desteDagit), 1);
                
                break;
                
            case ui::Widget::TouchEventType::MOVED:
                
                break;
                
                
        }
        
        
    });
    
    Label *dontShowLabel;
    
    if(appManager->currentLanguage() == false){
        
        dontShowLabel = Label::createWithTTF("BİR DAHA GÖSTERME", "chinese rocks rg.ttf", 37);
        
    }
    else {
        
        dontShowLabel = Label::createWithTTF("DON'T SHOW IT AGAIN", "chinese rocks rg.ttf", 36);
        
    }
    
    dontShowLabel->setPosition(Vec2(dontShowButton->getContentSize().width/2, dontShowButton->getContentSize().height/2));
    
    dontShowLabel->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
    dontShowButton->addChild(dontShowLabel);
    
    
}

void GameScene::desteDagit(float dt){
    
    desteDagitiliyor = true;
    
    if(counter > -1){
        
        if(counter < 3){
            
            auto anim = MoveTo::create(.2, Vec2(WIDTH*.35 + (counter%3)*WIDTH*.15, HEIGHT*.15));
            
            auto anim2 = RotateTo::create(.2, 0);
            
            auto callb = CallFunc::create([&]{
                
                counter--;
                
                this->desteDagit(0);
                
            });
            
            auto seq = Sequence::create(anim, anim2,callb, NULL);
            
            olumMaciDesteCards.at(counter)->runAction(seq);

        }
        else {
            
            auto anim = MoveTo::create(.2, Vec2(WIDTH*.35 + (counter%3)*WIDTH*.15, HEIGHT*.85));
            
            auto anim2 = RotateTo::create(.2, 0);
            
            auto callb = CallFunc::create([&]{
                
                counter--;
                
                this->desteDagit(0);
                
            });
            
            auto seq = Sequence::create(anim, anim2 ,callb, NULL);
            
            olumMaciDesteCards.at(counter)->runAction(seq);
            
        }
        
    }
    else {
        
        desteDagitiliyor = false;
        
    }
   
}

void GameScene::setMac(){
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    warpClientRef->setRoomRequestListener(this);
    warpClientRef->setZoneRequestListener(this);
    
    isOnPenalty = false;

    createDeck(51);
    
    this->createPlayer();
    
    this->createOpponent();
    
    setAvatars(AppManager::getInstance()->username, appManager->opponentName);
    
    setSure();
    
}

void GameScene::onEnterTransitionDidFinish() {
    
    Layer::onEnterTransitionDidFinish();
    
    this->schedule(schedule_selector(GameScene::update));
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
}

void GameScene::update(float dt){
    
    
    
}

void GameScene::playKart(Kart *playingKart){
    
    myPlayedKarts.pushBack(playingKart);
    
    Point kartPosition = playingKart->getPosition(); // kartın ilk pozisyonunu al
    
    playingKart->setZOrder(10);
    
    float tempScale = playingKart->getScale();
    
    auto anim = MoveTo::create(.5, Vec2(WIDTH*.4,HEIGHT*.55));
    
    auto scaleAction1 = ScaleTo::create(.25, tempScale*1.9);
    
    auto scaleAction2 = ScaleTo::create(.25, tempScale);
    
    auto seq = Sequence::create(scaleAction1, scaleAction2, NULL);
    
    auto spawn = Spawn::create(anim, seq, NULL);
    
    playingKart->runAction(spawn);
    
    //playingKart->setPosition(Vec2(-WIDTH*.1, 0));
    
    tempKarts = player->getPlayerKarts();
    
    tempKarts.eraseObject(playingKart);
    
    player->setPlayerKarts(tempKarts);
    
    //giveMeKart(kartPosition);
    
    sendMeKart(kartPosition);
    
}

void GameScene::giveMeKart(Point pos) {
    
    Kart *newKart = Kart::create();
    
    newKart = deckVec.getRandomObject();
    
    this->addChild(newKart);
    
    float tempscalex = newKart->getScaleX();
    
    newKart->setScaleX(0);
    
    tempKarts.pushBack(newKart);
    
    player->setPlayerKarts(tempKarts);
    
    newKart->setPosition(pos);
    
    //auto anim = MoveTo::create(.5, pos);
    
    auto scaleRev = ScaleTo::create(.25, tempscalex, newKart->getScaleY());
    
    player->getPlayerKarts().back()->runAction(scaleRev);
    
    deckVec.eraseObject(newKart);
    
}

void GameScene::sendMeKart(cocos2d::Vec2 pos){
    
    desteCardOnline.at(desteCardOnline.size() - 1)->setZOrder(5);
    
    auto move = MoveTo::create(.5, pos);
    
    auto scale = ScaleTo::create(.25, 0, desteCardOnline.at(desteCardOnline.size() - 1)->getScaleY());
    
    auto callback = CallFunc::create([&, pos]{
        
        this->removeChild(desteCardOnline.at(desteCardOnline.size() - 1));
        
        desteCardOnline.erase(desteCardOnline.size() - 1);
        
        this->giveMeKart(pos);
        
    });
    
    auto rot = RotateBy::create(.5, -desteCardOnline.at(desteCardOnline.size() - 1)->getRotation());
    
    auto spawn = Spawn::create(move , rot, NULL);
    
    auto seq = Sequence::create(spawn, scale, callback, NULL);
    
    desteCardOnline.at(desteCardOnline.size() - 1)->runAction(seq);
    
}

void GameScene::sendDataToOpponent(Kart *playedKart, int i){
    
    AppWarp::Client *warpClient;
    warpClient = AppWarp::Client::getInstance();
    
    std::string nation = playedKart->getUlke();
    std::string name = playedKart->getName();
    int power = playedKart->getKartPower();
    
    
    std::string dataString = nation + "," + name + "," + StringUtils::format("%i", power) + "," + StringUtils::format("%i", i);
    
    
    
    warpClient->sendChat(dataString);
    
}

void GameScene::invitePlayAgain() {
    
    this->rakipTekrarBekle();
    
    AppWarp::Client *warpClient;
    
    warpClient = AppWarp::Client::getInstance();
    
    std::string inviteString = "p";
    
    warpClient->sendChat(inviteString);
    
    
    
}

void GameScene::rakipTekrarBekle() {
    
    this->removeChild(panelNode);
    
    tekrarBeklePanel = Node::create();
    
    tekrarBeklePanel->setZOrder(100);
    
    tekrarBekleBg = Sprite::create("cikisPanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(tekrarBekleBg, WIDTH*.8, HEIGHT*.8);
            
        }
        else {
            
            FIT_H(tekrarBekleBg, HEIGHT*.8);
            
        }
        
    }
    else {
    
        FIT_H(tekrarBekleBg, HEIGHT*.8);
        
    }
    
    tekrarBeklePanel->addChild(tekrarBekleBg);
    
    tekrarBekleBg->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    this->addChild(tekrarBeklePanel);
    
    tekrarBekleLabel = Label::createWithTTF("Rakip bekleniyor...", "chinese rocks rg.ttf" , 60);
    
    tekrarBekleLabel->setPosition(Vec2(WIDTH*.5,HEIGHT*.7));
    
    tekrarBeklePanel->addChild(tekrarBekleLabel);
    
    //buttons
    
    /*evetButton = Sprite::create("evetHayirCikis.png");
    
    FIT_H(evetButton, HEIGHT*.18);
    
    evetButton->setPosition(Vec2(WIDTH*.35, HEIGHT*.35));
    
    tekrarBeklePanel->addChild(evetButton);
    
    
    hayirButton = Sprite::create("evetHayirCikis.png");
    
    FIT_H(hayirButton, HEIGHT*.18);
    
    hayirButton->setPosition(Vec2(WIDTH*.65,HEIGHT*.35));
    
    tekrarBeklePanel->addChild(hayirButton);
    
    //labels
    
    Label *evetLabel = Label::createWithTTF("Evet", "chinese rocks rg.ttf", 70);
    
    evetLabel->setPosition(Vec2(evetButton->getContentSize().width/2,evetButton->getContentSize().height/2));
    
    evetLabel->setColor(Color3B(40, 85, 0));
    
    evetButton->addChild(evetLabel);
    
    
    Label *hayirLabel = Label::createWithTTF("Hayır", "chinese rocks rg.ttf", 70);
    
    hayirLabel->setPosition(Vec2(hayirButton->getContentSize().width/2,hayirButton->getContentSize().height/2));
    
    hayirLabel->setColor(Color3B(40,85,0));
    
    hayirButton->addChild(hayirLabel);*/

    
}

void GameScene::getInvitePlayAgain(){
    
    backPanelIsActive = true;
    
    tekrarBeklePanel = Node::create();
    
    tekrarBeklePanel->setZOrder(100);
    
    tekrarBekleBg = Sprite::create("cikisPanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(tekrarBekleBg, WIDTH*.8, HEIGHT*.8);
            
        }
        else {
            
            FIT_H(tekrarBekleBg, HEIGHT*.8);
            
        }
        
    }
    else {
        
        FIT_H(tekrarBekleBg, HEIGHT*.8);
        
    }
    
    tekrarBeklePanel->addChild(tekrarBekleBg);
    
    tekrarBekleBg->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    this->addChild(tekrarBeklePanel);
    
    tekrarBekleLabel = Label::createWithTTF("Rakip oynamak istiyor. Kabul et?", "chinese rocks rg.ttf" , 60);
    
    tekrarBekleLabel->setPosition(Vec2(WIDTH*.5,HEIGHT*.7));
    
    tekrarBeklePanel->addChild(tekrarBekleLabel);
    
    //buttons
    
    evetButton = Sprite::create("evetHayirCikis.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(evetButton, HEIGHT*.14);
            
        }
        else {
            
            FIT_H(evetButton, HEIGHT*.18);
            
        }
        
    }
    else {
        
        FIT_H(evetButton, HEIGHT*.18);
        
    }
     
    
     
     evetButton->setPosition(Vec2(WIDTH*.35, HEIGHT*.35));
     
     tekrarBeklePanel->addChild(evetButton);
     
     
     hayirButton = Sprite::create("evetHayirCikis.png");
     
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(hayirButton, HEIGHT*.14);
            
        }
        else {
            
            FIT_H(hayirButton, HEIGHT*.18);
            
        }
        
        
    }
    else {
        
        FIT_H(hayirButton, HEIGHT*.18);
        
    }
     
     hayirButton->setPosition(Vec2(WIDTH*.65,HEIGHT*.35));
     
     tekrarBeklePanel->addChild(hayirButton);
     
     //labels
     
     Label *evetLabel = Label::createWithTTF("Evet", "chinese rocks rg.ttf", 70);
     
     evetLabel->setPosition(Vec2(evetButton->getContentSize().width/2,evetButton->getContentSize().height/2));
     
     evetLabel->setColor(Color3B(40, 85, 0));
     
     evetButton->addChild(evetLabel);
     
     
     Label *hayirLabel = Label::createWithTTF("Hayır", "chinese rocks rg.ttf", 70);
     
     hayirLabel->setPosition(Vec2(hayirButton->getContentSize().width/2,hayirButton->getContentSize().height/2));
     
     hayirLabel->setColor(Color3B(40,85,0));
     
     hayirButton->addChild(hayirLabel);

    
}

void GameScene::deleteAndGone(float dt){
    
    this->unschedule(schedule_selector(GameScene::deleteAndGone));
    
    auto scene = MenuScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
    
}

void GameScene::onDeleteRoomDone(AppWarp::room event){
   
    
    this->schedule(schedule_selector(GameScene::deleteAndGone), 3);

}


void GameScene::onChatReceived(AppWarp::chat chatevent){
    
    printf("onchatreceived");
    
    if(chatevent.sender != player->getName()){
        
        if(chatevent.chat.compare("p") == 0){
        
            //yeniden oynama teklifi geldi
            
            panelNode->setVisible(false);
            
            this->getInvitePlayAgain();
            
        }
        else if(chatevent.chat.compare("y") == 0) {
            
            //rakip teklifi kabul etti
            
            this->removeChild(tekrarBeklePanel);
            
            auto scene = GameScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(.5, scene, Color3B::BLACK));
            
        }
        else if(chatevent.chat.compare("n") == 0){
            
            //rakip teklifi reddetti
            
            this->removeChild(tekrarBeklePanel);
            
            AppWarp::Client *warpClientRef;
            
            warpClientRef = AppWarp::Client::getInstance();
            
            warpClientRef->deleteRoom(appManager->myCurrentRoomId);
            
            
        }
        else if(chatevent.chat.substr(0,1).compare("#") == 0){
            
            chatevent.chat.erase(0,1);
            
            std::size_t loc = chatevent.chat.find(',');
            
            std::string nation = chatevent.chat.substr(0,loc);
            
            chatevent.chat.erase(0, loc+1);
            
            loc = chatevent.chat.find(',');
            
            std::string name = chatevent.chat.substr(0,loc);
            chatevent.chat.erase(0, loc+1);
            
            loc = chatevent.chat.find(',');
            
            int power = (int)std::atoi(chatevent.chat.substr(0,loc).c_str());
            
            Kart *kart = Kart::create();
            
            kart->setUlke(nation);
            kart->setName(name);
            kart->setKartPower(power);
            
            updateOpponentPenaltyKartStatus(kart);

            
        }
        else{
        
            std::size_t loc = chatevent.chat.find(',');
        
            std::string nation = chatevent.chat.substr(0,loc);
            chatevent.chat.erase(0, loc+1);
        
            loc = chatevent.chat.find(',');
        
            std::string name = chatevent.chat.substr(0,loc);
            chatevent.chat.erase(0, loc+1);
        
            loc = chatevent.chat.find(',');
        
            int power = (int)std::atoi(chatevent.chat.substr(0,loc).c_str());
        
            chatevent.chat.erase(0, loc+1);
        
            loc = chatevent.chat.find(',');
            
        
        
            int sira = (int)std::atoi(chatevent.chat.substr(0,loc).c_str());
            
            
            
            Kart *kart = Kart::create();
        
            kart->setUlke(nation);
            kart->setName(name);
            kart->setKartPower(power);
        
        
            updateOpponentStatus(kart, sira);

        }

        
    }
    
}

void GameScene::updateMyStatusAndChangeScene(std::string tName){
    
    
    if(tName.compare("") == 0){
        
        this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 25), (appManager->xp + 25), appManager->level, appManager->avatarNumber, (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint));
        
        appManager->betGold = 25;
        appManager->betXp = 25;
        
    }
    else {
        
        if(tName.compare("T1") == 0){
            
            
            this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 180), (appManager->xp + 100), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
            
            
            //puan gönderilecek fonksiyona parametre olarak
            
            appManager->betGold = 180;
            appManager->betGold = 100;
            
            
        }
        else if(tName.compare("T2") == 0){
            
            
            this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 360), (appManager->xp + 200), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
            
            
            appManager->betGold = 360;
            appManager->xp = 200;
            
        }
        else if(tName.compare("T3") == 0){
            
            
            this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 720), (appManager->xp + 400), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
            
            appManager->betGold = 720;
            appManager->betXp = 400;
            
        }
        else if(tName.compare("T4") == 0){
            
            
            this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 1800), (appManager->xp + 1000), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
            
            appManager->betGold = 1800;
            appManager->betXp = 1000;
            
            
        }
        else if(tName.compare("T5") == 0){
            
            
            this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 3600), (appManager->xp + 2000), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
            
            appManager->betGold = 3600;
            appManager->betXp = 2000;
            
        }
        
        this->getLastScoreByUsername(appManager->username.c_str(), appManager->turnuvaName.c_str());
        
    }
    
    //oyun kazandığında altın kazanma var
    
    
}

void GameScene::onUserLeftRoom(AppWarp::room event, std::string username){
    
    
    
    
        printf("%s oyundan ayrıldı",username.c_str());
    
    if(username != appManager->username){
        
        if(appManager->currentLanguage() == false){
    
            this->openWarningPanel("Rakip Oyundan Çıktı. Altınlar Senin!");
            
        }
        else {
            
            this->openWarningPanel("Opponent has left the game. You won!");
            
        }

        this->updateMyStatusAndChangeScene(appManager->turnuvaName);
    
    }
    else {
        
        this->addLoader();
        
        
    }
    
        appManager->isOnline = false;
    
        AppWarp::Client *warpClient = AppWarp::Client::getInstance();
    
        warpClient->deleteRoom(event.roomId);
        
    
    
    
    
}

void GameScene::addLoader() {
    
    blackBg = Sprite::create("penaltyyBG.png");
    FIT_WH(blackBg, WIDTH, HEIGHT);
    blackBg->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    blackBg->setZOrder(50);
    this->addChild(blackBg);
    blackBg->setOpacity(255);
    
    loaderBack = Sprite::create("grassicon.png");
    FIT_H(loaderBack, WIDTH*.4);
    loaderBack->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    loaderBack->setZOrder(50);
    this->addChild(loaderBack);
    loaderBack->setOpacity(200);
    
    loader = Sprite::create("topball.png");
    FIT_H(loader, WIDTH*.12);
    loader->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    loader->setZOrder(50);
    this->addChild(loader);
    
    
    auto anim = RotateBy::create(1.5, 160);
    auto repeat = RepeatForever::create(anim);
    
    loader->runAction(repeat);
    
    if(appManager->currentLanguage() == false){
    
        connectingLabel = Label::createWithTTF("OYUNDAN ÇIKILIYOR", "chinese rocks rg.ttf", 30);
        
    }
    else {
        
        connectingLabel = Label::createWithTTF("LEAVING THE GAME", "chinese rocks rg.ttf", 30);
        
    }
    
    connectingLabel->setTextColor(Color4B(255, 255, 255, 220));
    connectingLabel->setPosition(Vec2(WIDTH*.5,HEIGHT*.2));
    connectingLabel->setZOrder(50);
    this->addChild(connectingLabel);
    
    
    
}


void GameScene::updateOpponentPenaltyKartStatus(Kart *kart){
    
    //penalty kart penalty kart
    
    kart->drawKartWith(kart->getUlke(), kart->getName(), kart->getKartPower());
    
    opponentPenaltyPool.pushBack(kart);
    
    kart->setPosition(Vec2(-WIDTH*.1,HEIGHT*.8));
    
    this->addChild(kart,11);
    
    if(opponentPenaltyPool.size() == 1){
        
        auto action = MoveTo::create(.5, Vec2(WIDTH*.25,HEIGHT*.8));
        
        kart->runAction(action);
        
    }
    else if(opponentPenaltyPool.size() == 2){
        
        auto action = MoveTo::create(.5, Vec2(WIDTH*.5,HEIGHT*.8));
        
        kart->runAction(action);

        
    }
    else if(opponentPenaltyPool.size() == 3){
        
        auto action = MoveTo::create(.5, Vec2(WIDTH*.75,HEIGHT*.8));
        
        kart->runAction(action);
        
        this->schedule(schedule_selector(GameScene::controlScoreForPenalties),4);
    }
    
    
}

void GameScene::controlScoreForPenalties(float dt){
    
    this->unschedule(schedule_selector(GameScene::controlScoreForPenalties));
    
    int myScore = 0;
    int opponentScore = 0;
    
    for(int i = 0; i<penaltyPool.size(); i++){
        
        myScore += penaltyPool.at(i)->getKartPower();
        
    }
    
    for(int o = 0; o<opponentPenaltyPool.size(); o++){
        
        opponentScore += opponentPenaltyPool.at(o)->getKartPower();
        
    }
    
    if(myScore > opponentScore){
        
        playerCurrentScore += myScore + opponentScore;
        
        this->updateScoreLabels(playerCurrentScore, opponentCurrentScore);
        
        printf("\npenaltıyı kazandım");
        
        playerAllWonCards.pushBack(penaltyPool);
        playerAllWonCards.pushBack(opponentPenaltyPool);
        
        
        for(int i = 0; i<3; i++){
            
            if(i < penaltyPool.size() -1){
            
                auto action = MoveTo::create(1, Vec2(WIDTH*1.1,0));
                
                auto seq = Sequence::create(action, RemoveSelf::create(), NULL);
                
                auto seq2 = seq->clone();
            
                penaltyPool.at(i)->runAction(seq);
                opponentPenaltyPool.at(i)->runAction(seq2);
                
            }
            else {
                
                auto action = MoveTo::create(1, Vec2(WIDTH*1.1,0));
                auto action2 = action->clone();
                
                auto callBack = CallFunc::create([&]{
                    
                    letMeThrow = true;
                    penaltyPool.clear();
                    this->removeChild(perdeForPenalty);
                    
                    if(timeOfGame < 90){
                        
                        timeOfGame += 5;
                        sureLabel->setString(StringUtils::format("%d",timeOfGame));
                        
                    }
                    else {
                        
                        this->finishTheGame();
                        
                    }

                    
                    
                    
                });
                
                auto callBack2 = CallFunc::create([&]{
                    
                    opponentPenaltyPool.clear();
                    
                });
                
                auto seq = Sequence::create(action, RemoveSelf::create(), callBack, NULL);
                auto seqOpponent = Sequence::create(action2,RemoveSelf::create(),callBack2, NULL);
                
                penaltyPool.at(i)->runAction(seq);
                opponentPenaltyPool.at(i)->runAction(seqOpponent);
                
            }
            
        }
        
        
    }
    else if(myScore < opponentScore){
        
        opponentCurrentScore += myScore + opponentScore;
        
        this->updateScoreLabels(playerCurrentScore, opponentCurrentScore);
        
        printf("\npenaltıyı kaybettim");
        
        opponentAllWonCards.pushBack(penaltyPool);
        opponentAllWonCards.pushBack(opponentPenaltyPool);
        
        for(int i = 0; i<3; i++){
            
            if(i < penaltyPool.size() -1){
                
                auto action = MoveTo::create(1, Vec2(WIDTH*1.1,HEIGHT));
                
                auto seq = Sequence::create(action, RemoveSelf::create(), NULL);
                
                auto seq2 = seq->clone();
                
                penaltyPool.at(i)->runAction(seq);
                opponentPenaltyPool.at(i)->runAction(seq2);
                
            }
            else {
                
                auto action = MoveTo::create(1, Vec2(WIDTH*1.1,HEIGHT));
                auto action2 = action->clone();
                
                auto callBack = CallFunc::create([&]{
                    
                    letMeThrow = true;
                    penaltyPool.clear();
                    this->removeChild(perdeForPenalty);
                    
                    if(timeOfGame < 90){
                        
                        timeOfGame += 5;
                        sureLabel->setString(StringUtils::format("%d",timeOfGame));
                        
                    }
                    else {
                        
                        this->finishTheGame();
                        
                    }

                    
                });
                
                auto callBack2 = CallFunc::create([&]{
                    
                    opponentPenaltyPool.clear();
                    
                });
                
                auto seq = Sequence::create(action, RemoveSelf::create(), callBack, NULL);
                auto seqOpponent = Sequence::create(action2,RemoveSelf::create(),callBack2, NULL);
                
                penaltyPool.at(i)->runAction(seq);
                opponentPenaltyPool.at(i)->runAction(seqOpponent);
                
            }
            
        }
        
    }
    else {
        
        printf("berabere");
        
        playerAllWonCards.pushBack(penaltyPool);
        opponentAllWonCards.pushBack(opponentPenaltyPool);
        
        
        for(int i = 0; i<3; i++){
            
            if(i < penaltyPool.size() -1){
                
                auto action = MoveTo::create(1, Vec2(WIDTH*1.1,0));
                
                auto seq = Sequence::create(action, RemoveSelf::create(), NULL);
                
                auto action2 = MoveTo::create(1, Vec2(WIDTH*1.1,HEIGHT));
                
                auto seq2 = Sequence::create(action2,RemoveSelf::create(), NULL);
                
                penaltyPool.at(i)->runAction(seq);
                opponentPenaltyPool.at(i)->runAction(seq2);
                
            }
            else {
                
                auto action = MoveTo::create(1, Vec2(WIDTH*1.1,0));
                auto action2 = MoveTo::create(1, Vec2(WIDTH*1.1,HEIGHT));
                
                auto callBack = CallFunc::create([&]{
                    
                    letMeThrow = true;
                    penaltyPool.clear();
                    
                    this->removeChild(perdeForPenalty);
                    
                    if(timeOfGame < 90){
                        
                        timeOfGame += 5;
                        sureLabel->setString(StringUtils::format("%d",timeOfGame));
                        
                    }
                    else {
                        
                        this->finishTheGame();
                        
                    }

                    
                });
                
                auto callBack2 = CallFunc::create([&]{
                    
                    opponentPenaltyPool.clear();
                    
                });
                
                auto seq = Sequence::create(action, RemoveSelf::create(), callBack, NULL);
                auto seqOpponent = Sequence::create(action2,RemoveSelf::create(),callBack2, NULL);
                
                penaltyPool.at(i)->runAction(seq);
                opponentPenaltyPool.at(i)->runAction(seqOpponent);
                
            }
            
        }
        
    }
    
    for(int i = 0; i<cardPool.size(); i++){
        
        this->removeChild(cardPool.at(i));
        
        //atılan kartlar kaldırıldı
        
    }
    
    cardPool.clear(); // cardpool temizlendi
    
    isOnPenalty = false;
    
    if(appManager->isUserAdmin == true){
        
        this->startThrowCounter();
        
    }
    
}

void GameScene::addToSignalsOnCards(Kart *opponentCard){
    
    int opponentKartPower = opponentCard->getKartPower();
    
    for(int i = 0; i < player->getPlayerKarts().size(); i++){
        
        int myKartPower = player->getPlayerKarts().at(i)->getKartPower();
        
        if(opponentKartPower < myKartPower){
            
            Sprite *kupa = Sprite::create("Kupa.png");
            
            FIT_H(kupa, HEIGHT*.07);
            
            Kart *plKart = player->getPlayerKarts().at(i);
            
            kupa->setPosition(Vec2(plKart->getPositionX() - WIDTH*.04,
                                   plKart->getPositionY() + HEIGHT*.18
                                   ));
            
            this->addChild(kupa,2);
            
            signalsOnCards.pushBack(kupa);
            
        }
        else if(opponentKartPower == myKartPower){
            
            Sprite *pen = Sprite::create("P.png");
            
            FIT_H(pen, HEIGHT*.07);
            
            Kart *plKart = player->getPlayerKarts().at(i);
            
            pen->setPosition(Vec2(plKart->getPositionX() - WIDTH*.04,
                                  plKart->getPositionY() + HEIGHT*.18
                                  ));
            
            this->addChild(pen,2);
            
            signalsOnCards.pushBack(pen);
            
        }
        
    }
    
}

void GameScene::removeSignalsOnCards(){
    
    for(int i = 0; i < signalsOnCards.size(); i++){
        
        this->removeChild(signalsOnCards.at(i));
        
    }
    
    signalsOnCards.clear();
    
}

void GameScene::updateOpponentStatus(Kart *kart, int sira){
    
    // new card new card from the opponent
    
    kart->drawKartWith(kart->getUlke(), kart->getName(), kart->getKartPower());
    
    //
    
    Point tempPoint = opponent->getOpponentKarts().at(sira)->getPosition();
    
    auto anim2 = MoveTo::create(.5, Vec2(WIDTH*.6,HEIGHT*.55));
    
    Vector<Kart *>tempOpponentKarts = opponent->getOpponentKarts();
    
    this->addChild(kart);
    
    kart->setZOrder(10);
    
    kart->setPosition(tempOpponentKarts.at(sira)->getPosition());
    
    float tempScale = kart->getScale();
    
    auto scaleAction1 = ScaleTo::create(.25, tempScale*1.9 );
    
    auto scaleAction2 = ScaleTo::create(.25, tempScale);
    
    auto seq = Sequence::create(scaleAction1 , scaleAction2, NULL);
    
    auto spawn = Spawn::create(anim2, seq, NULL);
    
    kart->runAction(spawn);
    
    
    this->removeChild(tempOpponentKarts.at(sira));
    
    tempOpponentKarts.erase(sira);
    
    Kart *newOpponentKart = Kart::create();
    newOpponentKart->setPosition(Vec2(WIDTH*.1, HEIGHT*.5));
    newOpponentKart->drawUnplayedKart();
    this->addChild(newOpponentKart);
    
    tempOpponentKarts.pushBack(newOpponentKart);
    
    opponent->setOpponentKarts(tempOpponentKarts);
    
    auto anim = MoveTo::create(.5, tempPoint);
    opponent->getOpponentKarts().back()->runAction(anim);
    
    cardPool.pushBack(kart);
    
    this->removeChild(desteCardOnline.at(desteCardOnline.size() - 1));
    
    desteCardOnline.erase(desteCardOnline.size() - 1);
    
    if(cardPool.size() == 2){
        
        //kazanma
        if(cardPool.at(0)->getKartPower() > cardPool.at(1)->getKartPower()){
            
            printf("kazandım");
            
            playerCurrentScore += cardPool.at(0)->getKartPower() +cardPool.at(1)->getKartPower();
            
            this->updateScoreLabels(playerCurrentScore, opponentCurrentScore);
            
            this->schedule(schedule_selector(GameScene::wonPerThrow),2.5);
            
        }
        else if(cardPool.at(0)->getKartPower() == cardPool.at(1)->getKartPower() || cardPool.at(0)->getUlke().c_str() == cardPool.at(1)->getUlke().c_str()){
            
            printf("beraber");
            
            this->schedule(schedule_selector(GameScene::penalties),1);

            
        }
        else {
            
            printf("kaybettim");
            
            opponentCurrentScore += cardPool.at(0)->getKartPower() +cardPool.at(1)->getKartPower();
            
            this->updateScoreLabels(playerCurrentScore, opponentCurrentScore);

            
            this->schedule(schedule_selector(GameScene::losePerThrow),2.5);

            
        }
        
       
        
    }
    else {
        
        AppManager::getInstance()->isUserAdmin = true;
        player->setIsAdmin(AppManager::getInstance()->isUserAdmin);
        
        this->addToSignalsOnCards(cardPool.front());
        
        this->startThrowCounter();
        
        this->imlec->setPosition(Vec2(imlec->getPositionX(),HEIGHT*.15));
        
        letMeThrow = true;
        
        //daftpunk
        
    }
    
}

void GameScene::wonPerThrow(float dt) {
    
    this->unschedule(schedule_selector(GameScene::wonPerThrow));
    
    Explosion *exp = Explosion::create(Vec2(WIDTH*.6,HEIGHT*.5));
    
    exp->setPosition(exp->_position);
    
    exp->setUp();
    
    this->addChild(exp);
    
    exp->setZOrder(15);
    
    
    
    tempPool.clear();
    
    for(int i = 0; i < cardPool.size(); i++){
        
        auto tmk = Kart::create();
        tmk->drawKartWith(cardPool.at(i)->getUlke(), cardPool.at(i)->getName(), cardPool.at(i)->getKartPower());
        tmk->setPosition(cardPool.at(i)->getPosition());
        this->addChild(tmk);
        
        auto ply = Kart::create();
        ply->drawKartWith(cardPool.at(i)->getUlke(), cardPool.at(i)->getName(), cardPool.at(i)->getKartPower());
        
        this->removeChild(cardPool.at(i));
        tempPool.pushBack(tmk);
        
        
        playerAllWonCards.pushBack(ply);
        
    }
    
    
    cardPool.clear();
    
    auto callBack = CallFunc::create([&]{
        
        if(timeOfGame < 90){
            
            letMeThrow = true;
            timeOfGame += 5;
            sureLabel->setString(StringUtils::format("%d",timeOfGame));
            
            
            
            
        }
        else {
            
            this->finishTheGame();
            
        }
        
    });

    auto action = MoveTo::create(1, Vec2(WIDTH*1.2,0));
    auto action2 = action->clone();
    
    auto seq1 = Sequence::create(action2, callBack, NULL);
    

    for(int i = 0; i < tempPool.size(); i++){
       
        if(i == 0){
            tempPool.at(i)->runAction(action);
        }
        else {
            
            tempPool.at(i)->runAction(seq1);
        }
       
    }
    
    
}

void GameScene::losePerThrow(float dt) {
    
    this->unschedule(schedule_selector(GameScene::losePerThrow));
    
    Explosion *exp = Explosion::create(Vec2(WIDTH*.6,HEIGHT*.5));
    
    exp->setScaleY(exp->getScaleY()*(-1));
    
    exp->setPosition(exp->_position);
    
    this->addChild(exp);
    
    exp->setZOrder(15);
    
    exp->setUp();

    tempPool.clear();
    
    for(int i = 0; i < cardPool.size(); i++){
        
        auto tmk = Kart::create();
        tmk->drawKartWith(cardPool.at(i)->getUlke(), cardPool.at(i)->getName(), cardPool.at(i)->getKartPower());
        tmk->setPosition(cardPool.at(i)->getPosition());
        this->addChild(tmk);
        
        auto ply = Kart::create();
        ply->drawKartWith(cardPool.at(i)->getUlke(), cardPool.at(i)->getName(), cardPool.at(i)->getKartPower());
        
        this->removeChild(cardPool.at(i));
        tempPool.pushBack(tmk);
        
        opponentAllWonCards.pushBack(ply);
        
    }

    cardPool.clear();
    
    
    auto callBack = CallFunc::create([&]{
        
        if(timeOfGame < 90){
            
            letMeThrow = true;
            timeOfGame += 5;
            sureLabel->setString(StringUtils::format("%d",timeOfGame));
            
        }
        else {
            
            this->finishTheGame();
            
        }
        
    });

    
    auto action = MoveTo::create(1, Vec2(WIDTH*1.2,HEIGHT));
    auto action2 = action->clone();
    
    auto seq1 = Sequence::create(action2, callBack, NULL);
    

    
    for(int i = 0; i < tempPool.size(); i++){
        
        if(i == 0){
            tempPool.at(i)->runAction(action);
        }
        else {
            
            tempPool.at(i)->runAction(seq1);
        }
        
        
    }
    
}


void GameScene::penalties(float dt){
    
    isOnPenalty = true;
    
    perdeForPenalty = Sprite::create("penaltyyBG.png");
    perdeForPenalty->setColor(Color3B(Color4B::BLACK));
    FIT_WH(perdeForPenalty, WIDTH, HEIGHT);
    perdeForPenalty->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(perdeForPenalty,10);
    
    this->unschedule(schedule_selector(GameScene::penalties));
    
    this->schedule(schedule_selector(GameScene::chooseCardForPenalty),1);
    
    
    
}

void GameScene::chooseCardForPenalty(float dt){
    
    auto penaltyKart = Kart::create();
    penaltyKart = deckVec.getRandomObject();
    penaltyPool.pushBack(penaltyKart);
    deckVec.eraseObject(penaltyKart);
    
    penaltyKart->setPosition(Vec2(-WIDTH*.1,HEIGHT*.2));
    
    this->addChild(penaltyKart,11);
    
    if(penaltyPool.size() == 1){
        
        auto action = MoveTo::create(.5, Vec2(WIDTH*.25,HEIGHT*.2));
        
        penaltyKart->runAction(action);
        
    }
    else if(penaltyPool.size() == 2){
        
        auto action = MoveTo::create(.5, Vec2(WIDTH*.5,HEIGHT*.2));
        
        penaltyKart->runAction(action);

        
    }
    else if(penaltyPool.size() == 3){
        
        auto action = MoveTo::create(.5, Vec2(WIDTH*.75,HEIGHT*.2));
        
        penaltyKart->runAction(action);

        this->unschedule(schedule_selector(GameScene::chooseCardForPenalty));
        
        this->sendDataToOpponentForPenalty(penaltyPool);
        
    }
    
}

void GameScene::sendDataToOpponentForPenalty(Vector<Kart *> penaltyKarts){
    
    AppWarp::Client *warpClient;
    warpClient = AppWarp::Client::getInstance();
    
    for(int i = 0; i < penaltyKarts.size(); i++){
    
        std::string nation = penaltyKarts.at(i)->getUlke();
        std::string name = penaltyKarts.at(i)->getName();
        int power = penaltyKarts.at(i)->getKartPower();
    
    
        //# for penalty kart chat
        std::string dataString = "#" + nation + "," + name + "," + StringUtils::format("%i",power) + "," + StringUtils::format("%i",i);
    
        warpClient->sendChat(dataString);
        
    }
    
    
}

void GameScene::finishTheGame(){
    
    if(progressTimer){
    
        progressTimer->stopActionByTag(1);
        this->removeChild(progressTimer);
        this->removeChild(sureBos);
        
    }
    
    canThrow = false;
    
    this->removeChild(imlec);
    
    perdeForPenalty = Sprite::create("penaltyyBG.png");
    
    perdeForPenalty->setColor(Color3B(Color4B(0, 0, 0, 200)));
    
    FIT_WH(perdeForPenalty, WIDTH, HEIGHT);
    
    perdeForPenalty->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    this->addChild(perdeForPenalty);
    
    actOfGame = 2; // kartlara tıklanmasın

    std::sort(playerAllWonCards.begin(), playerAllWonCards.end(), sortByKartPower);
    
    std::sort(opponentAllWonCards.begin(), opponentAllWonCards.end(), sortByKartPower);
    
    
    playerAllWonCards.reverse();
    
    opponentAllWonCards.reverse();
    
    this->schedule(schedule_selector(GameScene::removePlayedCardsFromScene), 0.5);
    
    this->schedule(schedule_selector(GameScene::listAllPlayerCards),0.5);
    
    this->schedule(schedule_selector(GameScene::listAllOpponentCards),0.5);
    
}

void GameScene::removePlayedCardsFromScene(float dt){
    
    this->unschedule(schedule_selector(GameScene::removePlayedCardsFromScene));
    
    for(int i = 0; i < player->getPlayerKarts().size(); i++){
        
        this->removeChild(player->getPlayerKarts().at(i));
        
    }
    
    for(int j = 0; j < opponent->getOpponentKarts().size(); j ++){
        
        this->removeChild(opponent->getOpponentKarts().at(j));
        
    }
    
    for(int k = 0; k < desteCardOnline.size(); k ++){
        
        this->removeChild(desteCardOnline.at(k));
        
    }
    
}

void GameScene::listAllPlayerCards(float dt){
    
    if(sahnePlayerKarts.size() < 11){
        
        if(playerAllWonCards.size() > 0){
        
            sahnePlayerKarts.pushBack(playerAllWonCards.front());
            
            sahnePlayerKarts.at(sahnePlayerKarts.size()-1)->setPosition(Vec2(-WIDTH*.2,HEIGHT*.3));
            
            this->addChild(sahnePlayerKarts.at(sahnePlayerKarts.size()-1));
            
            playerFinishScore += playerAllWonCards.at(0)->getKartPower();
            
            playerAllWonCards.erase(0);
            
            
            if(sahnePlayerKarts.size() <= 6){
                
                auto action = MoveTo::create(.3, Vec2(WIDTH*.14*sahnePlayerKarts.size(), HEIGHT*.3));
                
                sahnePlayerKarts.at(sahnePlayerKarts.size() - 1)->runAction(action);
                
                sahnePlayerKarts.at(sahnePlayerKarts.size() - 1)->setZOrder(10);
                
                
            }
            else if(sahnePlayerKarts.size() > 6){
                
                auto action = MoveTo::create(.3, Vec2(WIDTH*.16*((int)sahnePlayerKarts.size()%6), HEIGHT*.15));
                
                sahnePlayerKarts.at(sahnePlayerKarts.size() - 1)->runAction(action);
                
                sahnePlayerKarts.at(sahnePlayerKarts.size() - 1)->setZOrder(11);
                
                
            }
            
        }
        else if(playerAllWonCards.size() == 0){
            
            this->unschedule(schedule_selector(GameScene::listAllPlayerCards));
            
            
            this->schedule(schedule_selector(GameScene::finishAndGetPanel), 5);
            
        }
        
    }
    else {
        
        this->unschedule(schedule_selector(GameScene::listAllPlayerCards));
        
        this->schedule(schedule_selector(GameScene::finishAndGetPanel), 3);

        
    }
    
    
}

void GameScene::finishAndGetPanel(float dt){
    
    actOfGame = 3;
    
    this->unschedule(schedule_selector(GameScene::finishAndGetPanel));
    
    if(playerFinishScore > opponentFinishScore){
        
        //oyun kazandığında altın kazanma var
        
        if(appManager->turnuvaName.compare("") == 0){
            
            this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 25), (appManager->xp + 25), appManager->level, appManager->avatarNumber, (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint));
            
            appManager->betGold = 25;
            appManager->betXp = 25;
            
        }
        else {
            
            if(appManager->turnuvaName.compare("T1") == 0){
                
                
                this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 180), (appManager->xp + 100), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                
                
                //puan gönderilecek fonksiyona parametre olarak
                
                appManager->betGold = 180;
                appManager->betXp = 100;
                
                
            }
            else if(appManager->turnuvaName.compare("T2") == 0){
                
                
                this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 360), (appManager->xp + 200), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                
                
                appManager->betGold = 360;
                appManager->betXp = 200;
                
            }
            else if(appManager->turnuvaName.compare("T3") == 0){
                
                
                this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 720), (appManager->xp + 400), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                
                appManager->betGold = 720;
                appManager->betXp = 400;
                
            }
            else if(appManager->turnuvaName.compare("T4") == 0){
                
                
                this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 1800), (appManager->xp + 1000), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                
                appManager->betGold = 1800;
                appManager->betXp = 1000;
                
                
            }
            else if(appManager->turnuvaName.compare("T5") == 0){
                
                
                this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 3600), (appManager->xp + 2000), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                
                appManager->betGold = 3600;
                appManager->betXp = 2000;
                
            }
            
            this->getLastScoreByUsername(appManager->username.c_str(), appManager->turnuvaName.c_str());
            
        }
        
        //oyun kazandığında altın kazanma var
        

        
        this->playerWon();
        
    }
    else if(playerFinishScore < opponentFinishScore){
        
        //oyun kaybettiğinde altın kaybetme veya kazanma yok
        
        appManager->betGold = 0;
        appManager->betXp = 0;
        
        this->playerLose();
        
    }
    else {
        
        if(playerCurrentScore > opponentCurrentScore){
            
            if(appManager->turnuvaName.compare("") == 0){
                
                this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 25), (appManager->xp + 25), appManager->level, appManager->avatarNumber, (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint));
                
                appManager->betGold = 25;
                appManager->betXp = 25;
                
            }
            else {
                
                if(appManager->turnuvaName.compare("T1") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 180), (appManager->xp + 100), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                    
                    
                    //puan gönderilecek fonksiyona parametre olarak
                    
                    appManager->betGold = 180;
                    appManager->betXp = 100;
                    
                    
                }
                else if(appManager->turnuvaName.compare("T2") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 360), (appManager->xp + 200), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                    
                    
                    appManager->betGold = 360;
                    appManager->betXp = 200;
                    
                }
                else if(appManager->turnuvaName.compare("T3") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 720), (appManager->xp + 400), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                    
                    appManager->betGold = 720;
                    appManager->betXp = 400;
                    
                }
                else if(appManager->turnuvaName.compare("T4") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 1800), (appManager->xp + 1000), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                    
                    appManager->betGold = 1800;
                    appManager->betXp = 1000;
                    
                    
                }
                else if(appManager->turnuvaName.compare("T5") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 3600), (appManager->xp + 2000), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon + 1), appManager->userLost, (appManager->userPoint + 3));
                    
                    appManager->betGold = 3600;
                    appManager->betXp = 2000;
                    
                }
                
                this->getLastScoreByUsername(appManager->username.c_str(), appManager->turnuvaName.c_str());
                
            }
            
            //oyun kazandığında altın kazanma var
            
            
            
            this->playerWon();

            
            
        }
        else if(playerCurrentScore < opponentCurrentScore){
            
            
            //oyun kaybettiğinde altın kaybetme veya kazanma yok
            
            appManager->betGold = 0;
            appManager->betXp = 0;
            
            this->playerLose();
            
            
        }
        else {
            
            //artık berabere
            
            if(appManager->turnuvaName.compare("") == 0){
                
                this->updateUserInformation(appManager->username.c_str(), (appManager->gold +10), (appManager->xp), appManager->level, appManager->avatarNumber, (appManager->userGames + 1), (appManager->userWon), appManager->userLost, (appManager->userPoint));
                
                appManager->betGold = 10;
                appManager->betXp = 0;
                
            }
            else {
                
                if(appManager->turnuvaName.compare("T1") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 90), (appManager->xp + 50), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon), appManager->userLost, (appManager->userPoint + 1));
                    
                    
                    //puan gönderilecek fonksiyona parametre olarak
                    
                    appManager->betGold = 90;
                    appManager->betXp = 50;
                    
                    
                }
                else if(appManager->turnuvaName.compare("T2") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 180), (appManager->xp + 100), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon), appManager->userLost, (appManager->userPoint + 1));
                    
                    
                    appManager->betGold = 180;
                    appManager->betXp = 100;
                    
                }
                else if(appManager->turnuvaName.compare("T3") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 360), (appManager->xp + 200), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon), appManager->userLost, (appManager->userPoint + 1));
                    
                    appManager->betGold = 360;
                    appManager->betXp = 200;
                    
                }
                else if(appManager->turnuvaName.compare("T4") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 900), (appManager->xp + 500), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon), appManager->userLost, (appManager->userPoint + 1));
                    
                    appManager->betGold = 900;
                    appManager->betXp = 500;
                    
                    
                }
                else if(appManager->turnuvaName.compare("T5") == 0){
                    
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 1800), (appManager->xp + 1000), appManager->level, appManager->avatarNumber,  (appManager->userGames + 1), (appManager->userWon), appManager->userLost, (appManager->userPoint + 1));
                    
                    appManager->betGold = 1800;
                    appManager->betXp = 1000;
                    
                }
                
                this->getLastScoreByUsername(appManager->username.c_str(), appManager->turnuvaName.c_str());
                
            }
            
            
            //berabere
            printf("berabere");

            auto scene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(.5, scene, Color3B::BLACK));
            
            
        }
        
        
    }
    
}

void GameScene::listAllOpponentCards(float dt){
    
    if(sahneOpponentKarts.size() < 11){
        
        if(opponentAllWonCards.size() > 0){
            
            sahneOpponentKarts.pushBack(opponentAllWonCards.front());
            
            sahneOpponentKarts.at(sahneOpponentKarts.size()-1)->setPosition(Vec2(-WIDTH*.2,HEIGHT*.7));
            
            this->addChild(sahneOpponentKarts.at(sahneOpponentKarts.size()-1));
            
            opponentFinishScore += opponentAllWonCards.at(0)->getKartPower();

            opponentAllWonCards.erase(0);
            
            
            if(sahneOpponentKarts.size() <= 6){
             
                sahneOpponentKarts.at(sahneOpponentKarts.size() - 1)->setZOrder(10);
                
                auto action = MoveTo::create(.3, Vec2(WIDTH*.14*sahneOpponentKarts.size(), HEIGHT*.85));
                
                sahneOpponentKarts.at(sahneOpponentKarts.size() - 1)->runAction(action);
                
                
            }
            else if(sahneOpponentKarts.size() > 6){
                
                sahneOpponentKarts.at(sahneOpponentKarts.size() - 1)->setZOrder(11);
                
                auto action = MoveTo::create(.3, Vec2(WIDTH*.16*((int)sahneOpponentKarts.size()%6), HEIGHT*.7));
                
                sahneOpponentKarts.at(sahneOpponentKarts.size() - 1)->runAction(action);
                
                
            }

            
        }
        else if(opponentAllWonCards.size() == 0){
            
            this->unschedule(schedule_selector(GameScene::listAllOpponentCards));
            
        }
        
    }
    else {
        
        this->unschedule(schedule_selector(GameScene::listAllOpponentCards));
        
    }
    
    
    
}

bool sortByKartPower(const Kart *lhs, const Kart *rhs)
{
    return lhs->_power < rhs->_power;

}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(actOfGame == 0 && deathMatchStart == false && desteDagitiliyor == false){
        
        for(int i = 0; i <deathMatchCardArray.size(); i++){
            
            if(deathMatchCardArray.at(i)->getBoundingBox().containsPoint(touch->getLocation())){
                
                deathMatchCardArray.at(i)->setOpacity(128);
                
                break;
                
            }
            
        }
        
        
        
    }
    else if(actOfGame == 0 && deathMatchStart == true){
        
        if(geriBut){
            
            Point nodePoint = panelNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(geriBut->getBoundingBox().containsPoint(nodePoint)){
                
                geriBut->setOpacity(128);
                
            }
            
        }
        if(yenidenOynaBut){
            
            Point nodePoint = panelNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(yenidenOynaBut->getBoundingBox().containsPoint(nodePoint)){
                
                yenidenOynaBut->setOpacity(128);
                
            }
            
        }
        
    }
    else if(actOfGame == 3){
        
        if(geriBut){
            
            Point nodePoint = panelNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(geriBut->getBoundingBox().containsPoint(nodePoint)){
                
                geriBut->setOpacity(128);
                
            }
            
        }
        /*if(yenidenOynaBut){
            
            Point nodePoint = panelNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(yenidenOynaBut->getBoundingBox().containsPoint(nodePoint)){
                
                yenidenOynaBut->setOpacity(128);
                
            }
            
        }*/

        if(backPanelIsActive == true){
            
            Point nodePoint = tekrarBeklePanel->convertToNodeSpaceAR(touch->getLocation());
            
            if(evetButton->getBoundingBox().containsPoint(nodePoint)){
                
                evetButton->setOpacity(128);
                
            }
            else if(hayirButton->getBoundingBox().containsPoint(nodePoint)){
                
                hayirButton->setOpacity(128);
                
            }

            
            
        }
        
    }
    
    //ayarlarbutton node touches
    
    Point ayarlarPoint = ayarlarNode->convertToNodeSpaceAR(touch->getLocation());
    
    if(ayarlarBut->getBoundingBox().containsPoint(ayarlarPoint)){
        
        ayarlarBut->setOpacity(128);
        
    }
    else if(quitGameButton->getBoundingBox().containsPoint(ayarlarPoint)){
        
        quitGameButton->setOpacity(128);
        
    }
    else if(soundButton->getBoundingBox().containsPoint(ayarlarPoint)){
        
        soundButton->setOpacity(128);
        
    }
    else if(musicButton->getBoundingBox().containsPoint(ayarlarPoint)){
        
        musicButton->setOpacity(128);
        
    }
    
        
    return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(actOfGame == 0 && deathMatchStart == false && desteDagitiliyor == false){
        
        for(int i = 0; i <deathMatchCardArray.size(); i++){
            
            if(!deathMatchCardArray.at(i)->getBoundingBox().containsPoint(touch->getLocation())){
                
                deathMatchCardArray.at(i)->setOpacity(255);
                
            }
            
        }
        
    }
    else if(actOfGame == 0 && deathMatchStart == true){
        
        if(geriBut){
            
            Point nodePoint = panelNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(!geriBut->getBoundingBox().containsPoint(nodePoint)){
                
                geriBut->setOpacity(255);
                
            }
            
        }
        if(yenidenOynaBut){
            
            Point nodePoint = panelNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(!yenidenOynaBut->getBoundingBox().containsPoint(nodePoint)){
                
                yenidenOynaBut->setOpacity(255);
                
            }
            
        }
        
    }

    else if(actOfGame == 3){
        
        if(geriBut){
            
            Point nodePoint = panelNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(!geriBut->getBoundingBox().containsPoint(nodePoint)){
                
                geriBut->setOpacity(255);
                
            }
            
        }
        /*if(yenidenOynaBut){
            
            Point nodePoint = panelNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(!yenidenOynaBut->getBoundingBox().containsPoint(nodePoint)){
                
                yenidenOynaBut->setOpacity(255);
                
            }
            
        }*/
        
        if(backPanelIsActive == true){
            
            Point nodePoint = tekrarBeklePanel->convertToNodeSpaceAR(touch->getLocation());
            
            if(!evetButton->getBoundingBox().containsPoint(nodePoint)){
                
                evetButton->setOpacity(255);
                
            }
            else if(!hayirButton->getBoundingBox().containsPoint(nodePoint)){
                
                hayirButton->setOpacity(255);
                
            }
            
            
            
        }


        
    }
    
    
    //ayarlar node touches
    Point ayarlarPoint = ayarlarNode->convertToNodeSpaceAR(touch->getLocation());
    
    if(!ayarlarBut->getBoundingBox().containsPoint(ayarlarPoint)){
        
        ayarlarBut->setOpacity(255);
        
    }
    else if(!quitGameButton->getBoundingBox().containsPoint(ayarlarPoint)){
        
        quitGameButton->setOpacity(255);
        
    }
    else if(!soundButton->getBoundingBox().containsPoint(ayarlarPoint)){
        
        soundButton->setOpacity(255);
        
    }
    else if(!musicButton->getBoundingBox().containsPoint(ayarlarPoint)){
        
        musicButton->setOpacity(255);
        
    }
    
    
}

void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(actOfGame == 0 && deathMatchStart == false && desteDagitiliyor == false){
        
        for(int i = 0; i <deathMatchCardArray.size(); i++){
            
            if(deathMatchCardArray.at(i)->getOpacity() == 128){
                
                if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                    
                }
                
                deathMatchCardArray.at(i)->setOpacity(255);
                
                deathMatchStart = true;
                
                startCounter(i);
                
                break;
                
            }
            
        }
        
    }
    else if(actOfGame == 0 && deathMatchStart == true){
        
        if(geriBut){
          
            if(geriBut->getOpacity() == 128){
                
                if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                    
                }
                
                auto scene = MenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionCrossFade::create(.5, scene));
                
            }
            
        }
        if(yenidenOynaBut){
            
            if(yenidenOynaBut->getOpacity() == 128){
                
                if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                    
                }
                
                if(appManager->gold >= 100){
                
                    updateUserInformation(appManager->username.c_str(), appManager->gold - 100, appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
                    
                    auto scene = GameScene::createScene();
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(.5, scene));
                    
                }
                else {
                    
                    //UYAR
                    
                    if(appManager->currentLanguage() == false){
                    
                        this->openWarningPanel("Yeterli Altın Yok");
                        
                    }
                    else {
                        
                        this->openWarningPanel("Not enough gold!");
                        
                    }
                    
                }
               
                
            }
            
        }
        
    }
    else if(actOfGame == 3){
        
        if(geriBut){
            
            if(geriBut->getOpacity() == 128){
                
                if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                    
                }
                
                auto scene = MenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionCrossFade::create(.5, scene));
                
            }
            
        }
        /*if(yenidenOynaBut){
            
            if(yenidenOynaBut->getOpacity() == 128){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
                this->invitePlayAgain();
                
            }
            
        }
        */
        if(backPanelIsActive == true){
            
            if(evetButton->getOpacity() == 128){
                
                if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                    
                }
                
                removeChild(tekrarBeklePanel);
                
                AppWarp::Client *warpClientRef;
                
                warpClientRef = AppWarp::Client::getInstance();
                
                warpClientRef->sendChat("y");
                
                
                auto scene = GameScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(.5, scene, Color3B::BLACK));
                
            }
            else if(hayirButton->getOpacity() == 128){
                
                if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                    
                }
                
                removeChild(tekrarBeklePanel);
                
                AppWarp::Client *warpClientRef;
                
                warpClientRef = AppWarp::Client::getInstance();
                
                warpClientRef->sendChat("n");
                
                warpClientRef->leaveRoom(appManager->myCurrentRoomId);
                
                auto scene = MenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(.5, scene, Color3B::BLACK));

                
            }
            
            
            
        }


        
    }
    else if(actOfGame == 1){
        
        if(player->getIsAdmin() == true && isOnPenalty == false && letMeThrow == true){
            
            if(touch->getLocation().y < HEIGHT*.5 && player->getIsAdmin() == true && canThrow == true){
                
                for(int i=0; i<player->getPlayerKarts().size(); i++){
                    
                    if(player->getPlayerKarts().at(i)->getIsPlayed() == true){
                        
                        letMeThrow = false;
                        
                        canThrow = false;
                        
                        progressTimer->stopActionByTag(1);
                        
                        if(appManager->fx == true){
                        
                        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                            
                        }
                        
                        controlCard = Kart::create();
                        
                        controlCard = player->getPlayerKarts().at(i);
                        
                        cardPool.pushBack(controlCard);
                        
                        sendDataToOpponent(player->getPlayerKarts().at(i), i);
                        
                        playKart(player->getPlayerKarts().at(i));
                        
                        this->removeChild(sureBos);
                        
                        this->removeChild(progressTimer);

                        
                        if(cardPool.size() == 2){
                        
                            this->removeSignalsOnCards();
                            
                            if(cardPool.at(0)->getKartPower() < cardPool.at(1)->getKartPower()){
                            
                                printf("kazandım");
                                 
                                playerCurrentScore += cardPool.at(0)->getKartPower() + cardPool.at(1)->getKartPower();
                                
                                this->updateScoreLabels(playerCurrentScore, opponentCurrentScore);

                                this->schedule(schedule_selector(GameScene::wonPerThrow),2.5);
                                
                                this->startThrowCounter();
                            
                            }
                            else if(cardPool.at(0)->getKartPower() == cardPool.at(1)->getKartPower() || cardPool.at(0)->getUlke().c_str() == cardPool.at(1)->getUlke().c_str()){
                            
                                printf("beraber");
                                this->schedule(schedule_selector(GameScene::penalties),1);
                            
                            }
                            else {
                            
                                printf("kaybettim");
                                
                             
                                
                                opponentCurrentScore += cardPool.at(0)->getKartPower() +cardPool.at(1)->getKartPower();
                                
                                this->updateScoreLabels(playerCurrentScore, opponentCurrentScore);

                                
                                this->schedule(schedule_selector(GameScene::losePerThrow),2.5);
                                
                                this->startThrowCounter();
                                
                            }
                            
                        
                            
                        }
                        else {
                            
                            AppManager::getInstance()->isUserAdmin = false;
                            
                            player->setIsAdmin(AppManager::getInstance()->isUserAdmin);
                            
                            this->imlec->setPosition(Vec2(imlec->getPositionX(),HEIGHT*.85));

                            
                        }
                        
                        break;
                        
                    }
                    
                }
                
            }
        
        }
        
    }
    
    //ayarlar node touches
    if(ayarlarBut->getOpacity() == 128){
        
        if(appManager->fx == true){
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
        }
        
        ayarlarBut->setOpacity(255);
        openAyarlarMenu(isAyarlarOpen);
        
    }
    else if(quitGameButton->getOpacity() == 128){
        
        if(appManager->fx == true){
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
        }
        
        //this->pause();
        
        if(appManager->isOnline == false){
            
            auto scene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
            
        }
        else if(appManager->isOnline == true){
            
            AppWarp::Client *warpClientRef;
            
            warpClientRef = AppWarp::Client::getInstance();
            
            warpClientRef->leaveRoom(appManager->myCurrentRoomId);

            
            appManager->isOnline = false;
            
        }
        
    }
    else if(soundButton->getOpacity() == 128){
        
        soundButton->setOpacity(255);
        
        if(appManager->fx == true){
            
            appManager->fx = false;
            
            soundButton->setTexture("Ses Ac Kapa Kapali.png");
            
        }
        else{
            
            appManager->fx = true;
            
            soundButton->setTexture("Ses Ac Kapa.png");
            
        }
        
        
    }
    else if(musicButton->getOpacity() == 128){
        
        musicButton->setOpacity(255);
        
        if(appManager->sound == true){
            
            appManager->sound = false;
            
            musicButton->setTexture("Muzik Ac Kapa Kapali.png");
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
            
        }
        else {
            
            appManager->sound = true;
            
            musicButton->setTexture("Muzik Ac Kapa.png");
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
            
        }
        
        
    }
    

    
}

void GameScene::callQuitKeyAction() {
    
    quitPanel = Node::create();
    
    yesButton = Button::create("tamamyeni.png");
    
    noButton = Button::create("tamamyeni.png");
    
    quitPanel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(quitPanel, 50);
    
    quitBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(quitBG, WIDTH*.8, HEIGHT*.8);
            
            FIT_H(yesButton, HEIGHT*.13);
            
            FIT_H(noButton, HEIGHT*.13);
            
        }
        else {
            
            FIT_WH(quitBG,WIDTH*.8 ,HEIGHT*.8);
            
            FIT_H(yesButton, HEIGHT*.18);
            
            FIT_H(noButton, HEIGHT*.18);
            
        }
        
    }
    else {
        
        FIT_WH(quitBG,WIDTH*.8 ,HEIGHT*.8);
        
        FIT_H(yesButton, HEIGHT*.18);
        
        FIT_H(noButton, HEIGHT*.18);
        
    }
    
    quitPanel->addChild(quitBG);
    
    quitBG->setPosition(Vec2(0,0));
    
    std::string quitWarnStr, yesStr, noStr;
    
    if(appManager->currentLanguage() == false){
        
        quitWarnStr = "OYUNDAN CIKMAK ISTIYOR MUSUNUZ?";
        
        yesStr = "EVET";
        
        noStr = "HAYIR";
        
    }
    else {
        
        quitWarnStr = "ARE YOU SURE TO QUIT?";
        
        yesStr = "YES";
        
        noStr = "NO";
        
    }
    
    
    Label *quitWarnLabel = Label::createWithTTF(quitWarnStr, "chinese rocks rg.ttf", HEIGHT*.07);
    
    Label *yesLabel = Label::createWithTTF(yesStr, "chinese rocks rg.ttf", 50);
    
    Label *noLabel = Label::createWithTTF(noStr, "chinese rocks rg.ttf", 50);
    
    quitPanel->addChild(quitWarnLabel);
    
    quitWarnLabel->setPosition(Vec2(0, HEIGHT*.25));

    quitPanel->addChild(yesButton);
    
    yesButton->addChild(yesLabel);
    
    yesLabel->setPosition(Vec2(yesButton->getContentSize().width/2, yesButton->getContentSize().height/2));
    
    yesButton->setPosition(Vec2(-WIDTH*.15, -HEIGHT*.15));

    quitPanel->addChild(noButton);
    
    noButton->addChild(noLabel);
    
    noLabel->setPosition(Vec2(noButton->getContentSize().width/2, noButton->getContentSize().height/2));
    
    noButton->setPosition(Vec2(WIDTH*.15, -HEIGHT*.15));

    
    yesButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
        
      
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                break;
            
            case ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case ui::Widget::TouchEventType::ENDED:
                
                
                //quit
                
                if(appManager->fx == true){
                    
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                    
                }
                
                //this->pause();
                
                if(appManager->isOnline == false){
                    
                    auto scene = MenuScene::createScene();
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
                    
                }
                else if(appManager->isOnline == true){
                    
                    AppWarp::Client *warpClientRef;
                    
                    warpClientRef = AppWarp::Client::getInstance();
                    
                    warpClientRef->leaveRoom(appManager->myCurrentRoomId);
                    
                    
                    appManager->isOnline = false;
                    
                }
                
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
            
        }
        
        
    });
    
    noButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
        
       
        switch (type) {
            
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
            
                break;
            
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                
                //remove quit panel
                
                this->removeChild(quitPanel);
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
           
        }
        
        
    });
    
    
}








