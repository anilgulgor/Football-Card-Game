//
//  TurnuvaScene.cpp
//  Santra
//
//  Created by Anıl Gülgör on 16/12/15.
//
//

#include "TurnuvaScene.hpp"


Scene* TurnuvaScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TurnuvaScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool TurnuvaScene::init(){
    
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        this->setKeypadEnabled(true);
        
    }
    
    appManager = AppManager::getInstance();
    
    appManager->handleRecover();
    
    WIDTH = GET_WIDTH;
    HEIGHT = GET_HEIGHT;
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    
    
    setScene();
    createItems();
    
    return true;
    
}


void TurnuvaScene::onConnectDone(int res, int reasonCode){
    
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
void TurnuvaScene::recover(float dt)
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
        
        this->schedule(schedule_selector(TurnuvaScene::controlForConnection), 8.0f);
        
    }
    
}

void TurnuvaScene::scheduleRecover()
{
    printf("\nGameScene::scheduleRecover");
    this->schedule(schedule_selector(TurnuvaScene::recover), 1.0f);
    
    //showReconnectingLayer("Reconnecting ...");
}

void TurnuvaScene::unscheduleRecover()
{
    printf("\nGameScene::unscheduleRecover");
    this->unschedule(schedule_selector(TurnuvaScene::recover));
}



void TurnuvaScene::controlForConnection(float dt){
    
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


void TurnuvaScene::removeNoConnectionWarning(){
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel2);
    
}


void TurnuvaScene::noConnectionWarning(std::string st1, std::string st2, std::string st3){
    
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
        
        this->schedule(schedule_selector(TurnuvaScene::goBackMainForError),5.0f);
        
    }
    
}

void TurnuvaScene::goBackMainForError(float dt){
    
    this->unschedule(schedule_selector(TurnuvaScene::goBackMainForError));
    
    //****//
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
    
    
}

void TurnuvaScene::setScene(){
    
    background = Sprite::create("LigBG.png");
    FIT_WH(background, WIDTH, HEIGHT);
    background->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(background);
    
    
    backButton = Sprite::create("Geri Butonu.png");
    FIT_H(backButton, HEIGHT*.13);
    backButton->setPosition(Vec2(WIDTH*.07, HEIGHT*.08));
    this->addChild(backButton,2);
    
    std::string avatarNum = "avatar " + StringUtils::format("%.0f", (appManager->avatarNumber)) + ".png";
    
    userAvatar = Sprite::create(avatarNum);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(userAvatar, HEIGHT*.09);
            
        }
        else{
            
            FIT_H(userAvatar, HEIGHT*.13);
            
        }
        
    }
    else{
        
        FIT_H(userAvatar, HEIGHT*.13);

    }
    
    createTopViews();
    
}

void TurnuvaScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event){
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        /*UserDefault *userDefaults = UserDefault::getInstance();
        
        userDefaults->setIntegerForKey("sessionID", NULL);
        
        userDefaults->setIntegerForKey("tempSessionID", NULL);
        
        userDefaults->flush();
        
        AppWarp::Client *warpClientRef;
        
        warpClientRef = AppWarp::Client::getInstance();
        
        warpClientRef->leaveRoom(appManager->myCurrentRoomId);
        
        warpClientRef->disconnect();
        
        Director::getInstance()->end();
         
         */
        
        this->callQuitKeyAction();
        
    }
    
}


void TurnuvaScene::openAvatarPanel(){
    
    isAvatarPanelOpened = true;
    
    panelAvatar = Node::create();
    
    panelAvatar->setPosition(Vec2(-WIDTH,0));
    
    Sprite *panelAvatarBG = Sprite::create("Avatar Panel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panelAvatarBG,WIDTH*.9, HEIGHT*.9);
            
            
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
    
    Label *avatarLabelPanel = Label::createWithTTF("AVATAR", "chinese rocks rg.ttf", 50);
    
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
    
        avatarCikis->setPosition(Vec2(panelAvatarBG->getContentSize().width*.58,panelAvatarBG->getContentSize().height*.55));
    
    }
    
    avatarCikis->setZOrder(10);
    
    panelAvatar->addChild(avatarCikis);
    
    auto callBack = CallFunc::create([&]{
        
        
    });
    
    this->addChild(panelAvatar,50);
    
    this->createAvatarsPanel();
    
    auto moveAction = MoveTo::create(1.5, Vec2(0,0));
    
    auto seq = Sequence::create(moveAction,callBack, NULL);
    
    panelAvatar->runAction(seq);
    
}

void TurnuvaScene::createAvatarsPanel(){
    
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


void TurnuvaScene::createTopViews(){
    
    
    //sol taraf
   /* solUstBar = Sprite::create("SolUst.png");
    FIT_H(solUstBar, HEIGHT*.12);
    solUstBar->setPosition(Vec2(WIDTH*.25,HEIGHT*.91));
    this->addChild(solUstBar);
    
    Avatar = Sprite::create("Avatar.png");
    FIT_H(Avatar, HEIGHT*.12);
    Avatar->setPosition(Vec2(WIDTH*.08,HEIGHT*.91));
    this->addChild(Avatar);
    
    Ayarlar = Sprite::create("Ayarlar.png");
    FIT_H(Ayarlar, HEIGHT*.1);
    Ayarlar->setPosition(Vec2(WIDTH*.42,HEIGHT*.92));
    this->addChild(Ayarlar);
    
    Bildirim = Sprite::create("Bildirim.png");
    FIT_H(Bildirim, HEIGHT*.1);
    Bildirim->setPosition(Vec2(WIDTH*.48, HEIGHT*.92));
    this->addChild(Bildirim);
    
    //sağ taraf
    sagUst = Sprite::create("SagUst.png");
    FIT_H(sagUst, HEIGHT*.12);
    sagUst->setPosition(Vec2(WIDTH*.83,HEIGHT*.91));
    this->addChild(sagUst);
    
    arti = Sprite::create("ArtiButon.png");
    FIT_H(arti, HEIGHT*.1);
    arti->setPosition(Vec2(WIDTH*.665,HEIGHT*.92));
    this->addChild(arti);
    
    altin = Sprite::create("Altin.png");
    FIT_H(altin, HEIGHT*.1);
    altin->setPosition(Vec2(WIDTH*.88,HEIGHT*.92));
    this->addChild(altin);
    
    Krampon = Sprite::create("KramponMenu.png");
    FIT_H(Krampon, HEIGHT*.1);
    Krampon->setPosition(Vec2(WIDTH*.73, HEIGHT*.92));
    this->addChild(Krampon);*/
    
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
        Avatar->setPosition(Vec2(WIDTH*.07, HEIGHT*.91));
        
    }
 
    this->addChild(Avatar);
    
    userAvatar->setPosition(Avatar->getPosition());
    this->addChild(userAvatar);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
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
            
            bosLevelBar->setPosition(Vec2(WIDTH*.3, HEIGHT*.92));
            
        }
        else {
            
            FIT_W(bosLevelBar, WIDTH*.15);
            
            bosLevelBar->setPosition(Vec2(WIDTH*.3, HEIGHT*.92));
            
        }
        
    }
    else {
        
        FIT_W(bosLevelBar, WIDTH*.15);
        
        bosLevelBar->setPosition(Vec2(WIDTH*.3, HEIGHT*.92));
        
        
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
            
            levelBar->setPosition(Vec2(WIDTH*.3, HEIGHT*.92));
            
        }
        else {
            
            FIT_W(levelBar, WIDTH*.15);
            
            levelBar->setPosition(Vec2(WIDTH*.3, HEIGHT*.92));
            
        }
        
    }
    else {
        
        FIT_W(levelBar, WIDTH*.15);
        
        levelBar->setPosition(Vec2(WIDTH*.3, HEIGHT*.92));
        
    }
    
    this->addChild(levelBar);

    
    Ayarlar = Sprite::create("Ayarlar.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(Ayarlar, HEIGHT*.098);
            Ayarlar->setPosition(Vec2(WIDTH*.47,HEIGHT*.92));
            
            
        }
        else {
            
            FIT_H(Ayarlar, HEIGHT*.1);
            Ayarlar->setPosition(Vec2(WIDTH*.42,HEIGHT*.92));
            
        }
        
    }
    else {
        
        FIT_H(Ayarlar, HEIGHT*.1);
        Ayarlar->setPosition(Vec2(WIDTH*.42,HEIGHT*.92));
        
    }
    this->addChild(Ayarlar);
    
    Ayarlar->setOpacity(0);
    
    Bildirim = Sprite::create("Bildirim.png");
    FIT_H(Bildirim, HEIGHT*.1);
    Bildirim->setPosition(Vec2(WIDTH*.48, HEIGHT*.92));
    //this->addChild(Bildirim);
    
    //sağ taraf
    sagUst = Sprite::create("SagUst.png");
    FIT_WH(sagUst,WIDTH*.14 ,HEIGHT*.113);
    sagUst->setAnchorPoint(Vec2(1,.5));
    sagUst->setPosition(Vec2(WIDTH ,HEIGHT*.917));
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
        arti->setPosition(Vec2(WIDTH*.825 ,HEIGHT*.92));
        
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
            
            
            altin->setPosition(Vec2(WIDTH*.892,HEIGHT*.92));
            
        }
        
    }
    else {
        
        FIT_H(altin, HEIGHT*.075);
        
        
        altin->setPosition(Vec2(WIDTH*.892,HEIGHT*.92));
        
    }
    
    this->addChild(altin);
    
   
   
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
         goldLabel = Label::createWithBMFont("menuFont.fnt", StringUtils::format("%.0f", appManager->gold));
        
    }
    else {
        
         goldLabel =  Label::createWithBMFont("menuFont.fnt", StringUtils::format("%.0f", appManager->gold));
        
    }
    
    FIT_H(goldLabel, HEIGHT*.05);
    
    goldLabel->setColor(Color3B::WHITE);
    goldLabel->enableGlow(Color4B::BLACK);
    goldLabel->setPosition(Vec2(WIDTH*.94,HEIGHT*.915));
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


void TurnuvaScene::createItems(){
    
    actOfScene = 1;
    
    itemNode = Node::create();
    itemNode->setAnchorPoint(Vec2(0,0));
    addChild(itemNode);
    
    float ligBtnWid, ligbtnHgt;
    
    float ligPriceY;

    for(int i=0; i<5; i++){
        
        int btnlimitGold, btnlimitLevel;
        
        switch (i) {
                
            case 0:
                
                
                btnlimitGold = 100;
                btnlimitLevel = 0;
                
                break;
                
            case 1:
                
                
                btnlimitGold = 200;
                btnlimitLevel = 3;
                
                break;
                
            case 2:
                
                
                btnlimitGold = 400;
                btnlimitLevel = 5;
                
                break;
                
            case 3:
                
                
                
                btnlimitGold = 1000;
                btnlimitLevel = 10;
                
                break;
                
            case 4:
                
                
                
                btnlimitGold = 2000;
                btnlimitLevel = 20;
                
                break;
                
        }

        
        cocos2d::String *ligName;
        
        //Label *ligLabel,*ligPrice,*warnLabel;
        
        if(appManager->currentLanguage() == false){
            
            if(i != 4){
                ligName = cocos2d::String::createWithFormat("Lig%dTR.png",i+1);
            }
            else {
                ligName = cocos2d::String::createWithFormat("SwartagLigiTR.png");
                
            }
            /*ligLabel = Label::createWithTTF(StringUtils::format("%d. LİG", (i+1)), "chinese rocks rg.ttf", 80);
            ligPrice = Label::createWithTTF("GİRİŞ BEDELİ", "chinese rocks rg.ttf", 62);
            warnLabel = Label::createWithTTF(StringUtils::format("Giriş için %d seviyede olmalısınız",btnlimitLevel), "fonts/arial.ttf", 38);*/
            
        }
        else {
            
            if(i != 4){
                ligName = cocos2d::String::createWithFormat("League%d.png",i+1);
            }
            else{
                ligName = cocos2d::String::createWithFormat("SwartagLeagueing.png");
            }
            /*
            ligLabel = Label::createWithTTF(StringUtils::format("%d. LEAGUE", (i+1)), "chinese rocks rg.ttf", 80);
            ligPrice = Label::createWithTTF("ENTRANCE PRICE", "chinese rocks rg.ttf", 62);
            warnLabel = Label::createWithTTF(StringUtils::format("You must be at least %d level",btnlimitLevel), "fonts/arial.ttf", 38);*/

        }
        
       
        Sprite *ligButton = Sprite::create(ligName->getCString());
        
        FIT_H(ligButton, HEIGHT*.65);
        
        float dist = WIDTH*.5;
        
        dist = dist * i;
        
        ligButton->setPosition(Vec2(WIDTH*.5 + dist, HEIGHT*.5));
        itemNode->addChild(ligButton);
        
        //***
        
        
        if(i == 0){
        
            ligBtnWid = ligButton->getContentSize().width;
            ligbtnHgt = ligButton->getContentSize().height;
            
            ligPriceY = ligbtnHgt * 0.365;
            
        }
        
        /*ligLabel->setPosition(Vec2(ligBtnWid/2,(ligbtnHgt/2)*1.2));
        ligButton->addChild(ligLabel);
        
        
        ligPrice->setPosition(Vec2(ligBtnWid*.35, ligPriceY));
        ligButton->addChild(ligPrice);
        */
        
        Sprite *goldSprite = Sprite::create("Altin.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(goldSprite, HEIGHT*.1);

            }
            else {
                
                FIT_H(goldSprite, HEIGHT*.1);
                
            }
            
        }
        else {
            
            FIT_H(goldSprite, ligButton->getContentSize().height*.1);
            
        }

        
        goldSprite->setPosition(Vec2(ligBtnWid*.6,ligbtnHgt*.37));
        //ligButton->addChild(goldSprite);
        
        
        
        Label *limitLabel = Label::createWithTTF(StringUtils::format("%d",btnlimitGold), "fonts/MuseoSans.ttf", 67);
        limitLabel->setPosition(Vec2(ligBtnWid*.72,ligbtnHgt*.37));
        limitLabel->setColor(Color3B::YELLOW);
        //ligButton->addChild(limitLabel);
        
        if(i == 0){
            
            btnlimitLevel = 1;
            
        }
        
        
        /*warnLabel->setPosition(Vec2(ligBtnWid*.5,ligbtnHgt*.24));
        ligButton->addChild(warnLabel);*/

        ligButtonsVector.pushBack(ligButton);
        
        if(i == 1){
            
            distanceOfItems = dist;
            
        }
        
        
    }
    
    top = Sprite::create("Top.png");
    FIT_H(top, HEIGHT*.3);
    top->setPosition(Vec2(WIDTH*.5,HEIGHT*.15));
    this->addChild(top,2);

    
}

void TurnuvaScene::updateUserInformation(const char *username, double gold, double xp, double level, double avatar, double games, double won , double lost, double point){
    
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
    
    storageService->UpdateDocumentByKeyValue(db_name, collection_name, key, value, object, app42callback(TurnuvaScene::onStorageRequestCompleted, this));
    
}

void TurnuvaScene::onStorageRequestCompleted(void *response){
    
    App42StorageResponse *storageResponse = (App42StorageResponse *)response;
    
    printf("\n response body = %s", storageResponse->getBody().c_str());
    
    
    if(storageResponse->isSuccess){
        
        for(std::vector<App42Storage>::iterator it = storageResponse->storages.begin(); it != storageResponse->storages.end(); ++it){
            
            for(std::vector<JSONDocument>::iterator iit = it->jsonDocArray.begin(); iit != it->jsonDocArray.end(); ++iit){
                
                
                printf("\n jsonDoc = %s", iit->getJsonDoc().c_str());
                
                
                updateUserProfile(iit->getJsonDoc().c_str());
                
                auto scene = LoadingGameScene::createScene();

                Director::getInstance()->replaceScene(TransitionCrossFade::create(.5, scene));

                
            }
            
            
        }
        
    }
    else {
        
        printf("\n errordetails = %s", storageResponse->errorDetails.c_str());
        printf("\n errormessage = %s", storageResponse->errorMessage.c_str());
        
    }
    
    
    
}

void TurnuvaScene::updateUserProfile(const char *jsonString){
    
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
    
}

void TurnuvaScene::updateUserForAvatar(const char *username, double gold, double xp, double level, double avatar, double games, double won , double lost, double point){
    
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
    
    storageService->UpdateDocumentByKeyValue(db_name, collection_name, key, value, object, app42callback(TurnuvaScene::onAvatarStorageRequestComplete, this));
    
    
}

void TurnuvaScene::onAvatarStorageRequestComplete(void *response){
    
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






void TurnuvaScene::onEnterTransitionDidFinish() {
    
    Layer::onEnterTransitionDidFinish();
    
    this->schedule(schedule_selector(TurnuvaScene::update));
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(TurnuvaScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TurnuvaScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TurnuvaScene::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}

void TurnuvaScene::update(float dt){
    
    /*speed *= .9;
    
    if(speed > WIDTH*.08){
        
        speed = WIDTH*.08;
        
    }
    else if(speed < -WIDTH*.08){
        
        speed = -WIDTH*.08;
        
    }
    else if(speed != 0 && abs(speed) < WIDTH*.001){
        
        speed = 0;
        
    }
    
    if(itemNode->getPositionX() <= 0 && itemNode->getPositionX() >= -WIDTH*2.15){
        
        itemNode->setPositionX(itemNode->getPositionX() + speed*.99);
        
    }
    
    if(itemNode->getPositionX() > 0){
        
        itemNode->setPositionX(0);
        
    }
    else if(itemNode->getPositionX() < -WIDTH*2.15){
        
        itemNode->setPositionX(-WIDTH*2.15);
        
    }
     */
    
}

void TurnuvaScene::openWarningPanel(std::string warning){
    
    buttonTouch = false;
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(errorPanel);
    
    Sprite *panel = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panel, WIDTH*.8 ,HEIGHT*.8);
            
        }
        else {
            
            FIT_WH(panel,WIDTH*.8 ,HEIGHT*.8);
            
        }
        
        
    }
    else {
        
            FIT_WH(panel,WIDTH*.8 ,HEIGHT*.8);
        
    }
    
    errorPanel->addChild(panel);
    
    panel->setPosition(Vec2(0,0));
    
    //Label *warnLabel = Label::createWithTTF(warning, "chinese rocks rg.ttf", 50);
    
    Label *warnLabel = Label::createWithBMFont("menuFont.fnt", warning);
    
    FIT_H(warnLabel, HEIGHT*.1);
    
    warnLabel->setPosition(Vec2(0,0));
    
    errorPanel->addChild(warnLabel);
    
    this->schedule(schedule_selector(TurnuvaScene::closeWarningSchedule), 3);
    
}


void TurnuvaScene::closeWarningSchedule(float dt){
    
    buttonTouch = true;
    
    this->unschedule(schedule_selector(TurnuvaScene::closeWarningSchedule));
    
    this->removeChild(errorPanel);
    
}

void TurnuvaScene::openTurnuvaScene(double limitGold, double limitLevel, std::string turnuvaName){
    
    
    
    //actofscene 2 yap, node u kaldır ,sahneyi ekle play buttonda aşağıdaki komutları çağır
    
    actOfScene = 2;
    
    this->removeChild(itemNode);
    
    this->removeChild(top);
    
    ligTableNode = Node::create();
    
    ligTableNode->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(ligTableNode);
    
    ligTable = Sprite::create("ligsiralama.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(ligTable,HEIGHT*.8);
            
        }
        else {
            
            FIT_H(ligTable, HEIGHT*.9);
            
        }
        
    }
    else {
    
        FIT_H(ligTable, HEIGHT*.9);
        
    }
    
    std::string ligName;
    if(appManager->currentLanguage() == false){
    
    if(appManager->turnuvaName == "T1"){
        
        ligName = "1.LİG";
        
    }
    else if(appManager->turnuvaName == "T2"){
        
        ligName = "2.LİG";
        
    }
    else if(appManager->turnuvaName == "T3"){
        
        ligName = "3.LİG";
        
    }
    else if(appManager->turnuvaName == "T4"){
        
        ligName = "4.LİG";
        
    }
    else if(appManager->turnuvaName == "T5"){
        
        ligName = "Swartag";
        
    }
    }else {
        
        if(appManager->turnuvaName == "T1"){
            
            ligName = "LEAGUE 1";
            
        }
        else if(appManager->turnuvaName == "T2"){
            
            ligName = "LEAGUE 2";
            
        }
        else if(appManager->turnuvaName == "T3"){
            
            ligName = "LEAGUE 3";
            
        }
        else if(appManager->turnuvaName == "T4"){
            
            ligName = "LEAGUE 4";
            
        }
        else if(appManager->turnuvaName == "T5"){
            
            ligName = "SWARTAG";
            
        }
        
    }

    ligTable->setPosition(Vec2(WIDTH*.25, -HEIGHT*.05));
    
    ligTableNode->addChild(ligTable);
    
    Label *ligNameLabel = Label::createWithBMFont("menuFont.fnt", ligName);
    
    FIT_H(ligNameLabel, HEIGHT*.03);
    
    ligNameLabel->setPosition(Vec2(WIDTH*.135, HEIGHT*.335));
    
    ligTableNode->addChild(ligNameLabel);
    
    userStatisticTable = Sprite::create("kisibilgisi.png");
    
    userStatisticTable->setPosition(Vec2(-WIDTH*.25,HEIGHT*.05));
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(userStatisticTable, HEIGHT*.45);
            
        }
        else {
            
            FIT_H(userStatisticTable, HEIGHT*.6);
            
        }

        
    }
    else {
    
        FIT_H(userStatisticTable, HEIGHT*.6);
    
    }
        
    ligTableNode->addChild(userStatisticTable);
    
    
    std::string avatarNum = "avatar " + StringUtils::format("%.0f", (appManager->avatarNumber)) + ".png";

    
    currentAvatar = Sprite::create(avatarNum);
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(currentAvatar, HEIGHT*.2);
            
        }
        else {
            
            FIT_H(currentAvatar, HEIGHT*.25);
            
        }

        
    }
    else {
    
        FIT_H(currentAvatar, userStatisticTable->getContentSize().height*.25);
        
    }
    
    currentAvatar->setPosition(Vec2(userStatisticTable->getContentSize().width*.5, userStatisticTable->getContentSize().height*.75));
    
    userStatisticTable->addChild(currentAvatar);
    
    
    //user statistics labels
    
    Label *usernameLabel = Label::createWithTTF(appManager->username.c_str(), "fonts/MuseoSans.ttf", 40);
    
    usernameLabel->setPosition(Vec2(userStatisticTable->getContentSize().width/2, userStatisticTable->getContentSize().height*.5));
    
    userStatisticTable->addChild(usernameLabel);
    
    Label *playedGameLabel = Label::createWithTTF("O", "fonts/MuseoSans.ttf", 50);
    
    playedGameLabel->setPosition(Vec2(userStatisticTable->getContentSize().width*.3, userStatisticTable->getContentSize().height*.37));
    
    //userStatisticTable->addChild(playedGameLabel);
    
    //played count
    
    Label *playedCountLabel = Label::createWithTTF(StringUtils::format("%.0f",appManager->userGames), "fonts/MuseoSans.ttf", 50);
    
    playedCountLabel->setPosition(Vec2(userStatisticTable->getContentSize().width*.3, userStatisticTable->getContentSize().height*.22));
    
    userStatisticTable->addChild(playedCountLabel);
    
    
    Label *lostGameLabel = Label::createWithTTF("M", "fonts/MuseoSans.ttf", 50);
    
    lostGameLabel->setPosition(Vec2(userStatisticTable->getContentSize().width*.44, userStatisticTable->getContentSize().height*.37));
    
    //userStatisticTable->addChild(lostGameLabel);
    
    //lost count
    
    Label *lostCountLabel = Label::createWithTTF(StringUtils::format("%.0f",appManager->userLost), "fonts/MuseoSans.ttf", 50);
    
    lostCountLabel->setPosition(Vec2(userStatisticTable->getContentSize().width*.44, userStatisticTable->getContentSize().height*.22));
    
    userStatisticTable->addChild(lostCountLabel);
    
    
    Label *winGameLabel = Label::createWithTTF("G", "fonts/MuseoSans.ttf", 50);
    
    winGameLabel->setPosition(Vec2(userStatisticTable->getContentSize().width*.56, userStatisticTable->getContentSize().height*.37));
    
    //userStatisticTable->addChild(winGameLabel);
    
    //won count
    
    Label *wonCountLabel = Label::createWithTTF(StringUtils::format("%.0f",appManager->userWon), "fonts/MuseoSans.ttf", 50);
    
    wonCountLabel->setPosition(Vec2(userStatisticTable->getContentSize().width*.56, userStatisticTable->getContentSize().height*.22));
    
    userStatisticTable->addChild(wonCountLabel);
    
    
    Label *pointLabel = Label::createWithTTF("Puan", "fonts/MuseoSans.ttf", 50);
    
    pointLabel->setPosition(Vec2(userStatisticTable->getContentSize().width*.71, userStatisticTable->getContentSize().height*.37));
    
    //userStatisticTable->addChild(pointLabel);
    
    
    //point count
    
    Label *pointCountLabel = Label::createWithTTF(StringUtils::format("%.0f",appManager->userPoint), "fonts/MuseoSans.ttf", 50);
    
    pointCountLabel->setPosition(Vec2(userStatisticTable->getContentSize().width*.71, userStatisticTable->getContentSize().height*.22));
    
    userStatisticTable->addChild(pointCountLabel);
    
    
    //play button
    
    playButton = Sprite::create("oyna.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(playButton, HEIGHT*.15);
            
            playButton->setPosition(Vec2(WIDTH*.25, HEIGHT*.18));
            
        }
        else {
            
            FIT_H(playButton, HEIGHT*.2);
            
            playButton->setPosition(Vec2(WIDTH*.25, HEIGHT*.15));
            
        }
        
        
    }
    else {
    
        FIT_H(playButton, HEIGHT*.2);
        
        playButton->setPosition(Vec2(WIDTH*.25, HEIGHT*.15));
        
    }
    
    
    
    this->addChild(playButton);
    
    
    
    Label *oynaButtonlabel;
    
    if(appManager->currentLanguage() == false){
        
        oynaButtonlabel = Label::createWithTTF("Oyna", "chinese rocks rg.ttf", 70);
        
    }
    else {
        
        oynaButtonlabel = Label::createWithTTF("Play", "chinese rocks rg.ttf", 70);
        
    }
    
    oynaButtonlabel->setPosition(Vec2(playButton->getContentSize().width*.5, playButton->getContentSize().height*.5));
    
    playButton->addChild(oynaButtonlabel);
    
    
    //puan tablosu çek
    
    
    this->getTopRankers(10, turnuvaName);
    
    
}

void TurnuvaScene::getTopRankers(int max, std::string gameName){
    
    const char *gameRefName = gameName.c_str();
    
    App42API::Initialize(APPWARP_APP_KEY, APPWARP_SECRET_KEY);

    ScoreBoardService *scoreBoardService = App42API::BuildScoreBoardService();
    
    scoreBoardService->GetTopNRankers(gameRefName, max, app42callback(TurnuvaScene::onScoreboardRequestCompleted, this));
    
    
}

void TurnuvaScene::onScoreboardRequestCompleted(void *response){
    
    App42GameResponse *scoreResponse = (App42GameResponse*)response;
    printf("\ncode=%d",scoreResponse->getCode());
    printf("\nResponse Body=%s",scoreResponse->getBody().c_str());
    
    int count = 1;
    
    if (scoreResponse->isSuccess)
    {
        for(std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
        {
            printf("\n CreatedAt=%s",it->getCreatedOn().c_str());
            printf("\n ScoreId=%s\n",it->getScoreId().c_str());
            printf("\n ScoreValue=%f\n",it->getScoreValue());
            printf("\n UserName=%s\n",it->getUserName().c_str());
            
            std::string username = it->getUserName().c_str();
            
            username[0] = toupper(username[0]);
            
            Label *usernameLabel = Label::createWithTTF(StringUtils::format("%s", username.c_str()), "fonts/MuseoSans.ttf", 42);
        
            
            usernameLabel->setAnchorPoint(Vec2(0,0.5));
            
            usernameLabel->setPosition(Vec2(ligTable->getContentSize().width*.2, ligTable->getContentSize().height*.91 - (ligTable->getContentSize().height*.08)*count));
            
            usernameLabel->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
            
            ligTable->addChild(usernameLabel);
            
            
            Label *pointLabel = Label::createWithTTF(StringUtils::format("%.0f",it->getScoreValue()), "fonts/MuseoSans.ttf", 42);
            
            pointLabel->setAnchorPoint(Vec2(0,0.5));
            
            pointLabel->setPosition(Vec2(ligTable->getContentSize().width*.77, ligTable->getContentSize().height*.91 - (ligTable->getContentSize().height*.08)*count));
            
            pointLabel->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
            
            ligTable->addChild(pointLabel);
            
            count ++;
            
            
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

void TurnuvaScene::callQuitKeyAction() {
    
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
        
        quitWarnStr = "TURNUVADAN CIKMAK ISTIYOR MUSUNUZ?";
        
        yesStr = "EVET";
        
        noStr = "HAYIR";
        
    }
    else {
        
        quitWarnStr = "ARE YOU SURE TO LEAVE?";
        
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
                
                if(appManager->fx == true){
                    
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                    
                }
                
                //this->pause();
                
                
                    auto scene = MenuScene::createScene();
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
             
                
            
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


void TurnuvaScene::removeTurnuvaScene(){
    
    this->removeChild(ligTableNode);
    
    this->removeChild(playButton);
    
    actOfScene = 1;
    
}


bool TurnuvaScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(isAvatarPanelOpened == false){
        
        if(Avatar->getBoundingBox().containsPoint(touch->getLocation())){
            
            //Avatar->setOpacity(128);
            
        }
        
        if(actOfScene == 1){
            
            if(buttonTouch == true){
                
                initialTouchPos = touch->getLocation().x;
                currentTouchPos = touch->getLocation().x;
                
                for(int i= 0; i < ligButtonsVector.size(); i++){
                    
                    Point nodePoint = itemNode->convertToNodeSpaceAR(touch->getLocation());
                    
                    if(ligButtonsVector.at(i)->getBoundingBox().containsPoint(nodePoint)){
                        
                        initialTouchPos = touch->getLocation().x;
                        currentTouchPos = touch->getLocation().x;
                        
                        touchedLigId = i;
                        
                    }
                    
                }
                
            }
            
        }
        else if(actOfScene == 2){
            
            if(playButton->getBoundingBox().containsPoint(touch->getLocation())){
                
                playButton->setOpacity(128);
                
            }
            
        }
        
        if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(128);
            
        }
        else if(Ayarlar->getBoundingBox().containsPoint(touch->getLocation())){
            
            Ayarlar->setOpacity(128);
            
        }
        else if(arti->getBoundingBox().containsPoint(touch->getLocation())){
            
            arti->setOpacity(128);
            
        }


    }
    else if(isAvatarPanelOpened == true){
        
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
    
    
    
    return true;
}

void TurnuvaScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    
    
    if(isAvatarPanelOpened == false){
        
        if(!Avatar->getBoundingBox().containsPoint(touch->getLocation())){
            
            //Avatar->setOpacity(255);
            
        }
        
        if(actOfScene == 1){
            
            if(buttonTouch == true){
                
                currentTouchPos = touch->getLocation().x;
            
                speed = currentTouchPos - initialTouchPos;
                
                for(int i= 0; i < ligButtonsVector.size(); i++){
                    
                    Point nodePoint = itemNode->convertToNodeSpaceAR(touch->getLocation());
                    
                        if(ligButtonsVector.at(i)->getBoundingBox().containsPoint(nodePoint)){
                        
                            currentTouchPos  = touch->getLocation().x;
                        
                            speed = currentTouchPos - initialTouchPos;
                        
                        }
                    
                    }
             
            }
        }
        else if(actOfScene == 2){
            
            if(!playButton->getBoundingBox().containsPoint(touch->getLocation())){
                
                playButton->setOpacity(255);
                
            }
            
        }
        
        if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            backButton->setOpacity(255);
            
        }
        else if(!Ayarlar->getBoundingBox().containsPoint(touch->getLocation())){
            
            Ayarlar->setOpacity(255);
            
        }
        else if(!arti->getBoundingBox().containsPoint(touch->getLocation())){
            
            arti->setOpacity(255);
            
        }
        

        
    }
    else if(isAvatarPanelOpened == true){
        
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

}

void TurnuvaScene::openPricePanel(double limitGoldLig, double lig) {
    
    buttonTouch = false;
    
    duyuruPanel = Node::create();
    
    duyuruPanel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    this->addChild(duyuruPanel);
    
    Sprite *panel = Sprite::create("uyaripanel.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_WH(panel, WIDTH*.8 ,HEIGHT*.8);
            
        }
        else {
            
            FIT_WH(panel,WIDTH*.8 ,HEIGHT*.8);
            
        }
        
        
    }
    else {
        
        FIT_WH(panel,WIDTH*.8 ,HEIGHT*.8);
        
    }
    
    duyuruPanel->addChild(panel);
    
    panel->setPosition(Vec2(0,0));
    
    //giriş ücreti label
    
    Label *girisUcretiLabel, *levelUcretLabel, *warnLabel;
    
    Sprite *altins = Sprite::create("Altin.png");
    
    FIT_H(altins, HEIGHT*.09);
    
    duyuruPanel->addChild(altins);
    
    std::string ucretStr, warnStr, priceStr;
    
    if(appManager->currentLanguage() == false){
    
        ucretStr.assign("Giriş Ücreti :");
        
        warnStr.assign(StringUtils::format("%.0f.LİGDE MAÇ YAPMAK\nİSTEDİĞİNİZE EMİN MİSİNİZ?", lig));
        
        priceStr.assign(StringUtils::format("%.0f",limitGoldLig));
        
    }
    else {
        
        ucretStr.assign("Entrance Price :");
        
        warnStr.assign(StringUtils::format("ARE YOU SURE TO PLAY AT LEAGUE %.0f ?",lig));
        
        priceStr.assign(StringUtils::format("%.0f",limitGoldLig));
        
    }
    
    girisUcretiLabel = Label::createWithTTF(ucretStr, "CostaRica.ttf", HEIGHT*.08);
    
    levelUcretLabel = Label::createWithTTF(priceStr, "CostaRica.ttf", HEIGHT*.08);
    
    warnLabel = Label::createWithTTF(warnStr, "chinese rocks rg.ttf", HEIGHT*.07);
    
    duyuruPanel->addChild(girisUcretiLabel);
    duyuruPanel->addChild(levelUcretLabel);
    duyuruPanel->addChild(warnLabel);
    
    girisUcretiLabel->setPosition(Vec2(-panel->getBoundingBox().size.width*.15, panel->getBoundingBox().size.height*.3));
    levelUcretLabel->setPosition(Vec2(panel->getBoundingBox().size.width*.21, panel->getBoundingBox().size.height*.3));
    warnLabel->setPosition(Vec2(0, panel->getBoundingBox().size.height*.08));
    altins->setPosition(Vec2(panel->getBoundingBox().size.width*.11, panel->getBoundingBox().size.height*.3));
    
    girisUcretiLabel->setTextColor(Color4B(239, 202, 44, 255));
    levelUcretLabel->setTextColor(Color4B(239, 202, 44, 255));
    
    
    //buttons
    
    auto tamamBut = cocos2d::ui::Button::create("tamamyeni.png");
    
    auto hayirBut = cocos2d::ui::Button::create("tamamyeni.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad()){
            
            FIT_H(tamamBut, HEIGHT*.1);
            
            FIT_H(hayirBut, HEIGHT*.1);
            
        }
        else {
            
            FIT_H(tamamBut, HEIGHT*.15);
            
            FIT_H(hayirBut, HEIGHT*.15);
            
        }
        
        
    }
    else {
        
        FIT_H(tamamBut, HEIGHT*.15);
        
        FIT_H(hayirBut, HEIGHT*.15);
        
    }
    
    
    tamamBut->setPosition(Vec2(-panel->getBoundingBox().size.width*.2, -panel->getBoundingBox().size.height*.25));
    
    hayirBut->setPosition(Vec2(panel->getBoundingBox().size.width*.2, -panel->getBoundingBox().size.height*.25));
    
    duyuruPanel->addChild(tamamBut);

    duyuruPanel->addChild(hayirBut);
    
    Label *tamamLabel, *hayirLabel;
    
    if(appManager->currentLanguage() == false){
        
        tamamLabel = Label::createWithTTF("EVET", "chinese rocks rg.ttf", 50);
        
        hayirLabel = Label::createWithTTF("HAYIR", "chinese rocks rg.ttf", 50);
        
    }
    else {
        
        tamamLabel = Label::createWithTTF("YES", "chinese rocks rg.ttf", 50);
        
        hayirLabel = Label::createWithTTF("NO", "chinese rocks rg.ttf", 50);
        
    }
    
    tamamLabel->setPosition(Vec2(tamamBut->getContentSize().width/2, tamamBut->getContentSize().height/2));
    
    hayirLabel->setPosition(Vec2(hayirBut->getContentSize().width/2, hayirBut->getContentSize().height/2));
    
    tamamBut->addChild(tamamLabel);
    
    hayirBut->addChild(hayirLabel);
    
    tamamBut->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
        
        switch (type) {
                
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                break;
                
                
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                
                //başarılı olursa
                
                buttonTouch = true;
                
                this->removeChild(duyuruPanel);
                
                openTurnuvaScene(limitGold, limitLevel, appManager->turnuvaName);
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
                
        }
        
        
    });
    

    hayirBut->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type){
        
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                
                buttonTouch = true;
                
                this->removeChild(duyuruPanel);
                
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
        }
        
        
    });
    
    
    
    
    
    
    
}


void TurnuvaScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    
    
    if(isAvatarPanelOpened == false){
        
        if(Avatar->getOpacity() == 128){
            
            if(appManager->fx == true){
            
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            Avatar->setOpacity(255);
            
            //this->openAvatarPanel();
            
        }
        
        if(actOfScene == 1){
            
            
            if(buttonTouch == true){
                
                for(int i= 0; i < ligButtonsVector.size(); i++){
                    
                    Point nodePoint = itemNode->convertToNodeSpaceAR(touch->getLocation());
                    
                    if(!ligButtonsVector.at(i)->getBoundingBox().containsPoint(nodePoint)){
                        
                        currentTouchPos = touch->getLocation().x;
                        initialTouchPos = touch->getLocation().x;
                        
                    }
                    else if(speed != 0){
                        
                        touchedLigId = -1;
                        
                    }
                    else if(ligButtonsVector.at(i)->getBoundingBox().containsPoint(nodePoint) && speed == 0){
                        
                        
                        switch (i) {
                                
                            case 0:
                                
                                if(appManager->fx == true){
                                
                                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                                    
                                }
                                
                                lig = 1;
                                
                                appManager->goldForRoom = "100";
                                appManager->levelForRoom = "0";
                                appManager->turnuvaName = "T1";
                                
                                limitGold = 100;
                                limitLevel = 0;
                                
                                break;
                                
                            case 1:
                                
                                if(appManager->fx == true){
                                
                                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                                    
                                }
                                
                                lig = 2;
                                
                                appManager->goldForRoom = "200";
                                appManager->levelForRoom = "3";
                                appManager->turnuvaName = "T2";
                                
                                limitGold = 200;
                                limitLevel = 3;
                                
                                break;
                                
                            case 2:
                                
                                if(appManager->fx == true){
                                
                                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                                    
                                }
                                
                                lig = 3;
                                
                                appManager->goldForRoom = "400";
                                appManager->levelForRoom = "5";
                                appManager->turnuvaName = "T3";
                                
                                limitGold = 400;
                                limitLevel = 5;
                                
                                break;
                                
                            case 3:
                                
                                if(appManager->fx == true){
                                
                                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                                    
                                }
                                
                                lig = 4;
                                
                                appManager->goldForRoom = "1000";
                                appManager->levelForRoom = "10";
                                appManager->turnuvaName = "T4";
                                
                                limitGold = 1000;
                                limitLevel = 10;
                                
                                break;
                                
                            case 4:
                                
                                if(appManager->fx == true){
                                
                                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                                    
                                }
                                
                                lig = 5;
                                
                                appManager->goldForRoom = "2000";
                                appManager->levelForRoom = "20";
                                appManager->turnuvaName = "T5";
                                
                                limitGold = 2000;
                                limitLevel = 20;
                                
                                break;
                                
                        }
                        
                        //level ve gold kontrolü yapılacak...
                        
                        
                        
                        if(appManager->level >= limitLevel && appManager->gold >= limitGold){
                            
                            openPricePanel(limitGold, lig);
                            
                        }
                        else {
                            
                            if(appManager->level < limitLevel && appManager->gold < limitGold){
                                
                                if(appManager->currentLanguage() == false){
                                
                                    this->openWarningPanel("Altın ve Seviye Yetersiz");
                                    
                                }
                                else {
                                    
                                    this->openWarningPanel("Gold and Level are not enough");
                                    
                                }
                                
                            }
                            else if(appManager->level < limitLevel){
                                
                                if(appManager->currentLanguage() == false){
                                
                                    this->openWarningPanel("Seviye Yetersiz");
                                    
                                }
                                else{
                                    
                                    this->openWarningPanel("Not enough level");
                                    
                                }
                                
                            }
                            else if(appManager->gold < limitGold){
                                
                                if(appManager->currentLanguage() == false){
                                
                                    this->openWarningPanel("Altın Yetersiz");
                                    
                                }
                                else{
                                    
                                    this->openWarningPanel("Not enough gold");
                                    
                                }
                                
                            }
                            
                        }
                        
                        break;
                        
                    }
                    
                }
                
                if(speed > WIDTH*.1){
                    
                    speed = 0;
                    
                    if(itemNode->getPositionX() < 0){
                    
                    
                        auto moveByAnim = MoveBy::create(.2, Vec2(distanceOfItems, itemNode->getPositionY()));
                    
                        auto cubicEaseOut = EaseCubicActionOut::create(moveByAnim);
                    
                        itemNode->runAction(cubicEaseOut);
                        
                    }
                    
                    
                }
                else if(speed < -WIDTH*.1){
                    
                    
                    speed = 0;
                    
                    if(itemNode->getPositionX() > -WIDTH*2){
                        
                    
                        auto moveByAnim = MoveBy::create(.2, Vec2(-distanceOfItems,itemNode->getPositionY()));
                    
                        auto cubicEaseOut = EaseCubicActionOut::create(moveByAnim);
                    
                        itemNode->runAction(cubicEaseOut);
                        
                    }
                    
                    
                    
                }
                
            }
            
        }
        else if(actOfScene == 2){
            
            
            if(playButton->getOpacity() == 128){
                
                if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                    
                }
                
                playButton->setOpacity(255);
                
                if(appManager->gold >= limitGold && appManager->level >= limitLevel){
                    
                    appManager->limitGold = limitGold;
                    
                    this->updateUserInformation(appManager->username.c_str(), (appManager->gold - limitGold), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
                    
                }
                else {
                    
                    //yetersiz level uyarısı
                    
                    if(appManager->gold < limitGold){
                        
                        if(appManager->currentLanguage() == false){
                        
                            this->openWarningPanel("Yeterli Altınınız Yok");
                            
                        }
                        else {
                            
                            this->openWarningPanel("Not enough gold");
                            
                        }
                        
                    }
                    else if(appManager->level < limitLevel){
                        
                        if(appManager->currentLanguage() == false){
                        
                            this->openWarningPanel("Level Seviyesi Düşük");
                        
                        }
                        else {
                        
                            this->openWarningPanel("Not enough level");
                            
                        }
                        
                    }
                    
                    
                }
                
            }
            
            
        }
        
        
        if(backButton->getOpacity() == 128){
            
            if(appManager->fx == true){
            
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
                
            }
            
            if(actOfScene == 1){
                
                
                backButton->setOpacity(255);
                
                auto scene = MenuScene::createScene();
                
                Director::getInstance()->replaceScene(TransitionCrossFade::create(.5, scene));
                
            }
            else if(actOfScene == 2){
                
                backButton->setOpacity(255);
                
                this->removeTurnuvaScene();
                
                this->createItems();
                
            }
            
        }
        else if(Ayarlar->getOpacity() == 128){
            
            //ayarlar popup
            
        }
        else if(arti->getOpacity() == 128){
            
            auto scene = MagazaScene::createScene();
            Director::getInstance()->replaceScene(TransitionFlipAngular::create(.5, scene, cocos2d::TransitionScene::Orientation::RIGHT_OVER));
            
        }

        
    }
    else if(isAvatarPanelOpened == true){
            
            
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
        
    
 
        
        
}

