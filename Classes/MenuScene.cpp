//
//  MenuScene.cpp
//  Santra
//
//  Created by Anıl Gülgör on 08/12/15.
//
//

#include "MenuScene.hpp"


Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool MenuScene::init() {
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
    
        this->setKeypadEnabled(true);
        
    }
    
    appManager = AppManager::getInstance();
    
    auto scheduler = Director::getInstance()->getScheduler();
    
    scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MenuScene::getTimeFromAPI,this));
    
    //background music
    
    if(appManager->sound == true){

        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("menumusic.mp3",true);
        
    }
    else {
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("menumusic.mp3",true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        
    }
    
    WIDTH = GET_WIDTH;
    HEIGHT = GET_HEIGHT;
    
    
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    
    AppWarp::Client::initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
        
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    
    appManager->isOnline = false;
    
    std::string avatarNum = "avatar " + StringUtils::format("%.0f", (appManager->avatarNumber)) + ".png";
    
    userAvatar = Sprite::create(avatarNum);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(userAvatar, HEIGHT*.09);
            
            
            
        }
        else {
            
            FIT_H(userAvatar, HEIGHT*.13);
            
        }

        
        
    }
    else {
    
        FIT_H(userAvatar, HEIGHT*.13);
        
    }
    
    background = Sprite::create("Background.png");
    FIT_WH(background, WIDTH, HEIGHT);
    background->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    this->addChild(background);
    
    createTopViews();
    
    createMainMenu();
    
    UserDefault *userDefaults = UserDefault::getInstance();
    
    if(userDefaults->getBoolForKey("removed") == false){
        
        appManager->removeads = false;
        
    }
    else {
        
        appManager->removeads = true;
        
    }
    
    
    
    
    
    return true;
    
}

void MenuScene::callQuitKeyAction() {
    
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
            {
                
                //quit
                
                /*auto scene = MenuScene::createScene();
                 Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));*/
                
                
                UserDefault *userDefaults = UserDefault::getInstance();
                
                userDefaults->setIntegerForKey("sessionID", NULL);
                
                userDefaults->setIntegerForKey("tempSessionID", NULL);
                
                userDefaults->flush();
                
                AppWarp::Client *warpClientRef;
                
                warpClientRef = AppWarp::Client::getInstance();
                
                //warpClientRef->leaveRoom(appManager->myCurrentRoomId);
                
                if(appManager->isConnected == true){
                    
                    warpClientRef->disconnect();
                    
                }
                
                Director::getInstance()->end();
                
                break;
                
            }
                
            case ui::Widget::TouchEventType::MOVED:
                
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


void MenuScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event){
    
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
        
        
        this->callQuitKeyAction();
        
    }
    
}

void MenuScene::getTimeFromAPI() {
    
    /*std::vector<std::string> headers;
    
    headers.push_back("Content-Type:application/json; charset=utf-8");
    headers.push_back("Accept:application/json");
    
    request = new HttpRequest();
    
    request->setUrl(TimeApiURL);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setHeaders(headers);
    
    
    request->setResponseCallback(CC_CALLBACK_2(MenuScene::onHttpRequestCompleted, this));
    
    HttpClient::getInstance()->enableCookies(nullptr);
    HttpClient::getInstance()->send(request);
    
    request->release();*/
    
    
    const char* timerName = "timer";
    const char* userName = appManager->username.c_str();
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    TimerService *timerService = App42API::BuildTimerService();
    timerService->GetCurrentTime(app42callback(MenuScene::onTimerServiceRequestCompleted, this));
    
}

void MenuScene::onTimerServiceRequestCompleted(void *response){
    
    App42TimerResponse *app42TimerResponse = (App42TimerResponse*)response;
    if(app42TimerResponse->isSuccess)
    {
        /*printf("\ncode=%d",app42TimerResponse->getCode());
        printf("\nisSuccess=%d",app42TimerResponse->isSuccess);
        printf("\nResponse Body=%s",app42TimerResponse->getBody().c_str());
        printf("\n TimerName =%s",app42TimerResponse->app42Timer.name.c_str());
        printf("\n userName =%s",app42TimerResponse->app42Timer.userName.c_str());
        printf("\n startTime =%s",app42TimerResponse->app42Timer.startTime.c_str());
        printf("\n endTime =%s",app42TimerResponse->app42Timer.endTime.c_str());
        printf("\n isTimerActive=%d",app42TimerResponse->app42Timer.isTimerActive);*/
        
        appManager->currentTime = app42TimerResponse->app42Timer.currentTime.substr(0,10);
        
        UserDefault *userDefaults = UserDefault::getInstance();
        
        std::string str = userDefaults->getStringForKey("currentTime");
        
        //CCLOG(str.c_str());
        
        if(userDefaults->getStringForKey("currentTime") != appManager->currentTime){
            
            userDefaults->setStringForKey("currentTime", appManager->currentTime);
            
            userDefaults->flush();
            
            // altın ver
            
            if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("coin.wav");
                
            }
            
            auto scheduler = Director::getInstance()->getScheduler();
            
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MenuScene::altinAnim,this));
            
            goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 200)));
            
            this->updateUserForAvatar(appManager->username.c_str(), (appManager->gold + 200), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
            
        }
        
        
    }
    else
    {
        printf("\nerrordetails:%s",app42TimerResponse->errorDetails.c_str());
        printf("\nerrorMessage:%s",app42TimerResponse->errorMessage.c_str());
        printf("\nappErrorCode:%d",app42TimerResponse->appErrorCode);
        printf("\nhttpErrorCode:%d",app42TimerResponse->httpErrorCode);
    }
    
}



void MenuScene::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
    
    if(response->getResponseCode() == 200){
        
        printf("success\n");
        
        
        

        std::vector<char> *buffer = response->getResponseData();
        const char *data = reinterpret_cast<char *>(&(buffer->front()));
        //CCASSERT(error.empty(), error.c_str());
        
        picojson::value v;
        
        std::string error = picojson::parse(v, data, data + strlen(data));
        
        picojson::object obj = v.get<picojson::object>();
    
        std::string dateString = obj["dateString"].get<std::string>();
        
        
            
            appManager->currentTime = dateString.substr(0,10);
        
            UserDefault *userDefaults = UserDefault::getInstance();
            
            std::string str = userDefaults->getStringForKey("currentTime");
            
            //CCLOG(str.c_str());
        
            if(userDefaults->getStringForKey("currentTime") != appManager->currentTime){
            
                userDefaults->setStringForKey("currentTime", appManager->currentTime);
            
                userDefaults->flush();
            
                // altın ver
                
                if(appManager->fx == true){
                
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("coin.wav");
                    
                }
            
                auto scheduler = Director::getInstance()->getScheduler();
            
                scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MenuScene::altinAnim,this));
            
                this->updateUserForAvatar(appManager->username.c_str(), (appManager->gold + 200), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
            
            }
            
        
        
        
    }
    
}

void MenuScene::altinAnim(){
    
    Sprite *blackBG = Sprite::create("blackBG.png");
    
    FIT_WH(blackBG,WIDTH, HEIGHT);
    
    blackBG->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(blackBG,20);
    
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
            
            
        }
        else {
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
        }
        
        
    }
    else {
        
        FIT_WH(panelBG, WIDTH*.8, HEIGHT*.8);
        
    }
    
    panelBG->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(panelBG,20);
    
    //altinlar
    
    Sprite *altinlar = Sprite::create("gunlukaltin.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(altinlar, HEIGHT*.28);
            
            
            
        }
        else {
            
            FIT_H(altinlar, HEIGHT*.4);
            
        }
        
        
    }
    else {
    
        FIT_H(altinlar, HEIGHT*.4);
        
    }
    
    altinlar->setPosition(Vec2(WIDTH*.35, HEIGHT*.5));
    
    this->addChild(altinlar,20);
    
    
    //altin
    
    Label *altinLabel, *altinLabel2, *altinLabel3;
    
    if(appManager->currentLanguage() == false){
        
       
        //altinLabel = Label::createWithTTF("GUNLÜK HEDIYE", "MuseoSans.ttf", 20);
        
        altinLabel = Label::createWithBMFont("altin.fnt", "GUNLUK HEDIYE");
        
            
        //altinLabel2 = Label::createWithTTF("200", "MuseoSans.ttf", 100);
        
        altinLabel2 = Label::createWithBMFont("altin.fnt", "200");
            
        
        //altinLabel3 = Label::createWithTTF("ALTIN", "MuseoSans.ttf", 60);
        
        altinLabel3 = Label::createWithBMFont("altin.fnt", "ALTIN");
        
        
    }
    else {
        
      
        //altinLabel = Label::createWithTTF("DAILY PRESENT", "MuseoSans.ttf", 40);
        
        altinLabel = Label::createWithBMFont("altin.fnt", "DAILY PRESENT");
            
        
        //altinLabel2 = Label::createWithTTF("200", "MuseoSans.ttf", 100);
        
        altinLabel2 = Label::createWithBMFont("altin.fnt", "200");
            
        
        //altinLabel3 = Label::createWithTTF("GOLDS", "MuseoSans.ttf", 60);
        
        altinLabel3 = Label::createWithBMFont("altin.fnt", "GOLDS");
      
        
    }
    
    FIT_H(altinLabel, HEIGHT*.05);
    FIT_H(altinLabel2, HEIGHT*.27);
    FIT_H(altinLabel3, HEIGHT*.07);
    
    /*altinLabel->setTextColor(Color4B(239, 202, 44, 255));
    altinLabel2->setTextColor(Color4B(239, 202, 44, 255));
    altinLabel3->setTextColor(Color4B(239, 202, 44, 255));
     */
    
    altinLabel->setPosition(Vec2(WIDTH*.602, HEIGHT*.75));
    
    this->addChild(altinLabel,20);
    
    
    altinLabel2->setPosition(Vec2(WIDTH*.6, HEIGHT*.55));
    
    this->addChild(altinLabel2,20);
    
    altinLabel3->setPosition(Vec2(WIDTH*.6, HEIGHT*.4));
    
    this->addChild(altinLabel3,20);
    
        
    Sprite *altins = Sprite::create("Altin.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(altins, HEIGHT*.1);
            
            
            
        }
        else {
            
            FIT_H(altins, HEIGHT*.15);
            
        }
        
        
    }
    else {
    
        FIT_H(altins, HEIGHT*.15);
        
    }
    
    altins->setPosition(Vec2(WIDTH*.25, HEIGHT*.5));
        
    this->addChild(altins,20);
    
    auto tamamBut = cocos2d::ui::Button::create("tamamyeni.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tamamBut, HEIGHT*.1);
            
        }
        else {
            
            FIT_H(tamamBut, HEIGHT*.15);
            
        }
        
        
    }
    else {
        
        FIT_H(tamamBut, HEIGHT*.15);
        
    }
    
    
    tamamBut->setPosition(Vec2(WIDTH*.6, HEIGHT*.25));
    
    this->addChild(tamamBut,20);
    
    Label *tamamLabel;
    
    if(appManager->currentLanguage() == false){
    
        tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
        
    }
    
    tamamLabel->setPosition(Vec2(tamamBut->getContentSize().width/2, tamamBut->getContentSize().height/2));
    
    tamamBut->addChild(tamamLabel);
    
    tamamBut->addTouchEventListener([this,tamamBut,altins,blackBG, altinlar, altinLabel2, altinLabel, altinLabel3, panelBG](Ref *sender, ui::Widget::TouchEventType type){
        
        switch (type) {
            
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                break;
            
                
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
               
                this->endAltinAnim(blackBG, altinlar, altinLabel, altinLabel2, altins, tamamBut, panelBG, altinLabel3);
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
            
        }
        
        
    });
    
        

        
    //altins->runAction(seq);
    
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
    
}

void MenuScene::endAltinAnim(Sprite *blackBG, Sprite *altinlar, Label *altinLabel, Label *altinLabel2, Sprite *altins, Button *tamamBut, Sprite *panelBG, Label *altinLabel3){
    
    ccBezierConfig bezier;
    
    bezier.controlPoint_1 = Vec2(WIDTH*.35, HEIGHT*.45);
    
    bezier.controlPoint_2 = Vec2(WIDTH*.55, HEIGHT*.28);
    
    bezier.endPosition = Vec2(altin->getPositionX(), altin->getPositionY());
    
    auto bezierAnim = BezierTo::create(1, bezier);
    
    
    auto callback = CallFunc::create([this,blackBG, altinlar, altinLabel2, altinLabel,altins,tamamBut, panelBG, altinLabel3]{
        
        this->removeChild(blackBG);
        
        this->removeChild(altinlar);
        
        this->removeChild(altinLabel);
        
        this->removeChild(altinLabel2);
        
        this->removeChild(altins);
        
        this->removeChild(tamamBut);
        
        this->removeChild(altinLabel3);
        
        this->removeChild(panelBG);
        
    });
    
    auto seq = Sequence::create(bezierAnim, callback, NULL);
    
    altins->runAction(seq);
    
}


void MenuScene::onEnterTransitionDidFinish() {
    
    Layer::onEnterTransitionDidFinish();
    
    this->schedule(schedule_selector(MenuScene::update));
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MenuScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MenuScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MenuScene::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
}

void MenuScene::onConnectDone(int res, int rescode){
    
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
        
        printf("\nonConnectDone .. FAILED with reasonCode=%d..session=%d\n",rescode,AppWarp::AppWarpSessionID);
        
        //uyarı ve reason code
        
    }
    
}

void MenuScene::recover(float dt)
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
        
        this->schedule(schedule_selector(MenuScene::controlForConnection), 8.0f);
        
    }
    
}

void MenuScene::controlForConnection(float dt){
    
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

void MenuScene::altinRewardPanel(std::string str){
    
    ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0, 0));
    
    this->addChild(errorPanel,20);
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
            
            
        }
        else {
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
        }
        
        
    }
    else {
        
        FIT_WH(panelBG, WIDTH*.8, HEIGHT*.8);
        
    }
    
    panelBG->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel->addChild(panelBG);
    
    Label *errorLabel = Label::createWithTTF(str, "MuseoSans.ttf", 60);
    
    errorLabel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel->addChild(errorLabel);
    
    auto tamamBut = cocos2d::ui::Button::create("tamamyeni.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tamamBut, HEIGHT*.1);
            
        }
        else {
            
            FIT_H(tamamBut, HEIGHT*.15);
            
        }
        
        
    }
    else {
        
        FIT_H(tamamBut, HEIGHT*.15);
        
    }
    
    
    tamamBut->setPosition(Vec2(WIDTH*.5, HEIGHT*.25));
    
    errorPanel->addChild(tamamBut);
    
    Label *tamamLabel;
    
    if(appManager->currentLanguage() == false){
        
        tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
        
    }
    
    tamamLabel->setPosition(Vec2(tamamBut->getContentSize().width/2, tamamBut->getContentSize().height/2));
    
    tamamBut->addChild(tamamLabel);
    
    tamamBut->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
        
        switch (type) {
                
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                break;
                
                
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                
                //this->removeChangePassword();
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
                
        }
        
        
    });


    
}

void MenuScene::openRoomErrorPanel(std::string str){
    
    ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0, 0));
    
    this->addChild(errorPanel);
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.82);
            
            
            
        }
        else {
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.82);
            
        }
        
        
    }
    else {
        
        FIT_WH(panelBG, WIDTH*.8, HEIGHT*.82);
        
    }
    
    panelBG->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel->addChild(panelBG);
    
    //Label *errorLabel1 = Label::createWithTTF(str, "chinese rocks rg.ttf", 45);
    
    Label *errorLabel1 = Label::createWithBMFont("menuFont.fnt", str);
    
    FIT_H(errorLabel1, HEIGHT*.1);
    
    errorLabel1->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel->addChild(errorLabel1);
    
    
    
    auto tamamBut = cocos2d::ui::Button::create("tamamyeni.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tamamBut, HEIGHT*.1);
            
        }
        else {
            
            FIT_H(tamamBut, HEIGHT*.15);
            
        }
        
        
    }
    else {
        
        FIT_H(tamamBut, HEIGHT*.15);
        
    }
    
    
    tamamBut->setPosition(Vec2(WIDTH*.5, HEIGHT*.25));
    
    errorPanel->addChild(tamamBut);
    
    Label *tamamLabel;
    
    if(appManager->currentLanguage() == false){
        
        tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
        
    }
    
    tamamLabel->setPosition(Vec2(tamamBut->getContentSize().width/2, tamamBut->getContentSize().height/2));
    
    tamamBut->addChild(tamamLabel);
    
    tamamBut->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
        
        switch (type) {
                
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                break;
                
                
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                
                this->removeRoomErrorPanel();
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
                
        }
        
        
    });
    
}

void MenuScene::removeRoomErrorPanel() {
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel);
    
}

void MenuScene::passwordErrorPanel(std::string str){
    
    ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0, 0));
    
    this->addChild(errorPanel);
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
            
            
        }
        else {
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
        }
        
        
    }
    else {
        
        FIT_WH(panelBG, WIDTH*.8, HEIGHT*.8);
        
    }
    
    panelBG->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel->addChild(panelBG);
    
    //Label *errorLabel1 = Label::createWithTTF(str, "chinese rocks rg.ttf", 45);
    
    Label *errorLabel1 = Label::createWithBMFont("menuFont.fnt", str);
    
    FIT_H(errorLabel1, HEIGHT*.1);
    
    errorLabel1->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel->addChild(errorLabel1);
    
    
    
    auto tamamBut = cocos2d::ui::Button::create("tamamyeni.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tamamBut, HEIGHT*.1);
            
        }
        else {
            
            FIT_H(tamamBut, HEIGHT*.15);
            
        }
        
        
    }
    else {
        
        FIT_H(tamamBut, HEIGHT*.15);
        
    }
    
    
    tamamBut->setPosition(Vec2(WIDTH*.5, HEIGHT*.25));
    
    errorPanel->addChild(tamamBut);
    
    Label *tamamLabel;
    
    if(appManager->currentLanguage() == false){
    
        tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 50);
    
    }
    else {
        
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
        
    }
    
    tamamLabel->setPosition(Vec2(tamamBut->getContentSize().width/2, tamamBut->getContentSize().height/2));
    
    tamamBut->addChild(tamamLabel);
    
    tamamBut->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
        
        switch (type) {
                
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                break;
                
                
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                
                this->removeChangePassword();
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
                
        }
        
        
    });
    
    
}

void MenuScene::removeChangePasswordBefore() {
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel);
    
    this->removeChild(oldPasswordBox);
    
    this->removeChild(newPasswordBox);
    
}

void MenuScene::removeChangePassword() {
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel);
    
    this->openAyarlar();
    
}

void MenuScene::noItemWarning(std::string st1, std::string st2){
    
    ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0,0));
    
    this->addChild(errorPanel);
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
            
            
        }
        else {
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
        }
        
        
    }
    else {
    
        FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
        
    }
    
    panelBG->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel->addChild(panelBG);
    
    //Label *errorLabel1 = Label::createWithTTF(st1, "chinese rocks rg.ttf", 45);
    
    //Label *errorLabel2 = Label::createWithTTF(st2, "chinese rocks rg.ttf", 45);
    
    Label *errorLabel1 = Label::createWithBMFont("menuFont.fnt", st1);
    
    Label *errorLabel2 = Label::createWithBMFont("menuFont.fnt", st2);
    
    FIT_H(errorLabel1, HEIGHT*.1);
    
    FIT_H(errorLabel2, HEIGHT*.1);
    
    errorLabel1->setPosition(Vec2(WIDTH*.5,HEIGHT*.6));
    errorLabel2->setPosition(Vec2(WIDTH*.5,HEIGHT*.4));
    
    errorPanel->addChild(errorLabel1);
    
    errorPanel->addChild(errorLabel2);
   

    this->schedule(schedule_selector(MenuScene::removeNoItemWarning),2);
    
    
}

void MenuScene::removeNoItemWarning(float dt){
    
    this->unschedule(schedule_selector(MenuScene::removeNoItemWarning));
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel);
    
}

void MenuScene::removeNoConnectionWarning(){
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel);
    
}

void MenuScene::noConnectionWarning(std::string st1, std::string st2, std::string st3){
    
    ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0,0));
    
    this->addChild(errorPanel);
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
            
            
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
    
    /*Label *errorLabel1 = Label::createWithTTF(st1, "chinese rocks rg.ttf", 45);
    
    Label *errorLabel2 = Label::createWithTTF(st2, "chinese rocks rg.ttf", 45);
    
    Label *errorLabel3 = Label::createWithTTF(st3, "chinese rocks rg.ttf", 45);*/
    
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
    
        this->schedule(schedule_selector(MenuScene::goBackMainForError),5.0f);
        
    }
    
}

void MenuScene::goBackMainForError(float dt){
    
    this->unschedule(schedule_selector(MenuScene::goBackMainForError));
    
    //****//
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
    
    
}

void MenuScene::scheduleRecover()
{
    printf("\nHelloWorld::scheduleRecover");
    
    this->schedule(schedule_selector(MenuScene::recover), 1.0f);
    
    //showReconnectingLayer("Reconnecting ...");
}

void MenuScene::unscheduleRecover()
{
    printf("\nHelloWorld::unscheduleRecover");
    
    this->unschedule(schedule_selector(MenuScene::recover));
    
}


void MenuScene::onUserPaused(std::string user, std::string locId, bool isLobby){
    
    printf("koptum");
    
}

void MenuScene::onDisconnectDone(int res){
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(.5, scene, Color3B::BLACK));
    
}

bool MenuScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(isAvatarPanelOpened == false && isAyarlarPanelOpened == false){

    if(Avatar->getBoundingBox().containsPoint(touch->getLocation())){
            
        Avatar->setOpacity(128);

    }
    else if(Ayarlar->getBoundingBox().containsPoint(touch->getLocation())){
        
        Ayarlar->setOpacity(128);
        
    }


    
    if(actOfScene == 0){
        
        if(oyna->getBoundingBox().containsPoint(touch->getLocation())){
            
            oyna->setOpacity(128);
            
        }
        else if(magaza->getBoundingBox().containsPoint(touch->getLocation())){
            
            magaza->setOpacity(128);
            
        }
        else if(magazine->getBoundingBox().containsPoint(touch->getLocation())){
            
            magazine->setOpacity(128);
            
        }
        
        
    }
    else if(actOfScene == 1){
        
        if(olumMaci->getBoundingBox().containsPoint(touch->getLocation())){
            
            olumMaci->setOpacity(128);
            
        }
        else if(turnuva->getBoundingBox().containsPoint(touch->getLocation())){
            
            turnuva->setOpacity(128);
            
        }
        else if(tekKisi->getBoundingBox().containsPoint(touch->getLocation())){
            
            tekKisi->setOpacity(128);
            
        }
        else if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(128);
            
        }
        else if(arkadasinlaOyna->getBoundingBox().containsPoint(touch->getLocation())){
            
            arkadasinlaOyna->setOpacity(128);
            
        }
        else if(Ayarlar->getBoundingBox().containsPoint(touch->getLocation())){
            
            Ayarlar->setOpacity(128);
            
        }
        
    }
    else if(actOfScene == 2){
        
        if(odaKur->getBoundingBox().containsPoint(touch->getLocation())){
            
            odaKur->setOpacity(128);
            
        }
        else if(odaBul->getBoundingBox().containsPoint(touch->getLocation())){
            
            odaBul->setOpacity(128);
            
        }
        else if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(128);
            
        }

        
    }
    else if(actOfScene == 3){
        
        if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(128);
            
        }
        else if(arkadasTamam->getBoundingBox().containsPoint(touch->getLocation())){
            
            arkadasTamam->setOpacity(128);
            
        }

        
    }
    else if(actOfScene == 4){
        
        if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(128);
            
        }
        else if(arkadasTamam->getBoundingBox().containsPoint(touch->getLocation())){
            
            arkadasTamam->setOpacity(128);
            
        }

        
        
    }
    else if(actOfScene == 5){
        
        if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(128);
            
        }
        
        if(olumMaciSingleButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            olumMaciSingleButton->setOpacity(128);
            
        }
        
    }
        
    }
    else {
        
        if(isAvatarPanelOpened == true){
        
        Point panelAvatarPoint = panelAvatar->convertToNodeSpaceAR(touch->getLocation());
        
        if(avatarCikis->getBoundingBox().containsPoint(panelAvatarPoint)){
            
            avatarCikis->setOpacity(128);
            
        }
        
        for(int i=0; i<avatarsVector.size(); i++){
            
            if(avatarsVector.at(i)->getBoundingBox().containsPoint(panelAvatarPoint)){
                
                avatarsVector.at(i)->setOpacity(128);
                
            }
            
        }
        }
        else if(isAyarlarPanelOpened == true){
            
            Point panelAyarlarPoint = panelAyarlar->convertToNodeSpaceAR(touch->getLocation());
            
            if(xbuton->getBoundingBox().containsPoint(panelAyarlarPoint)){
                
                xbuton->setOpacity(128);
                
            }
            
            for(int i=0; i<ayarlarButonlar.size(); i++){
                
                if(ayarlarButonlar.at(i)->getBoundingBox().containsPoint(panelAyarlarPoint)){
                    
                    ayarlarButonlar.at(i)->setOpacity(128);
                    
                }
                
            }
            
            
        }
        
    }
    
    if(arti->getBoundingBox().containsPoint(touch->getLocation())){
        
        arti->setOpacity(128);
        
    }
    
    
    return true;
    
}

void MenuScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(isAvatarPanelOpened == false && isAyarlarPanelOpened == false){
        
    if(!Avatar->getBoundingBox().containsPoint(touch->getLocation())){
            
        Avatar->setOpacity(255);
    
    }
    else if(!Ayarlar->getBoundingBox().containsPoint(touch->getLocation())){
        
        Ayarlar->setOpacity(255);
        
    }


    if(actOfScene == 0){
        
        if(!oyna->getBoundingBox().containsPoint(touch->getLocation())){
            
            oyna->setOpacity(255);
            
        }
        else if(!magaza->getBoundingBox().containsPoint(touch->getLocation())){
            
            magaza->setOpacity(255);
            
        }
        else if(!magazine->getBoundingBox().containsPoint(touch->getLocation())){
            
            magazine->setOpacity(255);
            
        }
       
        
    }
    else if(actOfScene == 1){
        
        if(!olumMaci->getBoundingBox().containsPoint(touch->getLocation())){
            
            olumMaci->setOpacity(255);
            
        }
        else if(!turnuva->getBoundingBox().containsPoint(touch->getLocation())){
            
            turnuva->setOpacity(255);
            
        }
        else if(!tekKisi->getBoundingBox().containsPoint(touch->getLocation())){
            
            tekKisi->setOpacity(255);
            
        }
        else if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }
        else if(!arkadasinlaOyna->getBoundingBox().containsPoint(touch->getLocation())){
            
            arkadasinlaOyna->setOpacity(255);
            
        }
        else if(!Ayarlar->getBoundingBox().containsPoint(touch->getLocation())){
            
            Ayarlar->setOpacity(255);
            
        }
        
        
    }
    else if(actOfScene == 2){
        
        if(!odaKur->getBoundingBox().containsPoint(touch->getLocation())){
            
            odaKur->setOpacity(255);
            
        }
        else if(!odaBul->getBoundingBox().containsPoint(touch->getLocation())){
            
            odaBul->setOpacity(255);
            
        }
        else if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }

    }
    else if(actOfScene == 3){
        
        if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }
        else if(!arkadasTamam->getBoundingBox().containsPoint(touch->getLocation())){
            
            arkadasTamam->setOpacity(255);
            
        }
        
    }
    else if(actOfScene == 4){
        
        if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }
        else if(!arkadasTamam->getBoundingBox().containsPoint(touch->getLocation())){
            
            arkadasTamam->setOpacity(255);
            
        }

        
    }
    else if(actOfScene == 5){
        
        if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }
        
        if(!olumMaciSingleButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            olumMaciSingleButton->setOpacity(255);
            
        }
        
    }
        
    }
    else {
        
        if(isAvatarPanelOpened == true){
        
        Point panelAvatarPoint = panelAvatar->convertToNodeSpaceAR(touch->getLocation());
        
        if(!avatarCikis->getBoundingBox().containsPoint(panelAvatarPoint)){
            
            avatarCikis->setOpacity(255);
            
        }
        
        for(int i=0; i<avatarsVector.size(); i++){
            
            if(!avatarsVector.at(i)->getBoundingBox().containsPoint(panelAvatarPoint)){
                
                avatarsVector.at(i)->setOpacity(255);
                
            }
            
        }
            
        }
        else if(isAyarlarPanelOpened == true){
            
            Point panelAyarlarPoint = panelAyarlar->convertToNodeSpaceAR(touch->getLocation());
            
            if(!xbuton->getBoundingBox().containsPoint(panelAyarlarPoint)){
                
                xbuton->setOpacity(255);
                
            }
            
            for(int i=0; i<ayarlarButonlar.size(); i++){
                
                if(!ayarlarButonlar.at(i)->getBoundingBox().containsPoint(panelAyarlarPoint)){
                    
                    ayarlarButonlar.at(i)->setOpacity(255);
                    
                }
                
            }
            
            
        }

        
    }

    
    if(!arti->getBoundingBox().containsPoint(touch->getLocation())){
        
        arti->setOpacity(255);
        
    }
    
    
}

void MenuScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(isAvatarPanelOpened == false && isAyarlarPanelOpened == false){
        
    if(Avatar->getOpacity() == 128){
        
        if(appManager->fx == true){
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
        }
        
        Avatar->setOpacity(255);
            
        this->openAvatarPanel();
        
    }
    else if(Ayarlar->getOpacity() == 128){
        
        if(appManager->fx == true){
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
        }
        
        if(isAyarlarPanelOpened == false){
        
            Ayarlar->setOpacity(255);
        
            openAyarlar();
            
        }
        
    }

    
    if(actOfScene == 0){
        
        if(oyna->getOpacity() == 128){
            
            if(appManager->fx == true){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            //oyna
            oyna->setOpacity(255);
            removeMainMenu();
            createOynaMenu();
            actOfScene = 1; //oyun özellikler
            
        }
        else if(magaza->getOpacity() == 128){
            
            if(appManager->fx == true){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            //magaza ya gidecek scene değişecek
            magaza->setOpacity(255);
            gotoMagaza();
            
            
        }
        else if(magazine->getOpacity() == 128){
            
            if(appManager->fx == true){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            //magazine e gidecek scene değişecek
            magazine->setOpacity(255);
            
            
        }
       
        
    }
    else if(actOfScene == 1){
        
        if(olumMaci->getOpacity() == 128){
            
            if(appManager->fx == true){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            olumMaci->setOpacity(255);
            
            appManager->kindOfGame = "olumMaci";
            
            //olummaci
            
            createOlumMaciMenu();
            
            removeOynaMenu();
            
            //auto scene = GameScene::createScene();
            
            //Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
            
        }
        else if(turnuva->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            turnuva->setOpacity(255);
            
            appManager->kindOfGame = "turnuva";
            
            appManager->levelForRoom = "";
            appManager->goldForRoom = "";
            appManager->turnuvaName = "";
            appManager->roomName = "";
            appManager->roomPassword = "";

            
            auto scene = TurnuvaScene::createScene();
            
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
            
        }
        else if(tekKisi->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            tekKisi->setOpacity(255);
            
            appManager->kindOfGame = "tekKisi";
            
            appManager->levelForRoom = "";
            appManager->goldForRoom = "";
            appManager->turnuvaName = "";
            appManager->roomName = "";
            appManager->roomPassword = "";
            
            appManager->limitGold = 25;
            
            auto scene = LoadingGameScene::createScene();
            
            Director::getInstance()->replaceScene(TransitionShrinkGrow::create(.5, scene));
            
        }
        else if(arkadasinlaOyna->getOpacity() == 128)
        {
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            arkadasinlaOyna->setOpacity(255);
            
            appManager->kindOfGame = "arkadas";
            appManager->levelForRoom = "";
            appManager->goldForRoom = "";
            appManager->turnuvaName = "";
            appManager->roomName = "";
            appManager->roomPassword = "";

            
            removeOynaMenu();
            
            createArkadaslaOynaMenu();
            
            
            
        }
        else if(backButton->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                
            }
            
            actOfScene = 0;
            backButton->setOpacity(255);
            
            backButton->setOpacity(255);
            
            removeOynaMenu();
            
            createMainMenu();
            
        }
        else if(Ayarlar->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            Ayarlar->setOpacity(255);
            
            //ayarlar menü
            
            openAyarlar();
            
        }
        
    }
    else if(actOfScene == 2){
        
        if(odaKur->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            //oda kur
            actOfScene = 3;
            
            this->removeChild(odaKur);
            this->removeChild(odaBul);
            
            this->arkadasOdaKur();
            
        }
        else if(odaBul->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            //oda bul
            actOfScene = 4;
            
            this->removeChild(odaKur);
            this->removeChild(odaBul);
            
            this->arkadasOdaBul();
            
        }
        else if(backButton->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                
            }
            
            this->removeChild(odaKur);
            this->removeChild(odaBul);
            
            backButton->setOpacity(255);
            
            actOfScene = 1;
            
            createOynaMenu();
            
        }
        else if(Ayarlar->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            Ayarlar->setOpacity(255);
            
            //ayarlar menü
            
            openAyarlar();
            
        }

        
    }
    else if(actOfScene == 3){
        
        if(backButton->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                
            }
            
            this->removeChild(arkadasTamam);
            this->removeChild(odaKurBulPanel);
            //this->removeChild(odaKurtext);
            this->removeChild(odaKurLabel);
            this->removeChild(roomName);
            this->removeChild(roomPassword);
            
            backButton->setOpacity(255);
            
            actOfScene = 2;
            
            createArkadaslaOynaMenu();
            
            
        }
        else if(arkadasTamam->getOpacity() == 128){
            
            arkadasTamam->setOpacity(255);
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            roomNameLabel = cocos2d::__String::create(roomName->getText());
            roomPasswordLabel = cocos2d::__String::create(roomPassword->getText());
            
            if(strcmp(roomNameLabel->getCString(), "") != 0 && strcmp(roomPasswordLabel->getCString(), "") != 0){
            
                appManager->roomName = roomNameLabel->getCString();
                appManager->roomPassword = roomPasswordLabel->getCString();
            
                auto scene = LoadingGameScene::createScene();
            
                Director::getInstance()->replaceScene(TransitionShrinkGrow::create(.5, scene));
                
            }
            else {
                
                if(appManager->currentLanguage() == false){
                    
                    openRoomErrorPanel("ODA ADI VE ŞİFRE GEREKLİ");
                    
                }
                else {
                    
                    openRoomErrorPanel("ROOM NAME AND PASSWORD REQUIRED");
                    
                }
                
            }

            
        }
        
    }
    else if(actOfScene == 4){
        
        if(backButton->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                
            }
            
            this->removeChild(arkadasTamam);
            this->removeChild(odaKurBulPanel);
            //this->removeChild(odaBultext);
            this->removeChild(odaBulLabel);
            this->removeChild(roomName);
            this->removeChild(roomPassword);
            
            backButton->setOpacity(255);
            
            actOfScene = 2;
            
            createArkadaslaOynaMenu();
            
        }
        else if(arkadasTamam->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            roomNameLabel = cocos2d::__String::create(roomName->getText());
            roomPasswordLabel = cocos2d::__String::create(roomPassword->getText());
            
            if(strcmp(roomNameLabel->getCString(), "") != 0 && strcmp(roomPasswordLabel->getCString(), "") != 0){
            
                 appManager->roomName = roomNameLabel->getCString();
                
                 appManager->roomPassword = roomPasswordLabel->getCString();
                
                 auto scene = LoadingGameScene::createScene();
                
                 Director::getInstance()->replaceScene(TransitionShrinkGrow::create(.5, scene));
                
            }
            else {
                
                if(appManager->currentLanguage() == false){
                    
                    openRoomErrorPanel("ODA ADI VE ŞİFRE GEREKLİ");
                    
                }
                else {
                    
                    openRoomErrorPanel("ROOM NAME AND PASSWORD REQUIRED");
                    
                }

            }
            
        }
        
    }
    else if(actOfScene == 5){
        
        if(backButton->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                
            }
            
            actOfScene = 1;
            
            backButton->setOpacity(255);
            
            removeOlumMaciMenu();
            
            createOynaMenu();
            
        }
        
        if(olumMaciSingleButton->getOpacity() == 128){
            
            olumMaciSingleButton->setOpacity(255);
            
            this->startOlumMaci();
            
        }
        
    }
        
    }
    else {
        
        if(isAvatarPanelOpened == true){
        
        
        if(avatarCikis->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                
            }
            
            avatarCikis->setOpacity(255);
            
            auto runAndKillAvatarPanel = MoveTo::create(.5,Vec2(-WIDTH, 0));
            
            auto callBack = CallFunc::create([&]{
                
                this->removeChild(panelAvatar);
                
                isAvatarPanelOpened = false;
                
            });
            
            auto seq = Sequence::create(runAndKillAvatarPanel,callBack, NULL);
            
            panelAvatar->runAction(seq);
            
        }
        
        for(int i=0; i<avatarsVector.size(); i++){
            
            if(avatarsVector.at(i)->getOpacity() == 128){
                
                if(appManager->fx == true){
                
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                    
                }
                
                userAvatarData = UserDefault::getInstance();
                
                userAvatarData->setIntegerForKey("avatar", (int)i);
                
                userAvatarData->flush();
                
                appManager->avatarNumber = i;
                
                this->updateUserForAvatar(appManager->username.c_str(), appManager->gold, appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
                
                userAvatar->setTexture(avatarsVector.at(i)->getTexture());
                
                auto runAndKillAvatarPanel = MoveTo::create(.5,Vec2(-WIDTH, 0));

                auto callBack = CallFunc::create([&]{
                    
                    this->removeChild(panelAvatar);
                    
                    isAvatarPanelOpened = false;
                    
                });
                
                auto seq = Sequence::create(runAndKillAvatarPanel,callBack, NULL);
                
                panelAvatar->runAction(seq);
                
            }
            
        }
        
        }
        else if(isAyarlarPanelOpened == true){
            
            if(xbuton->getOpacity() == 128){
                
                xbuton->setOpacity(255);
                
                this->removeChild(panelAyarlar);
                
                this->removeChild(ayarlarPerde);
                
                isAyarlarPanelOpened = false;
                
            }
            
            for(int i=0; i<ayarlarButonlar.size(); i++){
                
                if(ayarlarButonlar.at(i)->getOpacity() == 128){
                    
                    switch (i) {
                            
                        case 0:
                            //Ses
                            
                            if(appManager->fx == true){
                                
                                appManager->fx = false;
                                
                                ses->setTexture("seskapali.png");
                            
                                
                            }
                            else {
                                
                                appManager->fx = true;
                                
                                ses->setTexture("ses.png");
                                
                            }
                            
                            break;
                        case 1:
                            //müzik
                            
                            if(appManager->sound == true){
                                
                                appManager->sound = false;
                                
                                muzik->setTexture("muzikkapali.png");
                                
                                CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
                                
                            }
                            else {
                                
                                appManager->sound = true;
                                
                                muzik->setTexture("muzik.png");
                                
                                CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
                                
                                
                            }
                            
                            
                            break;
                        case 2:
                            //ekip
                            
                            this->removeEkip();
                            
                            
                            
                            break;
                            
                        case 3:
                            
                            //howtoplay
                            
                            this->removeChild(panelAyarlar);
                            
                            this->removeChild(ayarlarPerde);
                            
                            isAyarlarPanelOpened = false;
                            
                            
                            //burda ŞİFRE DEĞİŞTİRME PANELİ AÇILACAK.
                            //panelde değişiklikler yapılıp tamama basılınca ->appwarp change password çağırılacak. username, old, new password alınacak.
                            //loader eklenecek. Response döndüğü an bu panel kapatılacak.
                            //password response da dönen duruma göre passwordErrorPanel açılacak.
                            //bu panelde tamama basılınca password error panel kapatılacak. ve openAyarlar çağırılacak.
                            
                            //YANİ BURADA ŞİFRE DEĞİŞTİRME PANEL FONKSİYONU ÇAĞIRILACAK. !!! FONKSİYONU YAZ!! ??? !!111!!11
                            
                            this->openChangePasswordPanel();
                            
                            break;
                            
                        case 4:
                            //çıkış
                            
                            //--- CCUserDefault
                            
                            UserDefault *userDefaults = UserDefault::getInstance();
                            
                            userDefaults->setBoolForKey("autoLogin", false);
                            userDefaults->setStringForKey("username", "");
                            userDefaults->setStringForKey("password", "");
                            userDefaults->flush();
                            
                            //--- Appwarp
                            
                            AppWarp::Client *warpClientRef;
                            warpClientRef = AppWarp::Client::getInstance();
                            warpClientRef->setConnectionRequestListener(this);
                            warpClientRef->disconnect();
                            
                            
                            break;
                            
                    }
                    
                    ayarlarButonlar.at(i)->setOpacity(255);
                    
                    
                }
                
            }
            
            
        }

        
    }
    
    if(arti->getOpacity() == 128){
        
        arti->setOpacity(255);
        this->gotoMagaza();
        
    }
    
    
    
}

void MenuScene::openChangePasswordPanel(){
    
    ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0, 0));
    
    this->addChild(errorPanel);
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
            
            
            
        }
        else {
            
            FIT_WH(panelBG, WIDTH*.8, HEIGHT*.8);
            
        }
        
        
    }
    else {
        
        FIT_WH(panelBG,WIDTH*.8 ,HEIGHT*.8);
        
    }
    
    panelBG->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel->addChild(panelBG);
    
    Label *errorLabel1;
    
    if(appManager->currentLanguage() == false){
    
        errorLabel1 = Label::createWithTTF("ŞİFRE DEĞİŞTİR", "chinese rocks rg.ttf", 45);
        
    }
    else {
        
        errorLabel1 = Label::createWithTTF("CHANGE PASSWORD", "chinese rocks rg.ttf", 45);
        
    }
    
    errorLabel1->setPosition(Vec2(WIDTH*.5,HEIGHT*.8));
    
    errorPanel->addChild(errorLabel1);
    
    
    
    auto tamamBut = cocos2d::ui::Button::create("tamamyeni.png");
    
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tamamBut, HEIGHT*.1);
            
        }
        else {
            
            FIT_H(tamamBut, HEIGHT*.15);
            
        }
        
        
    }
    else {
        
        FIT_H(tamamBut, HEIGHT*.15);
        
    }
    
    oldPasswordBox = EditBox::create(Size(WIDTH*.3, HEIGHT*.1), "arkadasBack.png");
    
    oldPasswordBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    oldPasswordBox->setPosition(Vec2(WIDTH*.5, HEIGHT*.55));
    oldPasswordBox->setFontSize(30);
    oldPasswordBox->setFontColor(Color3B::WHITE);
    oldPasswordBox->setMaxLength(15);
    const char* placePassOld;
    if(appManager->currentLanguage() == false){
        placePassOld = "Eski Şifre";
    }
    else {
        placePassOld = "Old Password";
    }
    oldPasswordBox->setPlaceHolder(placePassOld);
    oldPasswordBox->setPlaceholderFontSize(30);
    oldPasswordBox->setPlaceholderFontColor(Color3B::WHITE);
    oldPasswordBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    oldPasswordBox->setReturnType(EditBox::KeyboardReturnType::DONE);
    oldPasswordBox->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    oldPasswordBox->setDelegate(this);
    
    this->addChild(oldPasswordBox);
    
    newPasswordBox = EditBox::create(Size(WIDTH*.3, HEIGHT*.1), "arkadasBack.png");
    newPasswordBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    newPasswordBox->setPosition(Vec2(WIDTH*.5, HEIGHT*.4));
    newPasswordBox->setFontColor(Color3B::WHITE);
    newPasswordBox->setMaxLength(15);
    const char* placePassNew;
    if(appManager->currentLanguage() == false){
        placePassNew = "Yeni Şifre";
    }
    else{
        placePassNew = "New Password";
    }
    newPasswordBox->setPlaceHolder(placePassNew);
    newPasswordBox->setPlaceholderFontSize(30);
    newPasswordBox->setPlaceholderFontColor(Color3B::WHITE);
    newPasswordBox->setReturnType(EditBox::KeyboardReturnType::DONE);
    newPasswordBox->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    newPasswordBox->setDelegate(this);
    
    this->addChild(newPasswordBox);
    

    
    tamamBut->setPosition(Vec2(WIDTH*.5, HEIGHT*.25));
    
    errorPanel->addChild(tamamBut);
    
    Label *tamamLabel;
    
    if(appManager->currentLanguage() == false){
    
        tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
        
    }
    
    tamamLabel->setPosition(Vec2(tamamBut->getContentSize().width/2, tamamBut->getContentSize().height/2));
    
    tamamBut->addChild(tamamLabel);
    
    tamamBut->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
        
        switch (type) {
                
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                break;
                
                
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                
                old_temp_password = oldPasswordBox->getText();
                
                temp_password = newPasswordBox->getText();
                
                if(temp_password.length() < 5 || old_temp_password.length() < 5){
                    
                    this->removeChangePasswordBefore();
                    
                    if(appManager->currentLanguage() == false){
                    
                        this->passwordErrorPanel("Şifre en az 5 karakter olmalıdır");
                        
                    }
                    else {
                        
                        this->passwordErrorPanel("Password must have at least 5 characters");
                        
                    }
                    
                }
                else {
                    
                    this->changePassword(appManager->username.c_str(), oldPasswordBox->getText(), newPasswordBox->getText());
                    
                }
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
                
        }
        
        
    });
    
    

    
    
}

void MenuScene::removeEkip(){
    
    auto ekipSprite = Sprite::create("Ekip-Turkce.png");
    
    FIT_H(ekipSprite, HEIGHT*.9);
    
    ekipSprite->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(ekipSprite);
    
    auto ekipButton = cocos2d::ui::Button::create("xbuton.png");
    
    FIT_H(ekipButton, HEIGHT*.2);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
    
        ekipButton->setPosition(Vec2(WIDTH*.5 + ekipSprite->getContentSize().width/2, HEIGHT*.5 + ekipSprite->getContentSize().height/2));
    
    }
    else {
        
        ekipButton->setPosition(Vec2(WIDTH*.8, HEIGHT*.83));
        
    }
        
    this->addChild(ekipButton);
    
    ekipButton->addTouchEventListener([&,this,ekipSprite, ekipButton](Ref *sender, ui::Widget::TouchEventType type){
        
        
        switch (type) {
            
            case ui::Widget::TouchEventType::BEGAN:
                
                break;
                
            case ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case ui::Widget::TouchEventType::ENDED:
                
                this->removeChild(ekipSprite);
                
                this->removeChild(ekipButton);
                
                break;
                
            case ui::Widget::TouchEventType::MOVED:
                
                break;
        }
        
    });
    
    
}

void MenuScene::openAyarlar(){
    
    isAyarlarPanelOpened = true;
    
    ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    isAyarlarPanelOpened = true;
    
    panelAyarlar = Node::create();
    
    panelAyarlar->setPosition(Vec2(0,0));
    
    ayarlarOnPanel = Sprite::create("ayarlarpanel.png");
    
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(ayarlarOnPanel,WIDTH*.8 ,HEIGHT*.8);
            
            
            
        }
        else {
            
            FIT_H(ayarlarOnPanel, HEIGHT*.8);
            
        }
        
        
    }
    else {
    
        FIT_H(ayarlarOnPanel, HEIGHT*.8);
        
    }
    
    ayarlarOnPanel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    panelAyarlar->addChild(ayarlarOnPanel);
    
    xbuton = Sprite::create("xbuton.png");
    
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(xbuton,HEIGHT*.13);
            
             xbuton->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.61,ayarlarOnPanel->getContentSize().height*.7));
            
        }
        else {
            
            FIT_H(xbuton, HEIGHT*.2);
            
            xbuton->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.65,ayarlarOnPanel->getContentSize().height*.55));
            
        }
        
        
    }
    else {
    
        FIT_H(xbuton, HEIGHT*.2);
        
        xbuton->setPosition(Vec2(WIDTH*.8 , HEIGHT*.83));
        
    }
    
   
    
    xbuton->setZOrder(10);
    
    panelAyarlar->addChild(xbuton);
    
    this->addChild(panelAyarlar);
    
    if(appManager->fx == true){
    
        ses = Sprite::create("ses.png");
        
        FIT_H(ses, HEIGHT*.15);
        
    }
    else {
        
        ses = Sprite::create("seskapali.png");
        
        FIT_H(ses, HEIGHT*.15);
    
    }
    
    
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            ses->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.2,ayarlarOnPanel->getContentSize().height*.58));

            
        }
        else {
            
            ses->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.25,ayarlarOnPanel->getContentSize().height*.45));
            
        }
        
        
    }
    else {
    
        ses->setPosition(Vec2(WIDTH*.3, HEIGHT*.7));
        
    }
    
    ses->setZOrder(10);
    
    panelAyarlar->addChild(ses);
    
    ayarlarButonlar.pushBack(ses);
    
    
    
    Label *sesLabel, *muzikLabel, *ekipLabel, *howLabel, *cikisLabel;
    
    if(appManager->currentLanguage() == false){
        
        sesLabel = Label::createWithTTF("SES", "chinese rocks rg.ttf", 50);
        
        muzikLabel = Label::createWithTTF("MÜZİK", "chinese rocks rg.ttf", 50);
        
        ekipLabel = Label::createWithTTF("EKİP", "chinese rocks rg.ttf", 50);

        howLabel = Label::createWithTTF("ŞİFRE DEĞİŞTİR", "chinese rocks rg.ttf", 50);
        
        cikisLabel = Label::createWithTTF("ÇIKIŞ YAP", "chinese rocks rg.ttf", 50);


        
    }
    else {
        
        sesLabel = Label::createWithTTF("SOUND", "chinese rocks rg.ttf", 50);
        
        muzikLabel = Label::createWithTTF("MUSIC", "chinese rocks rg.ttf", 50);
        
        ekipLabel = Label::createWithTTF("TEAM", "chinese rocks rg.ttf", 50);

        howLabel = Label::createWithTTF("CHANGE PASSWORD", "chinese rocks rg.ttf", 50);
        
         cikisLabel = Label::createWithTTF("EXIT", "chinese rocks rg.ttf", 50);

    }
    
    
    
    sesLabel->setPosition(Vec2(ses->getContentSize().width/2, -ses->getContentSize().height/2.6));
        
 
    
    ses->addChild(sesLabel);
    
    if(appManager->sound == true){
    
        muzik = Sprite::create("muzik.png");
        
        FIT_H(muzik, HEIGHT*.15);
    
    }
    else {
        
        muzik = Sprite::create("muzikkapali.png");
        
        FIT_H(muzik, HEIGHT*.15);
        
    }
    

    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            muzik->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.35,ayarlarOnPanel->getContentSize().height*.58));
            
            
        }
        else {
            
            muzik->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.4,ayarlarOnPanel->getContentSize().height*.45));
            
        }
        
        
    }
    else {
    
        muzik->setPosition(Vec2(WIDTH*.5 , HEIGHT*.7));
        
    }
    
    muzik->setZOrder(10);
    
    panelAyarlar->addChild(muzik);
 
    
    muzikLabel->setPosition(Vec2(muzik->getContentSize().width/2, -muzik->getContentSize().height/2.6));
    
    muzik->addChild(muzikLabel);

    
    ekip = Sprite::create("ekip.png");
    
    ayarlarButonlar.pushBack(muzik);
    
    FIT_H(ekip, HEIGHT*.15);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            ekip->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.5,ayarlarOnPanel->getContentSize().height*.58));
            
            
        }
        else {
            
            ekip->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.55,ayarlarOnPanel->getContentSize().height*.45));
            
        }
        
        
    }
    else {
    
        ekip->setPosition(Vec2(WIDTH*.7, HEIGHT*.7));
        
    }
    
    ekip->setZOrder(10);
    
    panelAyarlar->addChild(ekip);
    
    
    ekipLabel->setPosition(Vec2(ekip->getContentSize().width/2, -ekip->getContentSize().height/2.6));
    
    ekip->addChild(ekipLabel);

    
    sifreDegistir = Sprite::create("sifredegistir.png");
    
    ayarlarButonlar.pushBack(ekip);
    
    FIT_H(sifreDegistir, HEIGHT*.15);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            sifreDegistir->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.27,ayarlarOnPanel->getContentSize().height*.28));
            
            
        }
        else {
            
            sifreDegistir->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.33,ayarlarOnPanel->getContentSize().height*.28));
            
        }
        
        
    }
    else {
    
        sifreDegistir->setPosition(Vec2(WIDTH*.4, HEIGHT*.4));
        
    }
    
    sifreDegistir->setZOrder(10);
    
    
    howLabel->setPosition(Vec2(sifreDegistir->getContentSize().width/2, -sifreDegistir->getContentSize().height/2.6));
    
    sifreDegistir->addChild(howLabel);

    
    panelAyarlar->addChild(sifreDegistir);
    
    ayarlarCikis = Sprite::create("ayarlarcikis.png");
    
    ayarlarButonlar.pushBack(sifreDegistir);
    
    FIT_H(ayarlarCikis, HEIGHT*.15);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            ayarlarCikis->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.42,ayarlarOnPanel->getContentSize().height*.28));
            
            
        }
        else {
            
            ayarlarCikis->setPosition(Vec2(ayarlarOnPanel->getContentSize().width*.47,ayarlarOnPanel->getContentSize().height*.28));
            
        }
        
        
    }
    else {
    
        ayarlarCikis->setPosition(Vec2(WIDTH*.6, HEIGHT*.4));
        
    }
    
    ayarlarCikis->setZOrder(10);
    
    panelAyarlar->addChild(ayarlarCikis);
    
    
    cikisLabel->setPosition(Vec2(ayarlarCikis->getContentSize().width/2, -ayarlarCikis->getContentSize().height/2.4));
    
    ayarlarCikis->addChild(cikisLabel);

    
    ayarlarButonlar.pushBack(ayarlarCikis);
    
}


//avatar panel

void MenuScene::openAvatarPanel(){
    
    isAvatarPanelOpened = true;
    
    panelAvatar = Node::create();
    
    panelAvatar->setPosition(Vec2(-WIDTH,0));
    
    Sprite *panelAvatarBG = Sprite::create("Avatar Panel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_WH(panelAvatarBG, WIDTH*.9, HEIGHT*.9);
            
        }
        else {
            
            FIT_H(panelAvatarBG, HEIGHT*.95);
            
        }
        
        
    }
    else {
    
        FIT_H(panelAvatarBG, HEIGHT*.95);
        
    }
    
    panelAvatarBG->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    panelAvatar->addChild(panelAvatarBG);
    
    Label *avatarLabelPanel = Label::createWithBMFont("menuFont.fnt", "AVATAR");
    
    FIT_H(avatarLabelPanel, HEIGHT*.08);
    
    avatarLabelPanel->setPosition(Vec2(WIDTH*.5, HEIGHT*.87));
    
    panelAvatar->addChild(avatarLabelPanel);
    
    avatarCikis = Sprite::create("avatarcikis.png");
    
    FIT_H(avatarCikis, HEIGHT*.17);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
             avatarCikis->setPosition(Vec2(panelAvatarBG->getContentSize().width*.5,panelAvatarBG->getContentSize().height*.65));
            
        }
        else {
            
            avatarCikis->setPosition(Vec2(panelAvatarBG->getContentSize().width*.58,panelAvatarBG->getContentSize().height*.55));
            
        }
        
        
    }
    else {
    
        avatarCikis->setPosition(Vec2(WIDTH*.9,HEIGHT*.9));
        
    }
    
    avatarCikis->setZOrder(10);
    
    panelAvatar->addChild(avatarCikis);
    
    auto callBack = CallFunc::create([&]{
        
        
    });
    
    this->addChild(panelAvatar,50);
    
    this->createAvatarsPanel();
    
    auto moveAction = MoveTo::create(.5, Vec2(0,0));
    
    auto seq = Sequence::create(moveAction,callBack, NULL);
    
    panelAvatar->runAction(seq);
    
}

void MenuScene::createAvatarsPanel(){
    
    float heightOfAvatars = HEIGHT*.7;
    
    for(int i = 0; i < 15; i++){
        
        std::string avatarTipi;
        
        avatarTipi = "avatar " + StringUtils::format("%d",(i)) + ".png";
        
        avatarOnPanel = Sprite::create(avatarTipi);
        
        avatarsVector.pushBack(avatarOnPanel);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                
                FIT_H(avatarOnPanel, HEIGHT*.15);
                
            }
            else {
                
                FIT_H(avatarOnPanel, HEIGHT*.18);
                
            }
            
            
        }
        else {
        
            FIT_H(avatarOnPanel, HEIGHT*.18);
            
        }
        
        if(i%5 == 0){
            
            if(i != 0){
            
                heightOfAvatars -= HEIGHT*.22;
                
            }
            
        }
        
        avatarOnPanel->setPosition(Vec2(WIDTH*.2 + i%5*WIDTH*.15, heightOfAvatars));
        
        panelAvatar->addChild(avatarOnPanel);
        
    }

}

void MenuScene::arkadasOdaKur(){
    
    odaKurBulPanel = Sprite::create("arkadasPanel.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(odaKurBulPanel, HEIGHT*.8);
            
                odaKurBulPanel->setPosition(Vec2(WIDTH*.5,HEIGHT*.45));
            
        }
        else {
            
            FIT_H(odaKurBulPanel, HEIGHT*.8);
            
            odaKurBulPanel->setPosition(Vec2(WIDTH*.5,HEIGHT*.4));
            
        }
        
        
    }
    else {
    
        FIT_H(odaKurBulPanel, HEIGHT*.8);
        
            odaKurBulPanel->setPosition(Vec2(WIDTH*.5,HEIGHT*.4));
        
    }

    this->addChild(odaKurBulPanel);
    
    /*odaKurtext = Sprite::create("odaKurtext.png");
    FIT_H(odaKurtext, HEIGHT*.1);
    odaKurtext->setPosition(Vec2(WIDTH*.5, HEIGHT*.7));
    this->addChild(odaKurtext);*/
    
    if(appManager->currentLanguage() == false){
        odaKurLabel = Label::createWithTTF("ODA KUR", "chinese rocks rg.ttf", 57);
    }
    else {
        odaKurLabel = Label::createWithTTF("CREATE ROOM", "chinese rocks rg.ttf", 57);
    }
    odaKurLabel->setPosition(Vec2(WIDTH*.5, HEIGHT*.69));
    this->addChild(odaKurLabel);
    
    
    //roomName= EditBox::create(Size(odaKurBulPanel->getContentSize().width*.35,HEIGHT*.1), Scale9Sprite::create("arkadasBack.png"));
    roomName= EditBox::create(Size(WIDTH*.25,HEIGHT*.1), Scale9Sprite::create("arkadasBack.png"));
    roomName->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    roomName->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    roomName->setFontSize(HEIGHT*.05);
    roomName->setFontColor(Color3B::WHITE);
    roomName->setMaxLength(15);
    const char *placeroomname;
    if(appManager->currentLanguage() == false){
        placeroomname = "Oda Adı";
    }
    else {
        placeroomname = "Room Name";
    }
    roomName->setPlaceHolder(placeroomname);
    roomName->setPlaceholderFontSize(HEIGHT*.05);
    roomName->setPlaceholderFontColor(Color3B::WHITE);
    
    roomName->setReturnType(EditBox::KeyboardReturnType::DONE);
    roomName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    roomName->setDelegate(this);
    
    //roomPassword = EditBox::create(Size(odaKurBulPanel->getContentSize().width*.35,HEIGHT*.1), Scale9Sprite::create("arkadasBack.png"));
    roomPassword = EditBox::create(Size(WIDTH*.25,HEIGHT*.1), Scale9Sprite::create("arkadasBack.png"));
    roomPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    roomPassword->setPosition(Vec2(WIDTH*.5,HEIGHT*.35));
    roomPassword->setFontSize(HEIGHT*.05);
    roomPassword->setFontColor(Color3B::WHITE);
    roomPassword->setMaxLength(15);
    const char *placeroompass;
    if(appManager->currentLanguage() == false){
        placeroompass = "Oda Şifresi";
    }
    else {
        placeroompass = "Room Password";
    }
    roomPassword->setPlaceHolder(placeroompass);
    roomPassword->setPlaceholderFontSize(HEIGHT*.05);
    roomPassword->setPlaceholderFontColor(Color3B::WHITE);
    
    roomPassword->setReturnType(EditBox::KeyboardReturnType::DONE);
    roomPassword->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    roomPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    roomPassword->setDelegate(this);
    this->addChild(roomPassword);

    
    this->addChild(roomName);
    
    
    //arkadas tamam button
    
    arkadasTamam = Sprite::create("tamamArkadaslaOyna.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(arkadasTamam, HEIGHT*.15);
            
            arkadasTamam->setPosition(Vec2(WIDTH*.5,HEIGHT*.17));
            
        }
        else {
            
            FIT_H(arkadasTamam, HEIGHT*.16);
            arkadasTamam->setPosition(Vec2(WIDTH*.5,HEIGHT*.17));
            
        }
        
        
    }
    else {
    
        FIT_H(arkadasTamam, HEIGHT*.16);
        arkadasTamam->setPosition(Vec2(WIDTH*.5,HEIGHT*.17));
        
    }
    this->addChild(arkadasTamam);
    
    if(appManager->currentLanguage() == false){
        arkadasTamamLabel = Label::createWithTTF("Tamam", "chinese rocks rg.ttf", 45);
    }
    else {
        arkadasTamamLabel = Label::createWithTTF("Ok", "chinese rocks rg.ttf", 45);
    }
    arkadasTamam->addChild(arkadasTamamLabel);
    arkadasTamamLabel->setPosition(Vec2(arkadasTamam->getContentSize().width/2, arkadasTamam->getContentSize().height/2));
    
    
}

void MenuScene::arkadasOdaBul(){
    
    odaKurBulPanel = Sprite::create("arkadasPanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(odaKurBulPanel, HEIGHT*.8);
            
            odaKurBulPanel->setPosition(Vec2(WIDTH*.5,HEIGHT*.45));
            
        }
        else {
            
            FIT_H(odaKurBulPanel, HEIGHT*.8);
            
            odaKurBulPanel->setPosition(Vec2(WIDTH*.5,HEIGHT*.4));
            
        }
        
        
    }
    else {
        
        FIT_H(odaKurBulPanel, HEIGHT*.8);
    
        odaKurBulPanel->setPosition(Vec2(WIDTH*.5,HEIGHT*.4));
        
    }
    this->addChild(odaKurBulPanel);
    
    /*odaBultext = Sprite::create("odaBultext.png");
    FIT_H(odaBultext, HEIGHT*.1);
    odaBultext->setPosition(Vec2(WIDTH*.5, HEIGHT*.7));
    this->addChild(odaBultext);*/
    
    if(appManager->currentLanguage() == false){
    
        odaBulLabel = Label::createWithTTF("ODA BUL", "chinese rocks rg.ttf", 57);
        
    }
    else {

        odaBulLabel = Label::createWithTTF("FIND ROOM", "chinese rocks rg.ttf", 57);

    }
    odaBulLabel->setPosition(Vec2(WIDTH*.5, HEIGHT*.69));
    this->addChild(odaBulLabel);
    
    roomName= EditBox::create(Size(WIDTH*.25,HEIGHT*.1), Scale9Sprite::create("arkadasBack.png"));
    roomName->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    roomName->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    roomName->setFontSize(HEIGHT*.05);
    roomName->setFontColor(Color3B::WHITE);
    roomName->setMaxLength(15);
    const char *placeroomname;
    if(appManager->currentLanguage() == false){
        placeroomname = "Oda Adı";
    }
    else {
        placeroomname = "Room Name";
    }
    roomName->setPlaceHolder(placeroomname);
    roomName->setPlaceholderFontSize(HEIGHT*.05);
    roomName->setPlaceholderFontColor(Color3B::WHITE);
    
    roomName->setReturnType(EditBox::KeyboardReturnType::DONE);
    roomName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    roomName->setDelegate(this);
    
    roomPassword = EditBox::create(Size(WIDTH*.25,HEIGHT*.1), Scale9Sprite::create("arkadasBack.png"));
    roomPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    roomPassword->setPosition(Vec2(WIDTH*.5,HEIGHT*.35));
    roomPassword->setFontSize(HEIGHT*.05);
    roomPassword->setFontColor(Color3B::WHITE);
    roomPassword->setMaxLength(15);
    const char *placeroompass;
    if(appManager->currentLanguage() == false){
        placeroompass = "Oda Şifresi";
    }
    else {
        placeroompass = "Room Password";
    }
    roomPassword->setPlaceHolder(placeroompass);
    roomPassword->setPlaceholderFontSize(HEIGHT*.05);
    roomPassword->setPlaceholderFontColor(Color3B::WHITE);
    
    roomPassword->setReturnType(EditBox::KeyboardReturnType::DONE);
    roomPassword->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    roomPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    roomPassword->setDelegate(this);
    this->addChild(roomPassword);
    
    
    this->addChild(roomName);
    
    
    //arkadas tamam button
    
    arkadasTamam = Sprite::create("tamamArkadaslaOyna.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(arkadasTamam, HEIGHT*.15);
            
            arkadasTamam->setPosition(Vec2(WIDTH*.5,HEIGHT*.17));
            
        }
        else {
            
            FIT_H(arkadasTamam, HEIGHT*.16);
            arkadasTamam->setPosition(Vec2(WIDTH*.5,HEIGHT*.17));
            
        }
        
        
    }
    else {
    
        FIT_H(arkadasTamam, HEIGHT*.16);
        arkadasTamam->setPosition(Vec2(WIDTH*.5,HEIGHT*.17));
   
    }
    this->addChild(arkadasTamam);
    
    if(appManager->currentLanguage() == false){
        arkadasTamamLabel = Label::createWithTTF("Tamam", "chinese rocks rg.ttf", 45);
    }
    else{
        arkadasTamamLabel = Label::createWithTTF("Ok", "chinese rocks rg.ttf", 45);
    }
    arkadasTamam->addChild(arkadasTamamLabel);
    arkadasTamamLabel->setPosition(Vec2(arkadasTamam->getContentSize().width/2, arkadasTamam->getContentSize().height/2));

    
}

void MenuScene::editBoxEditingDidBegin(EditBox* editBox){
    
}
void MenuScene::editBoxEditingDidEnd(EditBox* editBox){
    
}
void MenuScene::editBoxTextChanged(EditBox* editBox, string &text){
    
}
void MenuScene::editBoxReturn(EditBox * editBox){
    
}


void MenuScene::createTopViews(){
    
    
    //sol taraf
    solUstBar = Sprite::create("SolUst.png");
    FIT_H(solUstBar, HEIGHT*.12);
    solUstBar->setAnchorPoint(Vec2(0,.5));
    solUstBar->setPosition(Vec2(0,HEIGHT*.91));
    this->addChild(solUstBar);
    
    Avatar = Sprite::create("Avatar.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(Avatar, HEIGHT*.115);
               Avatar->setPosition(Vec2(WIDTH*.07,HEIGHT*.92));
            
            
        }
        else {
            
            FIT_H(Avatar, HEIGHT*.15);
            
            Avatar->setPosition(Vec2(WIDTH*.07,HEIGHT*.91));
            
        }
        
        
    }
    else {
    
        FIT_H(Avatar, HEIGHT*.15);
        
           Avatar->setPosition(Vec2(WIDTH*.07,HEIGHT*.91));
        
    }
 
        
    this->addChild(Avatar);
    
    userAvatar->setPosition(Avatar->getPosition());
    this->addChild(userAvatar);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
    
        //usernameLabel = Label::createWithTTF(StringUtils::format("LEVEL %d", int(appManager->level)), "chinese rocks rg.ttf", 30);
        
        usernameLabel = Label::createWithBMFont("menuFont.fnt", StringUtils::format("LEVEL %d", int(appManager->level)));
        
        
    }
    else {
        
        usernameLabel = Label::createWithBMFont("menuFont.fnt", StringUtils::format("LEVEL %d", int(appManager->level)));
        
    }
    
    FIT_H(usernameLabel, HEIGHT*.05);
    
    usernameLabel->setColor(Color3B::WHITE);
    usernameLabel->setPosition(Vec2(WIDTH*.165,HEIGHT*.91));
    usernameLabel->enableGlow(Color4B::BLACK);
    this->addChild(usernameLabel);
    
    Sprite *bosLevelBar = Sprite::create("Bos.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_W(bosLevelBar, WIDTH*.15);
            
           bosLevelBar->setPosition(Vec2(WIDTH*.29, HEIGHT*.92));
            
        }
        else {
            
            FIT_W(bosLevelBar, WIDTH*.15);
            
            bosLevelBar->setPosition(Vec2(WIDTH*.29, HEIGHT*.92));
            
        }
        
    }
    else {
        
        FIT_W(bosLevelBar, WIDTH*.15);
        
        bosLevelBar->setPosition(Vec2(WIDTH*.29, HEIGHT*.92));
        
    }
    
    this->addChild(bosLevelBar);
    
    
    levelBar = LoadingBar::create("Dolu.png");
    
    float percent;
    
    if(appManager->level < 2){
        
        percent = (appManager->xp/200)*100;
        
        levelBar->setPercent(percent);
        
    }
    else {
        
        percent = (appManager->xp/(200*appManager->level + (200*appManager->level*.2)))*100;
        
        levelBar->setPercent(percent);
        
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
        
            FIT_W(levelBar, WIDTH*.15);
            
            levelBar->setPosition(Vec2(WIDTH*.29, HEIGHT*.92));
            
        }
        else {
            
            FIT_W(levelBar, WIDTH*.15);
            
            levelBar->setPosition(Vec2(WIDTH*.29, HEIGHT*.92));
            
        }
        
    }
    else {
        
        FIT_W(levelBar, WIDTH*.15);
        
        levelBar->setPosition(Vec2(WIDTH*.29, HEIGHT*.92));
        
    }
    
    this->addChild(levelBar);
    
    
    
    
    Ayarlar = Sprite::create("Ayarlar.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(Ayarlar, HEIGHT*.098);
            Ayarlar->setPosition(Vec2(WIDTH*.5,HEIGHT*.92));
            
            
        }
        else {
            
            FIT_H(Ayarlar, HEIGHT*.1);
            Ayarlar->setPosition(Vec2(WIDTH*.438,HEIGHT*.92));
            
        }
        
        
    }
    else {
    
        FIT_H(Ayarlar, HEIGHT*.1);
        Ayarlar->setPosition(Vec2(WIDTH*.438,HEIGHT*.92));
        
    }
    this->addChild(Ayarlar);
    
    Bildirim = Sprite::create("Bildirim.png");
    FIT_H(Bildirim, HEIGHT*.1);
    Bildirim->setPosition(Vec2(WIDTH*.48, HEIGHT*.92));
    //this->addChild(Bildirim);
    
    //sağ taraf
    sagUst = Sprite::create("SagUst.png");
    FIT_WH(sagUst, WIDTH*.14, HEIGHT*.113);
    sagUst->setAnchorPoint(Vec2(1,.5));
    sagUst->setPosition(Vec2(WIDTH,HEIGHT*.917));
    this->addChild(sagUst);
    
    arti = Sprite::create("ArtiButon.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(arti, HEIGHT*.098);
            arti->setPosition(Vec2(WIDTH*.825,HEIGHT*.92));
            
            
        }
        else {
            
            FIT_H(arti, HEIGHT*.1);
            arti->setPosition(Vec2(WIDTH*.825,HEIGHT*.92));
            
        }
        
        
    }
    else {
    
        FIT_H(arti, HEIGHT*.1);
        arti->setPosition(Vec2(WIDTH*.825,HEIGHT*.92));
        
    }
    this->addChild(arti);
    
    altin = Sprite::create("Altin.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(altin, HEIGHT*.065);
            
            
            altin->setPosition(Vec2(WIDTH*.88,HEIGHT*.92));
            
        }
        else {
            
            FIT_H(altin, HEIGHT*.075);
            
            
            altin->setPosition(Vec2(WIDTH*.9,HEIGHT*.92));
            
        }
    }
    else {
        
        FIT_H(altin, HEIGHT*.075);
        
        
        altin->setPosition(Vec2(WIDTH*.9,HEIGHT*.92));
        
    }
   
    this->addChild(altin);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        //goldLabel = Label::createWithTTF(StringUtils::format("%.0f",appManager->gold), "chinese rocks rg.ttf", 38);
        
        goldLabel = Label::createWithBMFont("menuFont.fnt", StringUtils::format("%.0f", appManager->gold));
        
    }
    else {
        
        goldLabel = Label::createWithBMFont("menuFont.fnt", StringUtils::format("%.0f", appManager->gold));

        
    }
    
    FIT_H(goldLabel, HEIGHT*.05);
    
    goldLabel->setColor(Color3B::WHITE);
    goldLabel->enableGlow(Color4B::BLACK);
    goldLabel->setPosition(Vec2(WIDTH*.95,HEIGHT*.915));
    this->addChild(goldLabel);
    
    /*Krampon = Sprite::create("KramponMenu.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(Krampon, HEIGHT*.065);
            Krampon->setPosition(Vec2(WIDTH*.73, HEIGHT*.92));
            
        }
        else {
            
            FIT_H(Krampon, HEIGHT*.075);
            Krampon->setPosition(Vec2(WIDTH*.75, HEIGHT*.92));
            
        }
        
    }
    else {
        
        FIT_H(Krampon, HEIGHT*.075);
        Krampon->setPosition(Vec2(WIDTH*.75, HEIGHT*.92));
        
    }
    
    this->addChild(Krampon);
    
    levelLabel = Label::createWithTTF(StringUtils::format("%.0f",appManager->xp), "chinese rocks rg.ttf", 45);
    levelLabel->setColor(Color3B::WHITE);
    levelLabel->enableGlow(Color4B::BLACK);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
        
            levelLabel->setPosition(Vec2(WIDTH*.795, HEIGHT*.92));
            
        }
        else {
            
            levelLabel->setPosition(Vec2(WIDTH*.81,HEIGHT*.92));
            
        }
        
    }
    else {
    
        levelLabel->setPosition(Vec2(WIDTH*.81,HEIGHT*.92));
        
    }
    this->addChild(levelLabel);*/
    
}

void MenuScene::spendGolds(double goldCount){
    
    
    Sprite *gold = Sprite::create("Altin.png");
    FIT_H(gold, HEIGHT*.075);
    gold->setPosition(Vec2(WIDTH*.88, HEIGHT*.92));
    this->addChild(gold);
    
    auto moveAnim = MoveTo::create(.7, Vec2(WIDTH*.88,HEIGHT*.3));
    auto fadeAnim = FadeOut::create(1);
    auto scaleAnim = ScaleBy::create(.7, 2);
    auto spawnAnim = Spawn::create(moveAnim,fadeAnim,scaleAnim, NULL);
    
    
    gold->runAction(spawnAnim);
    
    
}

void MenuScene::changePassword(const char *username, const char *old_password, const char *new_password){
    
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    
    UserService *userService = App42API::BuildUserService();
    
    userService->ChangeUserPassword(username, old_password, new_password, app42callback(MenuScene::onChangePasswordRequestComplete, this));
    
}

void MenuScene::updateUserInformation(const char *username, double gold, double xp, double level, double avatar, double games, double won, double lost, double point){
    
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
    
    storageService->UpdateDocumentByKeyValue(db_name, collection_name, key, value, object, app42callback(MenuScene::onStorageRequestCompleted, this));
    
}

void MenuScene::updateUserForAvatar(const char *username, double gold, double xp, double level, double avatar, double games, double won, double lost, double point){
    
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
    
    storageService->UpdateDocumentByKeyValue(db_name, collection_name, key, value, object, app42callback(MenuScene::onAvatarStorageRequestComplete, this));

    
}

void MenuScene::onAvatarStorageRequestComplete(void *response){
    
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

void MenuScene::onChangePasswordRequestComplete(void *response){
    
    this->removeChangePasswordBefore();
    
    App42UserResponse *userResponse = (App42UserResponse*)response;
    printf("\ncode=%d...=%d",userResponse->getCode(),userResponse->isSuccess);
    printf("\nResponse Body=%s",userResponse->getBody().c_str());
    if (userResponse->isSuccess)
    {
        for(std::vector<App42User>::iterator it = userResponse->users.begin(); it != userResponse->users.end(); ++it)
        {
            printf("\n User name is %s",it->userName.c_str());
        }
        
        UserDefault *userDefaults = UserDefault::getInstance();
    
            
        appManager->password = temp_password.c_str();
            
        userDefaults->setStringForKey("username", appManager->username);
            
        userDefaults->setStringForKey("password", appManager->password);
            
        userDefaults->flush();
        
        if(appManager->currentLanguage() == false){
    
            this->passwordErrorPanel("Şifre Değişikliği Başarılı");
            
        }
        else {
            
            this->passwordErrorPanel("Password is changed successfully");
            
        }
        
    }
    else
    {
        printf("\nerrordetails:%s",userResponse->errorDetails.c_str());
        printf("\nerrorMessage:%s",userResponse->errorMessage.c_str());
        printf("\nappErrorCode:%d",userResponse->appErrorCode);
        printf("\nhttpErrorCode:%d",userResponse->httpErrorCode);
        
        
        
        std::string err = userResponse->errorDetails.c_str();
        
        if(appManager->currentLanguage() == false){
        
        if(err == StringUtils::format("User by the name '%s' does not exist.", appManager->username.c_str())){
            
            err = StringUtils::format("Kullanıcı bulunamadı");
            
        }
        else if(err == StringUtils::format("UserName/Password did not match. Authentication Failed.")){
            
            err = StringUtils::format("Kullanıcı adı veya şifre yanlış");
            
        }
        else if(err == StringUtils::format("Old Password is not matching for user '%s'.", appManager->username.c_str())){
            
            err = StringUtils::format("Eski şifre kullanıcı adıyla eşleşmiyor.");
            
        }
        else if(err == StringUtils::format("Users do not exist.")){
            
            err = StringUtils::format("Böyle bir kullanıcı yok");
            
        }
        else if(err == StringUtils::format("Internal Server Error. Please try again")){
            
            err = StringUtils::format("Genel sunucu hatası. Lütfen tekrar deneyin");
            
        }
        else if(err == StringUtils::format("The requested parameters are invalid")){
            
            err = StringUtils::format("Girilen değerler hatalı");
            
        }
        }
        
        this->passwordErrorPanel(err);
        
        
        
    }
    
}


void MenuScene::onStorageRequestCompleted(void *response){
    
    App42StorageResponse *storageResponse = (App42StorageResponse *)response;
    
    printf("\n response body = %s", storageResponse->getBody().c_str());
    
    
    if(storageResponse->isSuccess){
        
        for(std::vector<App42Storage>::iterator it = storageResponse->storages.begin(); it != storageResponse->storages.end(); ++it){
            
            for(std::vector<JSONDocument>::iterator iit = it->jsonDocArray.begin(); iit != it->jsonDocArray.end(); ++iit){
                
                
                printf("\n jsonDoc = %s", iit->getJsonDoc().c_str());
                
                
                updateUserProfile(iit->getJsonDoc().c_str());
                
                auto scene = GameScene::createScene();
                
                Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));

                
            }
            
            
        }
        
    }
    else {
        
        printf("\n errordetails = %s", storageResponse->errorDetails.c_str());
        printf("\n errormessage = %s", storageResponse->errorMessage.c_str());
        
    }

    
    
}

void MenuScene::updateUserProfile(const char *jsonString){
    
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
            
            this->updateUserForAvatar(appManager->username.c_str(), appManager->gold, appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
            
        }
        
        
    }
    else {
    
        if(appManager->xp >= 200*appManager->level + (200*appManager->level*.2)){
        
            appManager->level ++;
            
            this->updateUserForAvatar(appManager->username.c_str(), appManager->gold, appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
        
        }

    }
    
}



void MenuScene::createMainMenu(){
    
    if(backButton){
        
        this->removeChild(backButton);
        
    }
    
    oyna = Sprite::create("oyna.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(oyna, HEIGHT*.16);
            
            
        }
        else {
            
            FIT_H(oyna, HEIGHT*.23);
            
        }
        
    }
    else {
    
        FIT_H(oyna, HEIGHT*.23);
        
    }
    oyna->setPosition(Vec2(-WIDTH*.4, HEIGHT*.65));
    this->addChild(oyna);
    
    Label *oynaLabel;
    Label *mgzLabel;
    
    if(appManager->currentLanguage() == false){
    
        oynaLabel = Label::createWithTTF("Oyna", "chinese rocks rg.ttf", 80);
        mgzLabel = Label::createWithTTF("Mağaza", "chinese rocks rg.ttf", 80);

        
    }
    else {
        
        oynaLabel = Label::createWithTTF("Play", "chinese rocks rg.ttf", 80);
         mgzLabel = Label::createWithTTF("Store", "chinese rocks rg.ttf", 80);
        
    }
    
    oynaLabel->setPosition(Vec2(oyna->getContentSize().width/1.8, oyna->getContentSize().height/2));
    oyna->addChild(oynaLabel);
    
    magaza = Sprite::create("mgz.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(magaza, HEIGHT*.16);
            
            
        }
        else {
            
            FIT_H(magaza, HEIGHT*.23);
            
        }
        
    }
    else {
    
        FIT_H(magaza, HEIGHT*.23);
        
    }
    magaza->setPosition(Vec2(-WIDTH*.2, HEIGHT*.65));
    this->addChild(magaza);
    
    mgzLabel->setPosition(Vec2(oyna->getContentSize().width/1.8, oyna->getContentSize().height/2));
    magaza->addChild(mgzLabel);
    
    magazine = Sprite::create("cikartmaKitap.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(magazine, HEIGHT*.16);
            
            
        }
        else {
            
            FIT_H(magazine, HEIGHT*.23);
            
        }
        
    }
    else {
    
        FIT_H(magazine, HEIGHT*.23);
        
    }
    magazine->setPosition(Vec2(WIDTH*1.2, HEIGHT*.65));
    this->addChild(magazine);
    
    Label *magLabel = Label::createWithTTF("Çıkartmalar", "chinese rocks rg.ttf", 60);
    magLabel->setPosition(Vec2(magazine->getContentSize().width/1.8, magazine->getContentSize().height/2));
    magazine->addChild(magLabel);
    
    magazine->setVisible(false);
    
    auto anim = MoveTo::create(1, Vec2(WIDTH*.35,HEIGHT*.65));
    auto anim2 = MoveTo::create(1, Vec2(WIDTH*.65,HEIGHT*.65));
    //auto anim3 = MoveTo::create(1, Vec2(WIDTH*.8,HEIGHT*.65));
    
    auto ease1 = EaseBackInOut::create(anim);
    auto ease2 = EaseBackInOut::create(anim2);
    //auto ease3 = EaseBackInOut::create(anim3);
    
    oyna->runAction(ease1);
    magaza->runAction(ease2);
    //magazine->runAction(ease3);
    
}

void MenuScene::createOlumMaciMenu() {
    
    //single button
    
    actOfScene = 5;
    
    background->setTexture("deathmacbgyeni.png");
    
    olumMaciSingleButton = Sprite::create("olummacitekmac.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(olumMaciSingleButton, HEIGHT*.18);
            
            
        }
        else {
            
            FIT_H(olumMaciSingleButton, HEIGHT*.23);
            
        }
        
    }
    else {
    
        FIT_H(olumMaciSingleButton, HEIGHT*.23);
        
    }
    
    olumMaciSingleButton->setPosition(Vec2(-WIDTH*.2,HEIGHT*.65));
    
    this->addChild(olumMaciSingleButton);
    
    //label
    
    Label *singleOlumMaciLabel;
    
    if(appManager->currentLanguage() == false){
     
        singleOlumMaciLabel = Label::createWithTTF("TEK KİŞİ", "chinese rocks rg.ttf", 65);

    }
    else {
        
        singleOlumMaciLabel = Label::createWithTTF("RANDOM MATCH", "chinese rocks rg.ttf", 55);
        
    }
    
       singleOlumMaciLabel->setPosition(Vec2(olumMaciSingleButton->getContentSize().width/1.8, olumMaciSingleButton->getContentSize().height/2));
    olumMaciSingleButton->addChild(singleOlumMaciLabel);
    
    //multi button
    
    olumMaciMultiButton = Sprite::create("olummultibuton.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(olumMaciMultiButton, HEIGHT*.18);
            
            
        }
        else {
            
            FIT_H(olumMaciMultiButton, HEIGHT*.23);
            
        }
        
        
    }
    else {
        
        FIT_H(olumMaciMultiButton, HEIGHT*.23);
        
    }
    
    
    olumMaciMultiButton->setPosition(Vec2(-WIDTH*.2,HEIGHT*.65));
    
    olumMaciMultiButton->setOpacity(130);
    
    this->addChild(olumMaciMultiButton);
    
    //multi label
    
    Label *multiOlumMaciLabel = Label::createWithTTF("ONLINE MOD", "chinese rocks rg.ttf", 63);
    multiOlumMaciLabel->setPosition(Vec2(olumMaciMultiButton->getContentSize().width/1.8, olumMaciMultiButton->getContentSize().height/2));
    olumMaciMultiButton->addChild(multiOlumMaciLabel);
    
    auto anim1 = MoveTo::create(.5, Vec2(WIDTH*.3,HEIGHT*.65));
    auto anim2 = MoveTo::create(.5, Vec2(WIDTH*.7,HEIGHT*.65));
    
    auto ease1 = EaseBackInOut::create(anim1);
    auto ease2 = EaseBackInOut::create(anim2);
    
    olumMaciSingleButton->runAction(ease1);
    olumMaciMultiButton->runAction(ease2);
    
}

void MenuScene::playSuccessOlumMaci(){
    
    if(appManager->fx == true){
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
        
    }
    
    olumMaciMultiButton->setOpacity(255);
    
    //altin düş servise gönder animasyonu başlat
    
    if(appManager->gold >= 100){
        
        auto scheduler = Director::getInstance()->getScheduler();
        
        scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MenuScene::spendGolds,this,(double)10));
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold - 100)));
        
        updateUserInformation(appManager->username.c_str(), (appManager->gold - 100), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
        
    }
    else {
        
        //UYARI
        
        if(appManager->currentLanguage() == false){
        
            this->noItemWarning("ALTIN YOK", "Ölüm Maçı için yeterli altınınız yok");
            
        }
        else {
            
            this->noItemWarning("NOT ENOUGH GOLD", "There is not enough gold for deathmatch");
            
        }
        
    }
    
}

void MenuScene::removeAltinPanelForOlumMaci(){
    
    this->removeChild(olumMaciAltinPanel);
    
}

void MenuScene::startOlumMaci() {
    
    olumMaciAltinPanel = Node::create();
    
    olumMaciAltinPanel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(olumMaciAltinPanel);
    
    Sprite *panel = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(panel, HEIGHT*.85);
            
        }
        else {
            
            FIT_H(panel, HEIGHT*.95);
            
        }
        
    }
    else {
        
        FIT_H(panel, HEIGHT*.95);
        
    }
    
    panel->setPosition(Vec2(0,0));
    
    olumMaciAltinPanel->addChild(panel);
    
    //label
    
    Label *altinLabel;
    
    if(appManager->currentLanguage() == false){
    
        altinLabel = Label::createWithTTF("BU MODU OYNAMAK İÇİN\n100 ALTIN GEREKLİDİR", "chinese rocks rg.ttf", HEIGHT*.06);
        
    }
    else {
        
        altinLabel = Label::createWithTTF("YOU NEED 100 GOLDS \nTO PLAY THIS MODE", "chinese rocks rg.ttf", HEIGHT*.06);
        
    }
    
    altinLabel->setPosition(Vec2(0,HEIGHT*.1));
    
    altinLabel->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    
    olumMaciAltinPanel->addChild(altinLabel);
    
    //button
    
    if(appManager->gold >= 100){
        
        auto tamamButton = ui::Button::create("tamamyeni.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad()){
                
                FIT_H(tamamButton, HEIGHT*.2);
                
                tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
                
            }
            else {
                
                FIT_H(tamamButton, HEIGHT*.21);
                
                tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
                
            }
            
        }
        else {
            
            FIT_H(tamamButton, HEIGHT*.21);
            
            tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
            
        }
        
        olumMaciAltinPanel->addChild(tamamButton);
        
        Label *tamamLabel;
        
        if(appManager->currentLanguage() == false){
            
            tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 50);
            
        }
        else {
            
            tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
            
        }
        
        tamamLabel->setPosition(Vec2(tamamButton->getContentSize().width/2, tamamButton->getContentSize().height/2));
        
        tamamButton->addChild(tamamLabel);
        
        tamamButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
            
            switch (type) {
                
                case ui::Widget::TouchEventType::BEGAN:
                    
                    break;
                    
                case ui::Widget::TouchEventType::CANCELED:
                    
                    break;
                    
                case ui::Widget::TouchEventType::ENDED:
                    
                    this->removeAltinPanelForOlumMaci();
                    
                    this->playSuccessOlumMaci();
                    
                    break;
                    
                case ui::Widget::TouchEventType::MOVED:
                    
                    break;

                    
            }
            
            
        });
        
        //geri button
        
        auto geriButton = ui::Button::create("backbutonyeni.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad()){
                
                FIT_H(geriButton, HEIGHT*.14);
                
                geriButton->setPosition(Vec2(-WIDTH*.27, -HEIGHT*.27));
                
            }
            else {
                
                FIT_H(geriButton, HEIGHT*.13);
                
                geriButton->setPosition(Vec2(-WIDTH*.17, -HEIGHT*.27));
                
            }
            
        }
        else {
            
            FIT_H(geriButton, HEIGHT*.13);
            
            geriButton->setPosition(Vec2(-WIDTH*.17, -HEIGHT*.27));
            
        }
        
        olumMaciAltinPanel->addChild(geriButton);
        
        geriButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
            
            switch (type) {
                    
                case ui::Widget::TouchEventType::BEGAN:
                    
                    break;
                    
                case ui::Widget::TouchEventType::CANCELED:
                    
                    break;
                    
                case ui::Widget::TouchEventType::ENDED:
                    
                    this->removeAltinPanelForOlumMaci();
                    
                    break;
                    
                case ui::Widget::TouchEventType::MOVED:
                    
                    break;
                    
                    
            }
            
            
            
        });


        
    }
    else {
        
        auto tamamButton = ui::Button::create("tamamyeni.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad()){
                
                FIT_H(tamamButton, HEIGHT*.2);
                
                tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
                
            }
            else {
                
                FIT_H(tamamButton, HEIGHT*.21);
                
                tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
                
            }
            
        }
        else {
            
            FIT_H(tamamButton, HEIGHT*.21);
            
            tamamButton->setPosition(Vec2(0, -HEIGHT*.27));
            
        }
        
        olumMaciAltinPanel->addChild(tamamButton);
        
        Label *tamamLabel;
        
        if(appManager->currentLanguage() == false){
            
            tamamLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 50);
            
        }
        else {
            
            tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
            
        }
        
        tamamLabel->setPosition(Vec2(tamamButton->getContentSize().width/2, tamamButton->getContentSize().height/2));
        
        tamamButton->addChild(tamamLabel);
        
        tamamButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
            
            switch (type) {
                    
                case ui::Widget::TouchEventType::BEGAN:
                    
                    break;
                    
                case ui::Widget::TouchEventType::CANCELED:
                    
                    break;
                    
                case ui::Widget::TouchEventType::ENDED:
                    
                    this->removeAltinPanelForOlumMaci();
                    
                    break;
                    
                case ui::Widget::TouchEventType::MOVED:
                    
                    break;
                    
                    
            }
            
            
        });

        
        
    }
    
    
}

void MenuScene::createOynaMenu(){
    
    backButton = Sprite::create("backbutonyeni.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
     
        if(appManager->isIpad() == true){
            
            
            FIT_H(backButton, HEIGHT*.1);
            
            
        }
        else {
            
            FIT_H(backButton, HEIGHT*.13);
            
        }
        
        
    }
    else {
    
        FIT_H(backButton, HEIGHT*.13);
        
    }
    
    backButton->setPosition(Vec2(WIDTH*.07, HEIGHT*.08));
    addChild(backButton);
    
    olumMaci = Sprite::create("dthmac.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(olumMaci, HEIGHT*.16);
            
            
        }
        else {
            
            FIT_H(olumMaci, HEIGHT*.23);
            
        }
        
    }
    else {
    
        FIT_H(olumMaci, HEIGHT*.23);
        
    }
    
    
    olumMaci->setPosition(Vec2(-WIDTH*.2, HEIGHT*.65));
    this->addChild(olumMaci);
    
    
    Label *olumLabel, *turLabel,*tekLabel,*arkadasinlaOynaLabel;
    
    if(appManager->currentLanguage() == false){
        
        olumLabel = Label::createWithTTF("Ölüm Maçı", "chinese rocks rg.ttf", 60);
        turLabel = Label::createWithTTF("Turnuva Modu", "chinese rocks rg.ttf", 60);
        tekLabel = Label::createWithTTF("Tek Maç", "chinese rocks rg.ttf", 60);
        arkadasinlaOynaLabel = Label::createWithTTF("Arkadaşla Oyna", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        olumLabel = Label::createWithTTF("Deathmatch", "chinese rocks rg.ttf", 60);
        turLabel = Label::createWithTTF("Tournament Mode", "chinese rocks rg.ttf", 58);
        tekLabel = Label::createWithTTF("Random Match", "chinese rocks rg.ttf", 60);
        arkadasinlaOynaLabel = Label::createWithTTF("Play With Friend", "chinese rocks rg.ttf", 50);
        
    }
    
    olumLabel->setPosition(Vec2(olumMaci->getContentSize().width/1.8, olumMaci->getContentSize().height/2));
    olumMaci->addChild(olumLabel);
    
    turnuva = Sprite::create("turmac.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(turnuva, HEIGHT*.16);
            
            
        }
        else {
            
            FIT_H(turnuva, HEIGHT*.23);
            
        }
        
        
    }
    else {
    
        FIT_H(turnuva, HEIGHT*.23);
        
    }
    turnuva->setPosition(Vec2(-WIDTH*.2, HEIGHT*.65));
    this->addChild(turnuva);
    
    turLabel->setPosition(Vec2(turnuva->getContentSize().width/1.7, turnuva->getContentSize().height/2));
    turnuva->addChild(turLabel);
    
    tekKisi = Sprite::create("tekmac.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(tekKisi, HEIGHT*.16);
            
            
        }
        else {
            
            FIT_H(tekKisi, HEIGHT*.23);
            
        }
        
    }
    else {
    
        FIT_H(tekKisi, HEIGHT*.23);
        
    }
    
    tekKisi->setPosition(Vec2(-WIDTH*.2, HEIGHT*.4));
    this->addChild(tekKisi);
    
    tekLabel->setPosition(Vec2(tekKisi->getContentSize().width/1.8, tekKisi->getContentSize().height/2));
    tekKisi->addChild(tekLabel);
    
    
    arkadasinlaOyna = Sprite::create("arkadaslaoynayeni.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(arkadasinlaOyna, HEIGHT*.16);
            
        }
        else {
            
            FIT_H(arkadasinlaOyna, HEIGHT*.23);
            
        }
        
    }
    else {
        
        FIT_H(arkadasinlaOyna, HEIGHT*.23);
        
    }
    
    arkadasinlaOyna->setPosition(Vec2(-WIDTH*.2, HEIGHT*.4));
    this->addChild(arkadasinlaOyna);
    
    
    arkadasinlaOynaLabel->setPosition(Vec2(arkadasinlaOyna->getContentSize().width/1.68, arkadasinlaOyna->getContentSize().height/2));
    arkadasinlaOyna->addChild(arkadasinlaOynaLabel);
    
    
    auto anim = MoveTo::create(1, Vec2(WIDTH*.35,HEIGHT*.65));
    auto anim2 = MoveTo::create(1, Vec2(WIDTH*.65,HEIGHT*.65));
    auto anim3 = MoveTo::create(1, Vec2(WIDTH*.35,HEIGHT*.4));
    auto anim4 = MoveTo::create(1, Vec2(WIDTH*.65,HEIGHT*.4));
    
    auto ease1 = EaseBackInOut::create(anim);
    auto ease2 = EaseBackInOut::create(anim2);
    auto ease3 = EaseBackInOut::create(anim3);
    auto ease4 = EaseBackInOut::create(anim4);

    olumMaci->runAction(ease1);
    turnuva->runAction(ease2);
    tekKisi->runAction(ease3);
    arkadasinlaOyna->runAction(ease4);
    
}

void MenuScene::createArkadaslaOynaMenu(){
    
    //oda kur oda bul
    actOfScene = 2; // oda bul oda kur
    
    odaKur = Sprite::create("Oda Kur Buton.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(odaKur, HEIGHT*.27);
            
            
        }
        else {
            
            FIT_H(odaKur, HEIGHT*.27);
            
        }
        
    }
    else {
    
        FIT_H(odaKur, HEIGHT*.27);
        
    }
    
    
    
    odaKur->setPosition(Vec2(-WIDTH*.5,HEIGHT*.6));

    this->addChild(odaKur);
    
    Label *odakurlabel, *odabullabel;
    
    if(appManager->currentLanguage() == false){
        
        odakurlabel = Label::createWithTTF("Oda Kur", "chinese rocks rg.ttf", 55);
        odabullabel = Label::createWithTTF("Oda Bul", "chinese rocks rg.ttf", 55);

        
    }
    else {
        
        odakurlabel = Label::createWithTTF("Create Room", "chinese rocks rg.ttf", 55);
        odabullabel = Label::createWithTTF("Find Room", "chinese rocks rg.ttf", 55);

        
    }
    
    odakurlabel->setPosition(Vec2(odaKur->getContentSize().width/2,odaKur->getContentSize().height/2));
    
    odaKur->addChild(odakurlabel);
    
    auto anim = MoveTo::create(.5, Vec2(WIDTH*.3,HEIGHT*.6));
    
    auto ease = EaseCubicActionOut::create(anim);
    
    odaKur->runAction(ease);
    
    //
    
    odaBul = Sprite::create("Oda Bul Buton.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(odaBul, HEIGHT*.27);
            
        }
        else {
            
            FIT_H(odaBul, HEIGHT*.27);
            
        }
        
    }
    else {
        
        FIT_H(odaBul, HEIGHT*.27);
    
    }
    
    odaBul->setPosition(Vec2(-WIDTH*.5,HEIGHT*.6));
    
    this->addChild(odaBul);
    
    odabullabel->setPosition(Vec2(odaBul->getContentSize().width/2,odaBul->getContentSize().height/2));
    
    odaBul->addChild(odabullabel);

    auto anim2 = MoveTo::create(.5, Vec2(WIDTH*.7,HEIGHT*.6));
    
    auto ease2 = EaseCubicActionOut::create(anim2);
    
    odaBul->runAction(ease2);
  
    
}

void MenuScene::gotoMagaza(){
    
    auto scene = MagazaScene::createScene();
    Director::getInstance()->replaceScene(TransitionFlipAngular::create(.5, scene, cocos2d::TransitionScene::Orientation::RIGHT_OVER));
    
}

void MenuScene::removeMainMenu(){
    
    this->removeChild(oyna);
    this->removeChild(magaza);
    this->removeChild(magazine);
    
}

void MenuScene::removeArkadasMenu(){
    
    this->removeChild(odaBul);
    this->removeChild(odaKur);
    actOfScene = 1;
    
    
}

void MenuScene::removeOlumMaciMenu(){
    
    background->setTexture("Background.png");
    
    this->removeChild(olumMaciMultiButton);
    
    this->removeChild(olumMaciSingleButton);
    
}

void MenuScene::removeOynaMenu(){
    
    this->removeChild(olumMaci);
    this->removeChild(turnuva);
    this->removeChild(tekKisi);
    this->removeChild(arkadasinlaOyna);

    
}

