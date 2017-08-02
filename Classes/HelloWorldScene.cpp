#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    WIDTH = GET_WIDTH;
    HEIGHT = GET_HEIGHT;
    
    
    
    appManager = AppManager::getInstance();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
    
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("victory.mp3");
        
    }
    else if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("victory.ogg");
        
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    /*auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    */
    

    // create menu, it's an autorelease object
    /*auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    */
    
    //this->playIntro();
    
    
    //get server time and save 
    
    UserDefault *userDefaults = UserDefault::getInstance();
    
    
    if(userDefaults->getBoolForKey("autoLogin") == false){
    
        autoLogining = false;
        
        this->createMainScene();
        
    }
    else if(userDefaults->getBoolForKey("autoLogin") == true) {
        
        this->autoLogin();
        
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
    
        this->setKeypadEnabled(true);
        
    }
    
    return true;
}

void HelloWorld::playIntro() {
    
    
}



void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event){
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        
        /*UserDefault *userDefaults = UserDefault::getInstance();
        
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
        */
        
        this->callQuitKeyAction();
        
    }
    
}


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    void HelloWorld::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
        switch (eventType) {
            case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
                break;
            case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
                break;
            case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
                break;
            case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
                videoPlayOverCallback();
                break;
            default:
                break;
        }
    }

#endif

void HelloWorld::videoPlayOverCallback(){
    
    
    
}

void HelloWorld::callQuitKeyAction() {
    
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





void HelloWorld::connectToAppwarp() {
    
    if(appManager->currentLanguage() == false){
    
        connectingLabel->setString("SUNUCUYA BAĞLANIYOR");
        
    }
    else {
        
        connectingLabel->setString("CONNECTING TO SERVER");
        
    }
 
    tempScene = actOfScene;
    
    actOfScene = 3;
    
    AppWarp::Client *warpClientRef;
    
    AppWarp::Client::initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    
    //set username sharedinstance
    
    UserDefault *userDefaults = UserDefault::getInstance();
    
    int sessionID = userDefaults->getIntegerForKey("sessionID");
    
    if(sessionID != NULL){
    
        warpClientRef->recoverConnectionWithSessionID(sessionID, appManager->username);
        
    }
    else {
        
         warpClientRef->connect(appManager->username);
        
    }
   
    
}

UserService * HelloWorld::initApp42Service() {
    
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    
    UserService *userService = App42API::BuildUserService();
    
    return userService;
}

void HelloWorld::app42CreatePlayer(const char *username, const char *password, const char *email){
    
    const char *user_name = username;
    const char *pwd = password;
    const char *emailId = email;
    
    UserService *userService = initApp42Service();
    
    userService->CreateUser(user_name, pwd, emailId, app42callback(HelloWorld::onUserRequestCompleted, this));
    
    
}

void HelloWorld::insertUserInfoToService(const char *username, int gold, int level, int avatar, int xp, int games, int won, int lost, int point){
    
    const char *db_name = "Santra";
    const char *collection_name = "userInfo";
    
    App42Object *object = new App42Object();
    
    object->setObject("username", username);
    object->setObject("gold", gold);
    object->setObject("level", level);
    object->setObject("avatar", avatar);
    object->setObject("xp", xp);
    object->setObject("games", games);
    object->setObject("won", won);
    object->setObject("lost", lost);
    object->setObject("point", point);
    
    
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    
    StorageService *storageService = App42API::BuildStorageService();
    
    storageService->InsertJsonDocument(db_name, collection_name, object ,app42callback(HelloWorld::onStorageRequestCompleted, this));
}

void HelloWorld::updateUserProfile(const char *jsonString){
    
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
    
    
    
    //connect to appwarp
    
    connectToAppwarp();
    
    
}

void HelloWorld::getUserInfosWithUserName(const char *username){
    
    const char *db_name = "Santra";
    const char *collection_name = "userInfo";
    
    const char *key = "username";
    const char *value = username;
    
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);
    
    StorageService *storageService = App42API::BuildStorageService();

    storageService->FindDocumentByKeyValue(db_name, collection_name, key, value, app42callback(HelloWorld::onStorageRequestCompleted, this));
    
}


void HelloWorld::onStorageRequestCompleted(void *response){
    
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
        
        this->removeChild(loader);
        this->removeChild(loaderBack);
        this->removeChild(blackBg);
        this->removeChild(connectingLabel);

        
        
        openErrorMenu("Sunucuda bir Hata Oluştu");
        
    }
    
}


void HelloWorld::onUserRequestCompleted(void *response){
    
    App42UserResponse *userResponse = (App42UserResponse *)response;
    
    const char *user_name, *pass_word, *e_mail;
    
    if(userResponse->isSuccess){
        
        for(std::vector<App42User>::iterator it = userResponse->users.begin(); it != userResponse->users.end(); ++it){
            
            printf("\n username : %s", it->userName.c_str());
            printf("\n password : %s", it->password.c_str());
            
            user_name = it->userName.c_str();
            pass_word = appManager->password.c_str();
            e_mail = it->email.c_str();
            
            if(actOfScene == 2){
                
                if(appManager->authenticated == false){
                    
                    appManager->authenticated = true;
                    
                    auto scheduler = Director::getInstance()->getScheduler();
                    
                    scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::girisLoaderAnimation,this));

                    authenticateUser(user_name, pass_word);
                    
                    //insert users info json
                    
                    //insertUserInfoToService(user_name, 100, 1, 1, 0,0,0,0,0);
                    
                    
                }
                else {
                    
                    //change scene
                    
                    //get users info by username
                    
                    
                    //insert users info json
                    
                    insertUserInfoToService(user_name, 100, 1, 1, 0,0,0,0,0);
                    
                    //getUserInfosWithUserName(user_name);
                    
                }
                
                
            }
            else if(actOfScene == 1){
                
                //change scene bro
                
                //get users info by username
                
                getUserInfosWithUserName(user_name);
                
            }
         
            
        }
        
    }
    else {
        
        printf("\n errordetails : %s", userResponse->errorDetails.c_str());
        printf("\n errormessage : %s", userResponse->errorMessage.c_str());
        
        
        autoLogining = false;
       
        
        this->removeChild(loader);
        this->removeChild(loaderBack);
        this->removeChild(blackBg);
        this->removeChild(connectingLabel);
        
        
        
        
        if(actOfScene == 1){
            
            createGirisScene();
            
            name->setPosition(Vec2(WIDTH*2,HEIGHT*2));
            password->setPosition(Vec2(WIDTH*2,HEIGHT*2));
            
        }
        else if(actOfScene == 2){
            
            createUyeOlScene();
            
            name->setPosition(Vec2(WIDTH*2,HEIGHT*2));
            password->setPosition(Vec2(WIDTH*2,HEIGHT*2));
            email->setPosition(Vec2(WIDTH*2,HEIGHT*2));
            
        }
        
        std::string err = userResponse->errorDetails.c_str();
        
        if(appManager->currentLanguage() == false){

        if(err == StringUtils::format("User by the name '%s' does not exist.",kayitUserName.c_str())){
            
            err = StringUtils::format("Kullanıcı bulunamadı");
            
        }
        else if(err == StringUtils::format("The request parameters are invalid. Username '%s' already exists.",kayitUserName.c_str())){
            
            err = StringUtils::format("Kullanıcı adı daha önceden alınmış");
            
        }
        else if(err == StringUtils::format("UserName/Password did not match. Authentication Failed.")){
            
            err = StringUtils::format("Kullanıcı adı veya şifre yanlış");
            
        }
        else if(err == StringUtils::format("The request parameters are invalid. User with emailId '%s' already exists.",kayitEmail.c_str())){
            
            err = StringUtils::format("'E-Mail adresi daha önceden alınmış");
            
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
        
        
        openErrorMenu(err);
            
    }
    
    
}

void HelloWorld::editBoxEditingDidEnd(EditBox *editBox) {

    

}

void HelloWorld::girisLoaderAnimation(){
    
    blackBg = Sprite::create("blackBG.png");
    
    FIT_WH(blackBg, WIDTH, HEIGHT);
    blackBg->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(blackBg);
    blackBg->setOpacity(255);
    
    if(loaderBack != NULL){
        
        this->removeChild(loaderBack);
        
    }
    
    loaderBack = Sprite::create("intro1.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(loaderBack, WIDTH*.115);
            
        }
        else {
            
            FIT_H(loaderBack, WIDTH*.115);
            
        }
        
    }
    else {
        
        FIT_H(loaderBack, WIDTH*.115);
        
    }
    
    
    loaderBack->setPosition(Vec2(WIDTH*.9,HEIGHT*.13));
    this->addChild(loaderBack);
    
    
    if(loader){
        
        this->removeChild(loader);
        
    }
    
    loader = Sprite::create("intro2.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(loader, WIDTH*.17);
            
        }
        else {
            
            FIT_H(loader, WIDTH*.17);
            
        }
        
    }
    else {
        
        FIT_H(loader, WIDTH*.17);
        
    }
    loader->setPosition(Vec2(WIDTH*.9, HEIGHT*.13));
    this->addChild(loader);
    
    
    auto anim = RotateBy::create(1, 360);
    
    auto repeat = RepeatForever::create(anim);
    
    loader->runAction(repeat);
    
    auto anim2 = RotateBy::create(1, -360);
    
    auto repeat2 = RepeatForever::create(anim2);
    
    loaderBack->runAction(repeat2);
    
    if(connectingLabel){
        
        this->removeChild(connectingLabel);
        
    }
    
    if(appManager->currentLanguage() == false){
        
        connectingLabel = Label::createWithTTF("GİRİŞ YAPILIYOR", "chinese rocks rg.ttf", HEIGHT*.05);
        
    }
    else {
        
        connectingLabel = Label::createWithTTF("LOGGING IN", "chinese rocks rg.ttf", HEIGHT*.05);
        
    }
    

    connectingLabel->setTextColor(Color4B(255, 255, 255, 220));
    connectingLabel->setPosition(Vec2(WIDTH*.5,HEIGHT*.1));
    this->addChild(connectingLabel);
    
    auto fadeOutAnim = FadeOut::create(.4);
    
    auto fadeInAnim = FadeIn::create(.4);
    
    auto seq = Sequence::create(fadeOutAnim, fadeInAnim, NULL);
    
    auto repeatLabel = RepeatForever::create(seq);
    
    connectingLabel->runAction(repeatLabel);
    

    
}

void HelloWorld::authenticateUser(const char *username, const char *password){
    
    
    
    //_-----------------____----_____----____
    
    const char* user_name = username;
    const char* pass_word = password;
    
    UserService *userService = initApp42Service();
    
    userService->Authenticate(user_name, pass_word, app42callback(HelloWorld::onUserRequestCompleted, this));
    
}

void HelloWorld::onEnterTransitionDidFinish(){
    
    Layer::onEnterTransitionDidFinish();
    
    this->schedule(schedule_selector(HelloWorld::update));
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
}

void HelloWorld::openErrorMenu(std::string error){
    
    if(autoLogining == true){
        
        autoLogining = false;
        
    }
    
    UserDefault *userdefaults = UserDefault::getInstance();
    
    if(userdefaults->getIntegerForKey("sessionID")){
        
        userdefaults->setIntegerForKey("sessionID", NULL);
        
        userdefaults->flush();
        
    }
    
    //tamamButton->setOpacity(255);
    
    //tamamLabel->setOpacity(255);
    
    Sprite * ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0,0));
    
    this->addChild(errorPanel,100);
    
    errorPanel->addChild(ayarlarPerde);
    
    Sprite *panelBG = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(panelBG, HEIGHT*.7);
            
        }
        else {
            
            FIT_H(panelBG, HEIGHT*.85);
            
        }
        
    }
    else {
    
        FIT_H(panelBG, HEIGHT*.85);
        
    }
    
    panelBG->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel->addChild(panelBG);
    
    Label *errorLabel1 = Label::createWithTTF(error, "chinese rocks rg.ttf", HEIGHT*.045);
    
    errorLabel1->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));

    errorPanel->addChild(errorLabel1);
    
    auto okButton = ui::Button::create("tamamyeni.png");
    
    Label *okLabel;
    
    if(appManager->currentLanguage() == false){
    
        okLabel = Label::createWithTTF("TAMAM", "chinese rocks rg.ttf", 80);
        
    }
    else {
        
        okLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 80);
        
    }
    
    okLabel->setColor(Color3B::WHITE);
    
    okLabel->setPosition(Vec2(okButton->getContentSize().width/2, okButton->getContentSize().height/2));
    
    okButton->addChild(okLabel);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(okButton, WIDTH*.07);
            
            okButton->setPosition(Vec2(WIDTH*.5, HEIGHT*.3));
            
        }
        else {
            
            FIT_H(okButton, HEIGHT*.14);
            
            okButton->setPosition(Vec2(WIDTH*.5, HEIGHT*.25));
            
        }
        
    }
    else {
        
        FIT_H(okButton, HEIGHT*.14);
        
        okButton->setPosition(Vec2(WIDTH*.5, HEIGHT*.3));
        
    }

    errorPanel->addChild(okButton);
    
    okButton->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type){
        
        switch (type) {
            
            case ui::Widget::TouchEventType::BEGAN:
                
                //do-nothing
                
                break;
            
            case ui::Widget::TouchEventType::CANCELED:
                
                //do-nothing
                
                break;
                
            case ui::Widget::TouchEventType::ENDED:
                
                this->removeErrorPanel();
                
                break;
                
            case ui::Widget::TouchEventType::MOVED:
                
                //do-nothing
                
                break;
            
            
        }
        
        
    });
   
    //this->schedule(schedule_selector(HelloWorld::removeErrorPanel),3.0f);
    
}

void HelloWorld::removeErrorPanel(){
    
    if(actOfScene == 1){

        if(name){
        
            removeAllChildren();
            
            actOfScene = 0;
            
            createMainScene();
        
        }
        else {
            
            actOfScene = 0;
            
            createMainScene();
            
        }
        
    }
    else if(actOfScene == 2){
        
        name->setPosition(Vec2(WIDTH*.58,HEIGHT*.8));
        password->setPosition(Vec2(WIDTH*.6,HEIGHT*.6));
        email->setPosition(Vec2(WIDTH*.6,HEIGHT*.4));
        
    }
    else if(actOfScene == 5){
        
        removeAllChildren();
        
        this->resetUserPassword();
        
    }
    
    this->removeChild(errorPanel);
    
    //this->unschedule(schedule_selector(HelloWorld::removeErrorPanel));
    
}

void HelloWorld::onConnectDone(int res, int reasonCode){
    
    if(res == AppWarp::ResultCode::success){
        
        
        unscheduleRecover();
        printf("\nonConnectDone .. SUNUCUYA BAŞARIYLA BAĞLANDI..session=%d\n",AppWarp::AppWarpSessionID);
        
        if(appManager->currentLanguage() == false){
        
            connectingLabel->setString("SUNUCUYA BAŞARIYLA BAĞLANDI");
        
        }
        else {
            
            connectingLabel->setString("CONNECTED SUCCESSFULLY");
            
        }
        
        actOfScene = 5;
        
        //başarılı bağlantı
        
        AppManager::getInstance()->isConnected = true;
        
        UserDefault *userDefaults = UserDefault::getInstance();
        
        if(userDefaults->getBoolForKey("autoLogin") == false){
            
            userDefaults->setBoolForKey("autoLogin", true);
            
            userDefaults->setStringForKey("username", appManager->username);
            
            userDefaults->setStringForKey("password", appManager->password);
            
            userDefaults->flush();
            
        }
        
        if(!userDefaults->getIntegerForKey("sessionID")){
            
            AppWarp::Client *warpClient = AppWarp::Client::getInstance();
            
            userDefaults->setIntegerForKey("sessionID", warpClient->getSessionID());
            
            userDefaults->flush();
            
        }
        
        //scene
        
        auto scene = MenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionShrinkGrow::create(.6, scene));
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        autoLogining = false;
        
        unscheduleRecover();
        printf("\nonConnectDone .. SUCCESS with success_recovered..session=%d\n",AppWarp::AppWarpSessionID);
        
        AppManager::getInstance()->isConnected = true;
        
        UserDefault *userDefaults = UserDefault::getInstance();
        
        if(userDefaults->getBoolForKey("autoLogin") == false){
            
            userDefaults->setBoolForKey("autoLogin", true);
            
            userDefaults->setStringForKey("username", appManager->username);
            
            userDefaults->setStringForKey("password", appManager->password);
            
            userDefaults->flush();
            
        }
        
        if(userDefaults->getIntegerForKey("sessionID") != AppWarp::Client::getInstance()->getSessionID()){
            
            AppWarp::Client *warpClient = AppWarp::Client::getInstance();
            
            userDefaults->setIntegerForKey("sessionID", warpClient->getSessionID());
            
            userDefaults->flush();
            
        }

        //scene
        
        auto scene = MenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionShrinkGrow::create(.6, scene));

        
        //başarılı bağlantı recover
        
    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {
        autoLogining = false;
        
        scheduleRecover();
        printf("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
        
        //tekrar deneyecek
        
        actOfScene = tempScene;
        
        this->removeChild(loader);
        this->removeChild(loaderBack);
        this->removeChild(blackBg);
        this->removeChild(connectingLabel);
        
        if(appManager->currentLanguage() == false){
        
            openErrorMenu("Sunucuya Bağlanıyor...");
            
        }
        else {
            
            openErrorMenu("Connecting to the server...");
            
        }
        
    }
    else if (res==AppWarp::ResultCode::bad_request)
    {
        autoLogining = false;
        
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
        
        //uyarı ver bad request
    
        //bağlantınız yok...
        
        actOfScene = tempScene;
        
        this->removeChild(loader);
        this->removeChild(loaderBack);
        this->removeChild(blackBg);
        this->removeChild(connectingLabel);
        
        if(appManager->currentLanguage() == false){
        
            openErrorMenu("Sunucuya Bağlanırken Hata Oluştu");
            
        }
        else {
            
            openErrorMenu("An error has occured while connecting");
            
        }
        
    }
    else if(res == AppWarp::ResultCode::auth_error){
        
        if(reasonCode == AppWarp::ReasonCode::waiting_for_paused_user){
            
            UserDefault *userDefaults = UserDefault::getInstance();
            
            if(userDefaults->getIntegerForKey("sessionID") != 0){
                
                //server is really waiting for paused user. what can we do ?
                
                //we can recover connection with sessionID
                
                int sessionID = userDefaults->getIntegerForKey("sessionID");
                
                AppWarp::Client::getInstance()->recoverConnectionWithSessionID(sessionID, appManager->username);
                
            }
            else {
                
                if(userDefaults->getIntegerForKey("tempSessionID") != 0){
                    
                    int sessionID = userDefaults->getIntegerForKey("tempSessionID");
                    
                    AppWarp::Client::getInstance()->recoverConnectionWithSessionID(sessionID, appManager->username);
                    
                }
                else {
                
                    AppWarp::Client *warpClientRef;
                    warpClientRef = AppWarp::Client::getInstance();
                    warpClientRef->setRecoveryAllowance(60);
                    warpClientRef->setConnectionRequestListener(this);
                
                    warpClientRef->connect(appManager->username);
                    
                }
                
            }
            
        }
        else {

            /*autoLogining = false;
            
            unscheduleRecover();
            printf("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
            
            //uyarı ver bad request
            
            //bağlantınız yok...
            
            
            
            actOfScene = tempScene;
            
            this->removeChild(loader);
            this->removeChild(loaderBack);
            this->removeChild(blackBg);
            this->removeChild(connectingLabel);
            
            if(appManager->currentLanguage() == false){
                
                openErrorMenu("Sunucuya Bağlanırken Hata Oluştu");
                
            }
            else {
                
                openErrorMenu("An error has occured while connecting2222");
                
            }*/
            
            AppWarp::Client *warpClientRef;
            warpClientRef = AppWarp::Client::getInstance();
            warpClientRef->setRecoveryAllowance(60);
            warpClientRef->setConnectionRequestListener(this);
            
            warpClientRef->connect(appManager->username);
           
        }
        
    }
    else
    {
        autoLogining = false;
        
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with reasonCode=%d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
        
        //uyarı ve reason code
        
        actOfScene = tempScene;
        
        this->removeChild(loader);
        this->removeChild(loaderBack);
        this->removeChild(blackBg);
        this->removeChild(connectingLabel);
        
        if(appManager->currentLanguage() == false){
            
            openErrorMenu("Sunucuya Bağlanırken Hata Oluştu");
            
        }
        else {
            
            openErrorMenu("An error has occured while connecting");
            
        }
        
        
    }
    
    
}
void HelloWorld::recover(float dt)
{
    printf("\nHelloWorld::recover");
    AppWarp::Client::getInstance()->recoverConnection();
    

    
}

void HelloWorld::scheduleRecover()
{
    printf("\nHelloWorld::scheduleRecover");
    this->schedule(schedule_selector(HelloWorld::recover), 1.0f);
    
    //showReconnectingLayer("Reconnecting ...");
}

void HelloWorld::unscheduleRecover()
{
    printf("\nHelloWorld::unscheduleRecover");
    this->unschedule(schedule_selector(HelloWorld::recover));
}

void HelloWorld::editBoxEditingDidBegin(EditBox* editBox){
    

    
}

void HelloWorld::editBoxTextChanged(EditBox* editBox, string &text){
    
    
    
}
void HelloWorld::editBoxReturn(EditBox * editBox){
    
    
    
}

void HelloWorld::update(float dt){
    
    
    
    
}


bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(actOfScene == 0){
    
        if(girisYap->getBoundingBox().containsPoint(touch->getLocation())){
        
            girisYap->setOpacity(128);
        
        }
        else if(uyeOl->getBoundingBox().containsPoint(touch->getLocation())){
        
            uyeOl->setOpacity(128);
        
        }
        
    }
    else if(actOfScene == 1 || actOfScene == 2){
        
        if(autoLogining == false){
        
        if(actOfScene == 1){
            
            if(forgotPassButton){
            
                if(forgotPassButton->getBoundingBox().containsPoint(touch->getLocation())){
                
                    forgotPassButton->setOpacity(128);
                
                }
                
            }
            
        }
        
        if(tamamButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            tamamButton->setOpacity(128);
            
        }
        else if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
                
            backButton->setOpacity(128);
                
        }
            
        }

        
    }
    else if(actOfScene == 5){
        
        if(tamamButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            tamamButton->setOpacity(128);
            
        }
        else if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(128);
            
        }
        
    }
    
    
        return true;
    
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(actOfScene == 0){
        
        if(!girisYap->getBoundingBox().containsPoint(touch->getLocation())){
            
            girisYap->setOpacity(255);
            
        }
        else if(!uyeOl->getBoundingBox().containsPoint(touch->getLocation())){
            
            uyeOl->setOpacity(255);
            
        }
        
    }
    else if(actOfScene == 1){
        
        if(autoLogining == false){
        
        if(!tamamButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            tamamButton->setOpacity(255);
            
        }
        else if(!forgotPassButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            forgotPassButton->setOpacity(255);
            
        }
        else if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }
            
        }
   
        

        
    }
    else if(actOfScene == 2){
        
        
        if(!tamamButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            tamamButton->setOpacity(255);
            
        }
        else if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }
        
    }
    else if(actOfScene == 5){
        
        if(!tamamButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            tamamButton->setOpacity(255);
            
        }
        else if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }
        
    }
    
    
    
}


void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(actOfScene == 0){
        
        if(girisYap->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            //giris yap action
            girisYap->setOpacity(255);
            girisAction();
            
        }
        else if(uyeOl->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            //uye ol action
            girisYap->setOpacity(255);
            uyeOlAction();
            
        }
        
    }
    else if(actOfScene == 5){
        
        if(tamamButton->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            tamamButton->setOpacity(255);
            
            usernamelabel = cocos2d::__String::create(name->getText());
            
            if(!usernamelabel->isEqual(cocos2d::__String::create(""))){
                
                this->findUserByEmailId(usernamelabel->getCString());
                
                //this->sifreYenile(usernamelabel->getCString());
                
            }
            
        }
        else if(backButton->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            backButton->setOpacity(255);
            
            this->removeResetScene();;
            
        }
        
    }
    else if(actOfScene == 1){
        
        if(autoLogining == false){
        
        if(tamamButton->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            tamamButton->setOpacity(255);
            
            usernamelabel = cocos2d::__String::create(name->getText());
            passwordlabel = cocos2d::__String::create(password->getText());
            
            appManager->password = passwordlabel->getCString();
            
            
            if(!usernamelabel->isEqual(cocos2d::__String::create("")) && !passwordlabel->isEqual(cocos2d::__String::create(""))){
            
                //tamamButton->setOpacity(0);
                
                //tamamLabel->setOpacity(0);
                
                this->removeGirisSceneForLogin();
                
                auto scheduler = Director::getInstance()->getScheduler();
                
                scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::girisLoaderAnimation,this));

                
                authenticateUser(usernamelabel->getCString(), passwordlabel->getCString());
                
            }
            
        }
        else if(forgotPassButton->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            forgotPassButton->setOpacity(255);
            
            this->resetUserPassword();
            
        }
        else if(backButton->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            backButton->setOpacity(255);
            
            this->removeGirisScene();
            
        }
       
        }
        
    }
    else if(actOfScene == 2){
        
        if(tamamButton->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            tamamButton->setOpacity(255);
            
            usernamelabel = cocos2d::__String::create(name->getText());
            passwordlabel = cocos2d::__String::create(password->getText());
            emaillabel = cocos2d::__String::create(email->getText());
            
            appManager->password = passwordlabel->getCString();
            
            if(!usernamelabel->isEqual(cocos2d::__String::create("")) && !passwordlabel->isEqual(cocos2d::__String::create("")) && !emaillabel->isEqual(cocos2d::__String::create(""))){
                
                kayitUserName = usernamelabel->getCString();
                
                kayitParola = passwordlabel->getCString();
                
                kayitEmail = emaillabel->getCString();
                
                if(kayitParola.length() < 5){
                    
                    if(appManager->currentLanguage() == false){
                    
                        openErrorMenu("Şifre en az 5 haneden oluşmalı");
                        
                    }
                    else {
                        
                        openErrorMenu("Password must have at least 5 characters");
                        
                    }
                    
                }
                else {
                    
                    this->removeGirisSceneForRegister();
                    
                    loaderBack = Sprite::create("intro1.png");
                    FIT_H(loaderBack, WIDTH*.115);
                    loaderBack->setPosition(Vec2(WIDTH*.9,HEIGHT*.13));
                    this->addChild(loaderBack);
                    loaderBack->setOpacity(255);
                    
                    loader = Sprite::create("intro2.png");
                    FIT_H(loader, WIDTH*.17);
                    loader->setPosition(Vec2(WIDTH*.9, HEIGHT*.13));
                    this->addChild(loader);
                    
                    auto anim = RotateBy::create(1, 360);
                    
                    auto repeat = RepeatForever::create(anim);
                    
                    loader->runAction(repeat);
                    
                    auto anim2 = RotateBy::create(1, -360);
                    
                    auto repeat2 = RepeatForever::create(anim2);
                    
                    loaderBack->runAction(repeat2);
                    
                    if(appManager->currentLanguage() == false){
                    
                        connectingLabel = Label::createWithTTF("KAYıT YAPıLıYOR", "chinese rocks rg.ttf", HEIGHT*.045);
                        
                    }
                    else {
                        
                        connectingLabel = Label::createWithTTF("REGISTERING", "chinese rocks rg.ttf", HEIGHT*.045);
                        
                    }
                    
                    connectingLabel->setTextColor(Color4B(255, 255, 255, 220));
                    connectingLabel->setPosition(Vec2(WIDTH*.5,HEIGHT*.1));
                    this->addChild(connectingLabel);
                    
                    auto fadeOutAnim = FadeOut::create(.4);
                    
                    auto fadeInAnim = FadeIn::create(.4);
                    
                    auto seq = Sequence::create(fadeOutAnim, fadeInAnim, NULL);
                    
                    auto repeatLabel = RepeatForever::create(seq);
                    
                    connectingLabel->runAction(repeatLabel);
                    
                    tamamButton->setOpacity(0);
                    
                    tamamLabel->setOpacity(0);

                    
                    app42CreatePlayer(usernamelabel->getCString(), passwordlabel->getCString(), emaillabel->getCString());
                    
                    
                    
                }

                
                
                
            }
            
            
        }
        else if (backButton->getOpacity() == 128){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
            backButton->setOpacity(255);
            
            this->removeUyeOlScene();
            
        }
        
        
    }
    
}

void HelloWorld::autoLogin() {
    
    autoLogining = true;
    
    actOfScene = 1;
    
    menuBackground = Sprite::create("MenuBackground.png");
    FIT_H(menuBackground, HEIGHT);
    menuBackground->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(menuBackground);
    menuBackground->setOpacity(128);

    UserDefault *userDefaults = UserDefault::getInstance();
    
    std::string username = userDefaults->getStringForKey("username");
    
    std::string password = userDefaults->getStringForKey("password");
    
    auto scheduler = Director::getInstance()->getScheduler();
    
    scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::girisLoaderAnimation,this));
    
    this->authenticateUser(username.c_str(), password.c_str());
    
    
}

void HelloWorld::createMainScene() {
    
    UserDefault *userDefaults = UserDefault::getInstance();
    
    userDefaults->setIntegerForKey("sessionID", NULL);
    
    userDefaults->flush();
    
    menuBackground = Sprite::create("MenuBackground.png");
    FIT_H(menuBackground, HEIGHT);
    menuBackground->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(menuBackground);
    menuBackground->setOpacity(128);
    
    girisYap = Sprite::create("kgbuton.png");
    
    Label *girisLabel;
    
    if(appManager->currentLanguage() == false){
    
        girisLabel = Label::createWithTTF("GİRİŞ YAP", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        girisLabel = Label::createWithTTF("LOG IN", "chinese rocks rg.ttf", 50);
        
    }
    
    girisLabel->setPosition(Vec2(girisYap->getContentSize().width/2,girisYap->getContentSize().height/2));
    girisYap->addChild(girisLabel);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(girisYap, WIDTH*.07);
            
            girisYap->setPosition(Vec2(WIDTH*.35, HEIGHT*.2));
            
        }
        else {
            
            FIT_H(girisYap, HEIGHT*.14);
            
            girisYap->setPosition(Vec2(WIDTH*.3, HEIGHT*.2));
            
        }
        
    }
    else {
        
        FIT_H(girisYap, HEIGHT*.14);
        
        girisYap->setPosition(Vec2(WIDTH*.3, HEIGHT*.2));
        
    }
    
    this->addChild(girisYap);
    //girisYap->setOpacity(0);
    
    uyeOl = Sprite::create("kgbuton.png");
    
    Label *uyeolLabel;
    
    if(appManager->currentLanguage() == false){
        
        uyeolLabel = Label::createWithTTF("Üye Ol", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        uyeolLabel = Label::createWithTTF("Register", "chinese rocks rg.ttf", 50);
        
    }
    
    uyeolLabel->setPosition(Vec2(uyeOl->getContentSize().width/2,uyeOl->getContentSize().height/2));
    uyeOl->addChild(uyeolLabel);
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(uyeOl, WIDTH*.07);
        
            uyeOl->setPosition(Vec2(WIDTH*.65, HEIGHT*.2));
            
            
        }
        else {
            
            FIT_H(uyeOl, HEIGHT*.14);
            
            uyeOl->setPosition(Vec2(WIDTH*.7, HEIGHT*.2));
            
        }
        
        
    }
    else {
        
         FIT_H(uyeOl, HEIGHT*.14);
        
        uyeOl->setPosition(Vec2(WIDTH*.7, HEIGHT*.2));
        
    }
   
    
    this->addChild(uyeOl);
    //uyeOl->setOpacity(0);
    

    
}

void HelloWorld::girisAction() {
    
    
    menuBackground->setOpacity(50);
    
    auto anim = MoveTo::create(.3, Vec2(-WIDTH*.5, HEIGHT*.2));
    auto anim2 = MoveTo::create(.3, Vec2(WIDTH*1.5, HEIGHT*.2));
    
    Sequence *seq1 = Sequence::create(anim,RemoveSelf::create(), NULL);
    Sequence  *seq2 = Sequence::create(anim2,RemoveSelf::create(), NULL);
    
    girisYap->runAction(seq1);
    uyeOl->runAction(seq2);
    
    actOfScene = 1;
    
    createGirisScene();
    
    
}

void HelloWorld::uyeOlAction() {
    
    
    auto anim = MoveTo::create(.3, Vec2(-WIDTH*.5, HEIGHT*.2));
    auto anim2 = MoveTo::create(.3, Vec2(WIDTH*1.5, HEIGHT*.2));
    
    Sequence *seq1 = Sequence::create(anim,RemoveSelf::create(), NULL);
    Sequence  *seq2 = Sequence::create(anim2,RemoveSelf::create(), NULL);
    
    girisYap->runAction(seq1);
    uyeOl->runAction(seq2);
    
    actOfScene = 2;
    
    
    //create uye ol scene
    
    createUyeOlScene();
    
    
}

void HelloWorld::findUserByEmailId(const char *email){
    
    UserService *userService = this->initApp42Service();
    
    userService->GetUserByEmailId(email, app42callback(HelloWorld::onFindUserByEmailCompleted, this));
    
}

void HelloWorld::onFindUserByEmailCompleted(void *response){
    
    App42UserResponse *userResponse = (App42UserResponse*)response;
    printf("\ncode=%d...=%d",userResponse->getCode(),userResponse->isSuccess);
    printf("\nResponse Body=%s",userResponse->getBody().c_str());
    
    
    if (userResponse->isSuccess)
    {
        for(std::vector<App42User>::iterator it = userResponse->users.begin(); it != userResponse->users.end(); ++it)
        {
            printf("\n User name is %s",it->userName.c_str());
            
            this->sifreYenile(it->userName.c_str());
            
            /*printf("\n Email is %s",it->email.c_str());
            //Fetching Profile Info
            printf("\n FirstName=%s",it->profile.firstName.c_str());
            printf("\n LastName=%s",it->profile.lastName.c_str());
            printf("\n Sex=%s",it->profile.getSex().c_str());
            printf("\n Date of Birth=%s",it->profile.dateOfBirth.c_str());
            printf("\n Line1=%s",it->profile.line1.c_str());
            printf("\n Line2=%s",it->profile.line2.c_str());
            printf("\n City=%s",it->profile.city.c_str());
            printf("\n State=%s",it->profile.state.c_str());
            printf("\n Pincode=%s",it->profile.pincode.c_str());
            printf("\n Country=%s",it->profile.country.c_str());
            printf("\n Mobile=%s",it->profile.mobile.c_str());
            printf("\n HomeLandLine=%s",it->profile.homeLandLine.c_str());
            printf("\n OfficeLandLine=%s",it->profile.officeLandLine.c_str());
            */
             
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
        
        
        if(err == StringUtils::format("User by the name '%s' does not exist.",kayitUserName.c_str())){
            
            err = StringUtils::format("Kayıtlı kullanıcı bulunamadı");
            
        }
        else if(err == StringUtils::format("The request parameters are invalid. Username '%s' already exists.",kayitUserName.c_str())){
            
            err = StringUtils::format("Kullanıcı adı daha önceden alınmış");
            
        }
        else if(err == StringUtils::format("UserName/Password did not match. Authentication Failed.")){
            
            err = StringUtils::format("Kullanıcı adınız veya şifreniz yanlış.");
            
        }
        else if(err == StringUtils::format("The request parameters are invalid. User with emailId '%s' already exists.",kayitEmail.c_str())){
            
            err = StringUtils::format("E-Mail adresi daha önceden alınmış");
            
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
        else if(userResponse->appErrorCode == 2004){
            
            err = StringUtils::format("E-Mail adresi kayıtlı değil");
            
        }
            
        }
        
        openErrorMenu(err);

        
    }
    
}

void HelloWorld::sifreYenile(const char *username){
    
    UserService *userService = this->initApp42Service();
    
    userService->ResetUserPassword(username, app42callback(HelloWorld::onUserPasswordResetCompleted, this));
    
}

void HelloWorld::onUserPasswordResetCompleted(void *response){
    
    App42UserResponse *userResponse = (App42UserResponse*)response;
    printf("\ncode=%d...=%d",userResponse->getCode(),userResponse->isSuccess);
    printf("\nResponse Body=%s",userResponse->getBody().c_str());
    if (userResponse->isSuccess)
    {
        for(std::vector<App42User>::iterator it = userResponse->users.begin(); it != userResponse->users.end(); ++it)
        {
            printf("\n User name is %s",it->userName.c_str());
            
            if(appManager->currentLanguage() == false){
            
                this->openErrorMenu(StringUtils::format("Şifreniz E-Mail adresinize gönderildi"));
                
            }
            else {
                
                this->openErrorMenu(StringUtils::format("New password has been sent to e-mail adress"));
                
            }
            
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
        
        if(err == StringUtils::format("User by the name '%s' does not exist.",kayitUserName.c_str())){
            
            err = StringUtils::format("Kayıtlı kullanıcı bulunamadı");
            
        }
        else if(err == StringUtils::format("The request parameters are invalid. Username '%s' already exists.",kayitUserName.c_str())){
            
            err = StringUtils::format("Kullanıcı adı daha önceden alınmış");
            
        }
        else if(err == StringUtils::format("UserName/Password did not match. Authentication Failed.")){
            
            err = StringUtils::format("Kullanıcı adınız veya şifreniz yanlış");
            
        }
        else if(err == StringUtils::format("The request parameters are invalid. User with emailId '%s' already exists.",kayitEmail.c_str())){
            
            err = StringUtils::format("E-Mail adresi daha önceden alınmış");
            
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
        else if(userResponse->appErrorCode == 2004){
            
            err = StringUtils::format("E-Mail adresi bulunamadı");
            
        }
            
        }
        
        openErrorMenu(err);
        
    }

    
}

void HelloWorld::removeResetScene() {
    
    actOfScene = 1;
    
    this->removeAllChildren();
    
    menuBackground = Sprite::create("MenuBackground.png");
    FIT_H(menuBackground, HEIGHT);
    menuBackground->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(menuBackground);
    menuBackground->setOpacity(128);

    this->createGirisScene();
    
    
    
}

void HelloWorld::removeUyeOlScene() {
    
    actOfScene = 0;
    
    this->removeAllChildren();
    
    this->createMainScene();
    
}

void HelloWorld::removeGirisSceneForLogin() {
    
    nameSp->setOpacity(0);
    
    passSp->setOpacity(0);
    
    forgotPassButton->setOpacity(0);
    
    tamamButton->setOpacity(0);
    
    this->removeChild(name);
    
    this->removeChild(password);
    
    tamamLabel->setOpacity(0);
    
    forgotLabel->setOpacity(0);
    
}

void HelloWorld::removeGirisSceneForRegister() {
    
    nameSp->setOpacity(0);
    
    passSp->setOpacity(0);
    
    emailSp->setOpacity(0);
    
    this->removeChild(name);
    
    this->removeChild(password);
    
    this->removeChild(email);
    
    tamamButton->setOpacity(0);
    
    tamamLabel->setOpacity(0);
    
}

void HelloWorld::removeGirisScene() {
    
    actOfScene = 0;
    
    this->removeAllChildren();
    
    this->createMainScene();
    
}

void HelloWorld::resetUserPassword() {
    
    
    actOfScene = 5;
    
    this->removeAllChildren();
    
    menuBackground = Sprite::create("MenuBackground.png");
    FIT_H(menuBackground, HEIGHT);
    menuBackground->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(menuBackground);
    menuBackground->setOpacity(128);
    
    backButton = Sprite::create("backButton.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(backButton, HEIGHT*.07);
            
            
            
        }
        else {
            
            FIT_H(backButton, HEIGHT*.1);
            
        }
        
        
    }
    else {
        
        FIT_H(backButton, HEIGHT*.1);
        
    }
    backButton->setPosition(Vec2(WIDTH*.08, HEIGHT*.1));
    this->addChild(backButton);
    
    nameSp = Sprite::create("usernamePassword.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(nameSp,WIDTH*.25, HEIGHT*.1);
            
            
            
        }
        else {
            
            FIT_WH(nameSp, WIDTH*.4, HEIGHT*.3);
            
        }
        
        
    }
    else {
        
        FIT_WH(nameSp, WIDTH*.4, HEIGHT*.3);
        
    }
    this->addChild(nameSp);
    nameSp->setPosition(Vec2(WIDTH*.35,HEIGHT*.2));
    
    name = EditBox::create(Size(WIDTH*.25, HEIGHT*.07), Scale9Sprite::create("usernamepassword.png"));
    name->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    name->setPosition(Vec2(WIDTH*.35, HEIGHT*.2));
    name->setFontSize(HEIGHT*.045);
    name->setFontColor(Color3B::WHITE);
    name->setMaxLength(50);
    name->setPlaceHolder("E-Mail");
    name->setPlaceholderFontColor(Color3B::WHITE);
    name->setPlaceholderFontSize(HEIGHT*.045);
    
    
    
    name->setReturnType(EditBox::KeyboardReturnType::DONE);
    name->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    name->setDelegate(this);
    this->addChild(name);
    

    
    tamamButton = Sprite::create("kgbuton.png");
    
    if(appManager->currentLanguage() == false){
    
        tamamLabel = Label::createWithTTF("ŞİFRE GÖNDER", "chinese rocks rg.ttf", 40);
        
    }
    else {
        
        tamamLabel = Label::createWithTTF("RESET PASSWORD", "chinese rocks rg.ttf", 40);

    }
    tamamButton->addChild(tamamLabel);
    tamamLabel->setPosition(Vec2(tamamButton->getContentSize().width/2, tamamButton->getContentSize().height/2));
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(tamamButton, HEIGHT*.1);
            
            
        }
        else {
            
            FIT_H(tamamButton, HEIGHT*.15);
        }
        
        
    }
    else {
        
        FIT_H(tamamButton, HEIGHT*.15);
    }
    tamamButton->setPosition(Vec2(WIDTH*.7, HEIGHT*.2));
    this->addChild(tamamButton);

    
    
}


void HelloWorld::createUyeOlScene(){
    
    
    backButton = Sprite::create("backButton.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(backButton, HEIGHT*.07);
            
            
            
        }
        else {
            
            FIT_H(backButton, HEIGHT*.1);
            
        }
        
        
    }
    else {
    
        FIT_H(backButton, HEIGHT*.1);
        
    }
    backButton->setPosition(Vec2(WIDTH*.08, HEIGHT*.1));
    this->addChild(backButton);
    
    
    nameSp = Sprite::create("usernamePassword.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(nameSp, WIDTH*.33, HEIGHT*.25);
            
           
        }
        else {
            
            FIT_WH(nameSp, WIDTH*.4, HEIGHT*.3);
            
        }
        
        
    }
    else {
        
        FIT_WH(nameSp, WIDTH*.4, HEIGHT*.3);
        
    }
    this->addChild(nameSp);
    nameSp->setPosition(Vec2(WIDTH*.5,HEIGHT*.8));
    
    
    passSp = Sprite::create("usernamePassword.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(passSp, WIDTH*.33, HEIGHT*.25);
            
            
        }
        else {
            
            FIT_WH(passSp, WIDTH*.4, HEIGHT*.3);
        }
        
        
    }
    else {
        
        FIT_WH(passSp, WIDTH*.4, HEIGHT*.3);
    }
    this->addChild(passSp);
    passSp->setPosition(Vec2(WIDTH*.5, HEIGHT*.6));
    
    
    emailSp = Sprite::create("usernamePassword.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(emailSp, WIDTH*.33, HEIGHT*.25);
            
            
        }
        else {
            
            FIT_WH(emailSp, WIDTH*.4, HEIGHT*.3);
            
        }

    }
    else {
    
        FIT_WH(emailSp, WIDTH*.4, HEIGHT*.3);
        
    }
    this->addChild(emailSp);
    emailSp->setPosition(Vec2(WIDTH*.5,HEIGHT*.4));
    
    name = EditBox::create(Size(WIDTH*.4, HEIGHT*.3), Scale9Sprite::create("usernamepassword.png"));
    name->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    name->setPosition(Vec2(WIDTH*.58, HEIGHT*.8));
    name->setFontSize(HEIGHT*.05);
    name->setFontColor(Color3B::WHITE);
    name->setMaxLength(10);
    const char *placeHolder;
    if(appManager->currentLanguage() == false){
        placeHolder = "Kullanıcı Adı";
    }
    else {
        
        placeHolder = "Username";
    }
    name->setPlaceHolder(placeHolder);
    name->setPlaceholderFontColor(Color3B::WHITE);
    name->setPlaceholderFontSize(HEIGHT*.05);
    

    name->setReturnType(EditBox::KeyboardReturnType::DONE);
    name->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    name->setDelegate(this);
    this->addChild(name);
    
    
    password = EditBox::create(Size(WIDTH*.25, HEIGHT*.1), Scale9Sprite::create("usernamepassword.png"));
    password->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    password->setPosition(Vec2(WIDTH*.5, HEIGHT*.6));
    password->setFontSize(HEIGHT*.05);
    password->setFontColor(Color3B::WHITE);
    password->setMaxLength(15);
    const char *placePassHolder;
    if(appManager->currentLanguage() == false){
        placePassHolder = "Parola";
    }
    else {
        placePassHolder = "Password";
    }
    password->setPlaceHolder(placePassHolder);
    password->setPlaceholderFontColor(Color3B::WHITE);
    password->setPlaceholderFontSize(HEIGHT*.05);
    password->setTag(1);
    
    
    password->setReturnType(EditBox::KeyboardReturnType::DONE);
    password->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    password->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    password->setDelegate(this);
    this->addChild(password);
    
    
    email = EditBox::create(Size(WIDTH*.25, HEIGHT*.07), Scale9Sprite::create("usernamepassword.png"));
    email->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    email->setPosition(Vec2(WIDTH*.5, HEIGHT*.4));
    email->setFontSize(HEIGHT*.045);
    email->setFontColor(Color3B::WHITE);
    email->setMaxLength(50);
    email->setPlaceHolder("E-Mail");
    email->setPlaceholderFontColor(Color3B::WHITE);
    email->setPlaceholderFontSize(HEIGHT*.045);
    
    email->setReturnType(EditBox::KeyboardReturnType::DONE);
    email->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    email->setDelegate(this);
    this->addChild(email);
    
    tamamButton = Sprite::create("kgbuton.png");
    if(appManager->currentLanguage() == false){
        tamamLabel = Label::createWithTTF("Tamam", "chinese rocks rg.ttf", 50);
    }
    else {
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
    }
    tamamButton->addChild(tamamLabel);
    tamamLabel->setPosition(Vec2(tamamButton->getContentSize().width/2, tamamButton->getContentSize().height/2));
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(tamamButton, HEIGHT*.1);
            
            
        }
        else {
            
            FIT_H(tamamButton, HEIGHT*.15);
        }
        
        
    }
    else {
    
        FIT_H(tamamButton, HEIGHT*.15);
    }
    tamamButton->setPosition(Vec2(WIDTH*.5, HEIGHT*.2));
    this->addChild(tamamButton);


    
}


void HelloWorld::createGirisScene(){
    
    if(backButton){
        
        this->removeChild(backButton);
        
    }
    
    
    backButton = Sprite::create("backButton.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(backButton, HEIGHT*.07);
            
            
            
        }
        else {
            
            FIT_H(backButton, HEIGHT*.1);
            
        }
        
        
    }
    else {
        
        FIT_H(backButton, HEIGHT*.1);
        
    }
    
    backButton->setPosition(Vec2(WIDTH*.08, HEIGHT*.1));
    this->addChild(backButton);
    
    if(nameSp){
        
        this->removeChild(nameSp);
        
    }
    
    nameSp = Sprite::create("usernamePassword.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(nameSp,WIDTH*.35, HEIGHT*.2);
            
            
            
        }
        else {
            
            FIT_WH(nameSp, WIDTH*.4, HEIGHT*.3);
            
        }
        
        
    }
    else {
        
        FIT_WH(nameSp, WIDTH*.4, HEIGHT*.3);
        
    }
    this->addChild(nameSp);
    nameSp->setPosition(Vec2(WIDTH*.3,HEIGHT*.35));
    
    if(passSp){
        
        this->removeChild(passSp);
        
    }
    
    passSp = Sprite::create("usernamePassword.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(passSp,WIDTH*.35, HEIGHT*.2);
            
            
            
        }
        else {
            
            FIT_WH(passSp, WIDTH*.4, HEIGHT*.3);
        }
        
        
    }
    else {
        
        FIT_WH(passSp, WIDTH*.4, HEIGHT*.3);
    }
    this->addChild(passSp);
    passSp->setPosition(Vec2(WIDTH*.7, HEIGHT*.35));
    
    if(name){
        
        this->removeChild(name);
        
    }
    
    name = EditBox::create(Size(WIDTH*.2, HEIGHT*.07), Scale9Sprite::create("usernamepassword.png"));
    name->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    name->setPosition(Vec2(WIDTH*.3, HEIGHT*.35));
    name->setFontSize(HEIGHT*.05);
    name->setFontName("chinese rocks rg.ttf");
    name->setFontColor(Color3B::WHITE);
    name->setMaxLength(10);
    const char*placeHolder;
    if(appManager->currentLanguage() == false){
        placeHolder = "Kullanıcı Adı";
    }
    else {
        placeHolder = "Username";
    }
    name->setPlaceHolder(placeHolder);
    name->setPlaceholderFontColor(Color3B::WHITE);
    name->setPlaceholderFontSize(HEIGHT*.05);

    
    name->setReturnType(EditBox::KeyboardReturnType::DONE);
    name->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    name->setDelegate(this);
    this->addChild(name);
    
    if(password){
        
        this->removeChild(password);
        
    }
    
    password = EditBox::create(Size(WIDTH*.25, HEIGHT*.07), Scale9Sprite::create("usernamepassword.png"));
    password->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    password->setPosition(Vec2(WIDTH*.7, HEIGHT*.35));
    password->setFontSize(HEIGHT*.05);
    password->setFontColor(Color3B::WHITE);
    password->setMaxLength(15);
    const char *placePassHolder;
    if(appManager->currentLanguage() == false){
        placePassHolder = "Parola";
    }
    else{
        placePassHolder = "Password";
    }
    password->setPlaceHolder(placePassHolder);
    password->setPlaceholderFontColor(Color3B::WHITE);
    password->setPlaceholderFontSize(HEIGHT*.05);
    
    
    password->setReturnType(EditBox::KeyboardReturnType::DONE);
    password->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    password->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    password->setDelegate(this);
    this->addChild(password);
    
    
    tamamButton = Sprite::create("kgbuton.png");
    if(appManager->currentLanguage() == false){
        tamamLabel = Label::createWithTTF("Tamam", "chinese rocks rg.ttf", 50);
    }
    else {
        tamamLabel = Label::createWithTTF("OK", "chinese rocks rg.ttf", 50);
    }
    tamamButton->addChild(tamamLabel);
    tamamLabel->setPosition(Vec2(tamamButton->getContentSize().width/2, tamamButton->getContentSize().height/2));
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(tamamButton, HEIGHT*.1);
            
        }
        else {
            
            FIT_H(tamamButton, HEIGHT*.15);
            
        }
        
        
    }
    else {
    FIT_H(tamamButton, HEIGHT*.15);
        
    }
    tamamButton->setPosition(Vec2(WIDTH*.65, HEIGHT*.15));
    this->addChild(tamamButton);
    
    forgotPassButton = Sprite::create("kgbuton.png");
    if(appManager->currentLanguage() == false){
        forgotLabel = Label::createWithTTF("ŞİFREMİ UNUTTUM", "chinese rocks rg.ttf", 35);
    }
    else {
        forgotLabel = Label::createWithTTF("FORGOT PASSWORD", "chinese rocks rg.ttf", 35);
    }
    forgotPassButton->addChild(forgotLabel);
    forgotLabel->setPosition(Vec2(forgotPassButton->getContentSize().width/2, forgotPassButton->getContentSize().height/2));
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(forgotPassButton, HEIGHT*.1);
            
        }
        else {
            
            FIT_H(forgotPassButton, HEIGHT*.15);
            
        }
    
    }
    else {
       
        FIT_H(forgotPassButton, HEIGHT*.15);
        
    }
    
    forgotPassButton->setPosition(Vec2(WIDTH*.35, HEIGHT*.15));
 
    this->addChild(forgotPassButton);
    
    
}




void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
