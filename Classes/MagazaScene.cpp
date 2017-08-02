//
//  MagazaScene.cpp
//  Santra
//
//  Created by Anıl Gülgör on 08/12/15.
//
//

#include "MagazaScene.hpp"


Scene* MagazaScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MagazaScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool MagazaScene::init() {
    
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
    
    
    AppWarp::Client *warpClientRef;
    
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);

    
    
#ifdef SDKBOX_ENABLED
    
    IAP::setDebug(true);
    IAP::setListener(this);
    IAP::refresh();
    
    sdkbox::PluginVungle::setListener(this);
    
    
#endif
    
    WIDTH = GET_WIDTH;
    HEIGHT = GET_HEIGHT;
    
    dist = 0;
    speed = 0;
    accelerate = WIDTH*.08;
    
    canSlide = true;
    
    background = Sprite::create("magazabackground.png");
    FIT_WH(background, WIDTH, HEIGHT);
    background->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(background);
    
    redBar = Sprite::create("magazaRedBar.png");
    FIT_H(redBar, HEIGHT*2.5);
    redBar->setPosition(Vec2(0,HEIGHT*.5));
    this->addChild(redBar);
    redBar->setZOrder(2);
    
    
    createButtons();
    createItems();
    
    Label *magazaLabel;
    
    if(appManager->currentLanguage() == false){
        
        magazaLabel = Label::createWithTTF("ALTIN", "chinese rocks rg.ttf", 75);
        
    }
    else {
        
        magazaLabel = Label::createWithTTF("GOLD", "chinese rocks rg.ttf", 75);
        
    }

    magazaLabel->setPosition(Vec2(WIDTH*.175, HEIGHT*.93));
    
    this->addChild(magazaLabel);
    
    
    //default altın mağazası
    itemNode = Node::create();
    itemNode->setAnchorPoint(Vec2(0,0));
    itemNode->setPosition(Vec2(0,0));
    this->addChild(itemNode);
    
    
    sliderVec = altinVec;
    addItemsToScreen(sliderVec.size());
    
    return true;
    
}
void MagazaScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event){
    
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

void MagazaScene::callQuitKeyAction() {
    
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
        
        quitWarnStr = "MAGAZADAN CIKMAK ISTIYOR MUSUNUZ?";
        
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


void MagazaScene::update(float dt){
    
    /*speed *= .8;
    
    if(speed > WIDTH*.07){
        
        speed = WIDTH*.07;
        
    }
    else if(speed < -WIDTH*.07){
        
        speed = -WIDTH*.07;
        
    }
    
    if(itemNode->getPositionX() <= WIDTH*.3 && itemNode->getPositionX() >= -WIDTH*.6){
        
        itemNode->setPositionX(itemNode->getPositionX() + speed*.99);
        
    }
    
    if(itemNode->getPositionX() > WIDTH*.3){
        
        itemNode->setPositionX(WIDTH*.3);
        
    }
    else if(itemNode->getPositionX() < -WIDTH*.6){
        
        itemNode->setPositionX(-WIDTH*.6);
        
    }
    
 */
    
}

void MagazaScene::onEnterTransitionDidFinish() {
    
    Layer::onEnterTransitionDidFinish();
    
    this->schedule(schedule_selector(MagazaScene::update));
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MagazaScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MagazaScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MagazaScene::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
}

void MagazaScene::createButtons(){
    
    altin = Sprite::create("AltinM.png");
    FIT_H(altin, HEIGHT*.145);
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            altin->setPosition(Vec2(WIDTH*.055,HEIGHT*.9));
            
            
        }
        else {
            
            altin->setPosition(Vec2(WIDTH*.045,HEIGHT*.9));
            
        }
        
    }
    else {
    
        altin->setPosition(Vec2(WIDTH*.045,HEIGHT*.9));
        
    }
    this->addChild(altin);
    altin->setZOrder(2);
    
    /*krampon = Sprite::create("Krampon.png");
    FIT_H(krampon, HEIGHT*.145);
    krampon->setPosition(Vec2(WIDTH*.045,HEIGHT*.68));
    this->addChild(krampon);
    krampon->setZOrder(2);
    
    kart = Sprite::create("Kart.png");
    FIT_H(kart, HEIGHT*.145);
    kart->setPosition(Vec2(WIDTH*.045,HEIGHT*.51));
    this->addChild(kart);
    kart->setZOrder(2);*/
    
    video = Sprite::create("video.png");
    FIT_H(video, HEIGHT*.145);
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            video->setPosition(Vec2(WIDTH*.055,HEIGHT*.73));
            
            
        }
        else {
            
            video->setPosition(Vec2(WIDTH*.045,HEIGHT*.73));
            
        }
        
    }
    else {
        
        video->setPosition(Vec2(WIDTH*.045,HEIGHT*.73));
        
    }
    this->addChild(video);
    video->setZOrder(2);
    
    backButton = Sprite::create("backbutonyeni.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(backButton, HEIGHT*.13);
            
            backButton->setPosition(Vec2(WIDTH*.055, HEIGHT*.1));
            
        }
        else {
            
            FIT_H(backButton, HEIGHT*.145);
            backButton->setPosition(Vec2(WIDTH*.045, HEIGHT*.1));
            
        }
        
    }
    else {
        
        FIT_H(backButton, HEIGHT*.145);
        backButton->setPosition(Vec2(WIDTH*.045, HEIGHT*.1));
        
    }
    this->addChild(backButton);
    backButton->setZOrder(2);
    
    
    removeAdsButton = Sprite::create("reklamkaldir.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(removeAdsButton, HEIGHT*.12);
            removeAdsButton->setPosition(Vec2(WIDTH*.75, HEIGHT*.92));

            
        }
        else {
            
            FIT_H(removeAdsButton, HEIGHT*.15);
            removeAdsButton->setPosition(Vec2(WIDTH*.75, HEIGHT*.92));
            
            
        }

        
    }
    else {
    
        FIT_H(removeAdsButton, HEIGHT*.15);
        removeAdsButton->setPosition(Vec2(WIDTH*.75, HEIGHT*.92));

        
    }
    
    this->addChild(removeAdsButton);
    
    Label *removeAdsLabel;
    
    if(appManager->currentLanguage() == false){
        
        removeAdsLabel = Label::createWithTTF("Reklam\nKaldır", "chinese rocks rg.ttf", 30);
        
    }
    else {
        
        removeAdsLabel = Label::createWithTTF("Remove\nAds", "chinese rocks rg.ttf", 30);
        
    }
    
    removeAdsLabel->setPosition(Vec2(removeAdsButton->getContentSize().width*.6, removeAdsButton->getContentSize().height*.5));
    removeAdsButton->addChild(removeAdsLabel);
    
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        restoreButton = Sprite::create("tamamyeni.png");
        
        if(appManager->isIpad() == true){
            
            FIT_H(restoreButton, HEIGHT*.12);
            restoreButton->setPosition(Vec2(WIDTH*.55, HEIGHT*.92));
            
            
        }
        else {
            
            FIT_H(restoreButton, HEIGHT*.15);
            restoreButton->setPosition(Vec2(WIDTH*.55, HEIGHT*.92));
            
            
        }
        
        Label *restoreLabel;
        
        if(appManager->currentLanguage() == false){
            
            restoreLabel = Label::createWithTTF("RESTORE", "chinese rocks rg.ttf", 40);
            
        }
        else {
            
            restoreLabel = Label::createWithTTF("RESTORE", "chinese rocks rg.ttf", 40);
            
        }

        restoreLabel->setPosition(Vec2(restoreButton->getContentSize().width*.5, restoreButton->getContentSize().height*.5));
        restoreButton->addChild(restoreLabel);
        
        this->addChild(restoreButton);
        
    }
   

    
    
    //sağ taraf
    auto sagUst = Sprite::create("SagUst.png");
    FIT_WH(sagUst,WIDTH *.14 ,HEIGHT*.113);
    sagUst->setAnchorPoint(Vec2(1,.5));
    sagUst->setPosition(Vec2(WIDTH,HEIGHT*.917));
    this->addChild(sagUst);
    
    
    auto altinpng = Sprite::create("Altin.png");
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            
            FIT_H(altinpng, HEIGHT*.065);
            
            
            altinpng->setPosition(Vec2(WIDTH*.88,HEIGHT*.92));
            
        }
        else {
            
            FIT_H(altinpng, HEIGHT*.075);
            
            
            altinpng->setPosition(Vec2(WIDTH*.9,HEIGHT*.92));
            
        }
        
    }
    else {
        
        FIT_H(altinpng, HEIGHT*.075);
        
        
        altinpng->setPosition(Vec2(WIDTH*.9,HEIGHT*.92));
        
    }
    
    this->addChild(altinpng);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
    
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

    
    
}

void MagazaScene::createItems(){
    
    for(int i=0; i<4; i++){
        
        cocos2d::String *altinPaket = cocos2d::String::createWithFormat("AltinPaket%d.png",i+1);
        Sprite *altinMagaza = Sprite::create(altinPaket->getCString());
        altinMagaza->setAnchorPoint(Vec2(0.5,0.5));
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                
                FIT_H(altinMagaza, HEIGHT*.45);
                
            }
            else {
                
                FIT_H(altinMagaza, HEIGHT*.6);
                
            }
            
        }
        else {
        
            FIT_H(altinMagaza, HEIGHT*.6);
            
        }
        altinMagaza->setPosition(Vec2(WIDTH*1.6 + (i*WIDTH*.3),HEIGHT*.5));
        
        int count;
        std::string price;
        
        switch (i) {
            case 0:
                count = 1000;
                if(appManager->currentLanguage() == false){
                    price = "5.29 TL";
                }
                else {
                    price = "1.99 $";
                }
                break;
                
            case 1:
                count = 5000;
                if(appManager->currentLanguage() == false){
                    price = "7.99 TL";
                }
                else {
                    price = "2.99 $";
                }
                break;
                
            case 2:
                count = 10000;
                if(appManager->currentLanguage() == false){
                    price = "10.49 TL";
                }
                else {
                    price = "3.99 $";
                }
                break;
                
            case 3:
                count = 50000;
                if(appManager->currentLanguage() == false){
                    price = "15.99 TL";
                }
                else {
                    price = "5.99 $";
                }
                break;
        }
        
        Label *altinCount;
        
        if(appManager->currentLanguage() == false){
            
            altinCount = Label::createWithTTF(StringUtils::format("%d ALTIN", count), "chinese rocks rg.ttf", 80);
            
        }
        else {
            
            altinCount = Label::createWithTTF(StringUtils::format("%d GOLDS", count), "chinese rocks rg.ttf", 80);
            
        }
        
        altinCount->setPosition(Vec2(altinMagaza->getContentSize().width*.5, altinMagaza->getContentSize().height*.2));
        
        altinMagaza->addChild(altinCount);
        
        
        Label *priceLabel = Label::createWithTTF(price, "chinese rocks rg.ttf", 100);
        
        priceLabel->setPosition(Vec2(altinMagaza->getContentSize().width*.5, -altinMagaza->getContentSize().height*.18));
        
        altinMagaza->addChild(priceLabel);
        
        
        altinVec.pushBack(altinMagaza);
        
        
        //this->addChild(altinMagaza);
        
    }
    
    /*for(int i=0; i<4; i++){
        
        cocos2d::String *kramponPaket = cocos2d::String::createWithFormat("KramponPaket%d.png",i+1);
        Sprite *kramponMagaza = Sprite::create(kramponPaket->getCString());
        kramponMagaza->setAnchorPoint(Vec2(0.5,0.5));
        FIT_H(kramponMagaza, HEIGHT*.6);
        kramponMagaza->setPosition(Vec2(WIDTH*1.4 + (i*WIDTH*.3),HEIGHT*.5));
        kramponVec.pushBack(kramponMagaza);
        //this->addChild(kramponMagaza);
        
    }
    
    for(int i= 0; i<4; i++){
        
        cocos2d::String *kartPaket = cocos2d::String::createWithFormat("KartPaket%d.png",i+1);
        Sprite *kartMagaza = Sprite::create(kartPaket->getCString());
        kartMagaza->setAnchorPoint(Vec2(0.5,0.5));
        FIT_H(kartMagaza, HEIGHT*.6);
        kartMagaza->setPosition(Vec2(WIDTH*1.4 + (i*WIDTH*.3),HEIGHT*.5));
        kartVec.pushBack(kartMagaza);
        
    }
    */
    
    camExp = CamExplosion::create(Vec2(WIDTH*.533, HEIGHT*.5));
    
    camExp->setPosition(camExp->_position);
    
    camExp->setUp();
    
    this->addChild(camExp);
    
    //camExp->startAnimation();
    
    camExp->setZOrder(15);
    
    
}

void MagazaScene::addItemsToScreen(ssize_t vecCount){
    
    itemNode = Node::create();
    itemNode->setAnchorPoint(Vec2(0,0));
    addChild(itemNode);
    
    for (int i=0 ; i<vecCount ; i++){
        
        sliderVec.at(i)->setPosition(Vec2(WIDTH*1.2 + (i*WIDTH*.3), HEIGHT*.5));
        auto anim = MoveTo::create(.5, Vec2(WIDTH*.53 + (i*WIDTH*.3), HEIGHT*.5));
        auto ease = EaseBackInOut::create(anim);
        sliderVec.at(i)->runAction(ease);
        itemNode->addChild(sliderVec.at(i));
        
        if(i == 1){
            
            distanceOfItems = (WIDTH*.3);
            
        }

        
    }
    

}

void MagazaScene::removeItemsFromScreen(ssize_t vecCount){
    
    for (int i=0 ; i<vecCount ; i++){
        
        //removeChild(sliderVec.at(i));
        
        auto anim = MoveTo::create(.5, Vec2(WIDTH*1.2 + (i*WIDTH*.3), HEIGHT*.5));
        auto ease = EaseBackInOut::create(anim);
        Sequence *seq = Sequence::create(ease,RemoveSelf::create(), NULL);
        sliderVec.at(i)->runAction(seq);

    }
    
    removeChild(itemNode);
    
}




bool MagazaScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(altin->getBoundingBox().containsPoint(touch->getLocation())){
        
        altin->setOpacity(128);
        
    }
    /*else if(krampon->getBoundingBox().containsPoint(touch->getLocation())){
        
        krampon->setOpacity(128);
        
    }
    else if(kart->getBoundingBox().containsPoint(touch->getLocation())){
        
        kart->setOpacity(128);
        
    }*/
    else if(video->getBoundingBox().containsPoint(touch->getLocation())){
        
        video->setOpacity(128);
        
    }
    else if(backButton->getBoundingBox().containsPoint(touch->getLocation())){
        
        backButton->setOpacity(128);
        
    }
    else if(removeAdsButton->getBoundingBox().containsPoint(touch->getLocation())){
        
        removeAdsButton->setOpacity(128);
        
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(restoreButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            restoreButton->setOpacity(128);
            
        }
        
    }
    
    if(canSlide == true){
    
    initialTouchPos = touch->getLocation().x;
    currentTouchPos = touch->getLocation().x;
    
    if(currentTouchPos > WIDTH*2){
    
        for(int i= 0; i < sliderVec.size(); i++){
            
            Point nodePoint = itemNode->convertToNodeSpaceAR(touch->getLocation());
            
            if(sliderVec.at(i)->getBoundingBox().containsPoint(nodePoint)){
                
                isTouchDown = true;
                initialTouchPos = touch->getLocation().x;
                currentTouchPos = touch->getLocation().x;
                
            }
            
        }

        
    }
        
    }
    
    
    return true;
    
}

void MagazaScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(!altin->getBoundingBox().containsPoint(touch->getLocation())){
        
        altin->setOpacity(255);
        
    }
    /*else if(!krampon->getBoundingBox().containsPoint(touch->getLocation())){
        
        krampon->setOpacity(255);
        
    }
    else if(!kart->getBoundingBox().containsPoint(touch->getLocation())){
        
        kart->setOpacity(255);
        
    }*/
    else if(!video->getBoundingBox().containsPoint(touch->getLocation())){
        
        video->setOpacity(255);
        
    }
    else if(!backButton->getBoundingBox().containsPoint(touch->getLocation())){
        
        backButton->setOpacity(255);
        
    }
    else if(!removeAdsButton->getBoundingBox().containsPoint(touch->getLocation())){
        
        removeAdsButton->setOpacity(255);
        
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(!restoreButton->getBoundingBox().containsPoint(touch->getLocation())){
            
            restoreButton->setOpacity(255);
            
        }
        
    }
    
    if(canSlide == true){
    
    currentTouchPos = touch->getLocation().x;
    
    speed = currentTouchPos - initialTouchPos;
    
    if(currentTouchPos > WIDTH*.1){
    
    for(int i= 0; i < sliderVec.size(); i++){
        
        Point nodePoint = itemNode->convertToNodeSpaceAR(touch->getLocation());
        
        if(sliderVec.at(i)->getBoundingBox().containsPoint(nodePoint)){
            
            isTouchDown = true;
            currentTouchPos  = touch->getLocation().x;
            
            speed = currentTouchPos - initialTouchPos;
            
        }
        
    }

    }
        
    }

    
}

void MagazaScene::openVideoRewardPanel() {
    
    canSlide = false;
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel->setZOrder(16);
    
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

    std::string warnStr;
    
    if(appManager->currentLanguage() == false){
        
        warnStr.assign("Video izle 25 Altın Kazan");
        
    }
    else {
        
        warnStr.assign("Watch ads to gain 25 golds");
        
    }
    
    Label *warnLabel = Label::createWithTTF(warnStr, "chinese rocks rg.ttf", 50);
    
    errorPanel->addChild(warnLabel);
    
    warnLabel->setPosition(Vec2(0, panel->getBoundingBox().size.height*.35));

    Sprite *videoSprite = Sprite::create("Videoizle.png");
    
    FIT_H(videoSprite, HEIGHT*.37);
    
    errorPanel->addChild(videoSprite);
    
    videoSprite->setPosition(Vec2(0,0));
    
    
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
    
    
    tamamBut->setPosition(Vec2(0, -panel->getBoundingBox().size.height*.32));
    
    
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
                
                //başarılı olursa
                
                //open video reward panel
                
                canSlide = true;
                
                this->removeChild(errorPanel);
                
                sdkbox::PluginVungle::show("reward");
                
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
                
        }
        
        
    });

    
    
}


void MagazaScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(altin->getOpacity() == 128){
        
        if(appManager->fx == true){
        
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
        }
        
        altin->setOpacity(255);
        removeItemsFromScreen(sliderVec.size());
        sliderVec = altinVec;
        addItemsToScreen(sliderVec.size());
        
    }/*
    else if(krampon->getOpacity() == 128){
        
        krampon->setOpacity(255);
        removeItemsFromScreen(sliderVec.size());
        sliderVec = kramponVec;
        addItemsToScreen(sliderVec.size());
        
    }
    else if(kart->getOpacity() == 128){
        
        kart->setOpacity(255);
        removeItemsFromScreen(sliderVec.size());
        sliderVec = kartVec;
        addItemsToScreen(sliderVec.size());
        s
    }*/
    else if(video->getOpacity() == 128){
        
        if(appManager->fx == true){
        
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
        }
        
        video->setOpacity(255);
        
        openVideoRewardPanel();
        
    }
    else if(backButton->getOpacity() == 128){
        
        if(appManager->fx == true){
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cikis.mp3");
            
        }
        
        backButton->setOpacity(255);
        
        //go back
        
        auto scene = MenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFlipX::create(.5, scene, cocos2d::TransitionScene::Orientation::RIGHT_OVER));
        
    }
    else if(removeAdsButton->getOpacity() == 128){
        
        removeAdsButton->setOpacity(255);
        
        if(appManager->fx == true){
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
            
        }
        
        auto scheduler = Director::getInstance()->getScheduler();
    
        scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MagazaScene::addLoader, this));
        
        sdkbox::IAP::purchase("removeads");
        
    }
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(restoreButton->getOpacity() == 128){
            
            restoreButton->setOpacity(255);
        
            if(appManager->fx == true){
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                
            }
            
            auto scheduler = Director::getInstance()->getScheduler();
            
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MagazaScene::addLoader, this));
            
            sdkbox::IAP::restore();
            
            
        }
        
        
    }
    
    if(canSlide == true){
    
 
    
    for(int i= 0; i < sliderVec.size(); i++){
        
        Point nodePoint = itemNode->convertToNodeSpaceAR(touch->getLocation());
        
        if(!sliderVec.at(i)->getBoundingBox().containsPoint(nodePoint)){
            
            isTouchDown = false;
            currentTouchPos = touch->getLocation().x;
            initialTouchPos = touch->getLocation().x;
            
        }
        else if(sliderVec.at(i)->getBoundingBox().containsPoint(nodePoint) && speed == 0){
            
            CCLOG("%d",i);
            
            auto scheduler = Director::getInstance()->getScheduler();

            switch (i) {
                    
                case 0:
                    
                    if(appManager->fx == true){
                    
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                        
                    }
                    
                    scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MagazaScene::addLoader,this));
                    
                    sdkbox::IAP::purchase("coinpackage");
                    
                    break;
                    
                case 1:
                    
                    if(appManager->fx == true){
                    
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                        
                    }
                    
                    scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MagazaScene::addLoader,this));
                    
                    sdkbox::IAP::purchase("coinpackage2");
                    
                    break;
                    
                case 2:
                    
                    if(appManager->fx == true){
                    
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                        
                    }
                    
                    scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MagazaScene::addLoader,this));
                    
                    sdkbox::IAP::purchase("coinpackage3");
                    
                    break;
                
                case 3:
                    
                    if(appManager->fx == true){
                    
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("butonses.mp3");
                        
                    }
                    
                    scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MagazaScene::addLoader,this));
                    
                    sdkbox::IAP::purchase("coinpackage4");
                    
                    break;
                    
                    
            }
            
        }
        
    }
    
    if(speed > WIDTH*.1){
        
        speed = 0;
        
        if(itemNode->getPositionX() < 0){
        
            auto moveByAnim = MoveBy::create(.07, Vec2(distanceOfItems, 0));
        
            auto cubicEaseOut = EaseCubicActionOut::create(moveByAnim);
        
            itemNode->runAction(cubicEaseOut);
            
        }
       
        
    }
    else if(speed < -WIDTH*.1){
        
        speed = 0;
        
        if(itemNode->getPositionX() > -0.8*WIDTH){
        
            auto moveByAnim = MoveBy::create(.07, Vec2(-distanceOfItems,0));
        
            auto cubicEaseOut = EaseCubicActionOut::create(moveByAnim);
        
            itemNode->runAction(cubicEaseOut);
            
        }
        
        
    }

}


    
}

void MagazaScene::addLoader() {
    
    canSlide = false;
    
    blackBg = Sprite::create("blackBG.png");
    FIT_WH(blackBg, WIDTH, HEIGHT);
    blackBg->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(blackBg,16);
    blackBg->setOpacity(255);
    
    loaderBack = Sprite::create("grassicon.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
              FIT_H(loaderBack, WIDTH*.25);
            
            
        }
        else {
            
            FIT_H(loaderBack, WIDTH*.4);
            
        }
        
    }
    else {
        
        FIT_H(loaderBack, WIDTH*.4);
        
    }
    
    loaderBack->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    this->addChild(loaderBack,16);
    loaderBack->setOpacity(200);
    
    loader = Sprite::create("topball.png");
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(loader, WIDTH*.09);
            
            
        }
        else {
            
            FIT_H(loader, WIDTH*.12);
            
        }
        
    }
    else {
    
        FIT_H(loader, WIDTH*.12);
        
    }
    loader->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    this->addChild(loader,16);
    
    
    auto anim = RotateBy::create(1.5, 160);
    auto repeat = RepeatForever::create(anim);
    
    loader->runAction(repeat);
    
    if(appManager->currentLanguage() == false){
        connectingLabel = Label::createWithTTF("UYGULANIYOR", "chinese rocks rg.ttf", HEIGHT* .055);
    }
    else {
        connectingLabel = Label::createWithTTF("APPLYING", "chinese rocks rg.ttf", HEIGHT* .055);
    }
    connectingLabel->setTextColor(Color4B(255, 255, 255, 220));
    connectingLabel->setPosition(Vec2(WIDTH*.5,HEIGHT*.2));
    this->addChild(connectingLabel,16);

    
    
}

void MagazaScene::removeLoader() {
    
    canSlide = true;
    
    this->removeChild(blackBg);
    
    this->removeChild(loaderBack);
    
    this->removeChild(loader);
    
    this->removeChild(connectingLabel);
    
}

//sdk box iap

void MagazaScene::updateIAP(const std::vector<sdkbox::Product>& products)
{
    
    _products = products;
    
    for (int i=0; i < products.size(); i++)
    {
        CCLOG("IAP: ========= IAP Item =========");
        CCLOG("IAP: Name: %s", products[i].name.c_str());
        CCLOG("IAP: ID: %s", products[i].id.c_str());
        CCLOG("IAP: Title: %s", products[i].title.c_str());
        CCLOG("IAP: Desc: %s", products[i].description.c_str());
        CCLOG("IAP: Price: %s", products[i].price.c_str());
        CCLOG("IAP: Price Value: %f", products[i].priceValue);
        
        auto item = MenuItemFont::create(products[i].name, CC_CALLBACK_1(MagazaScene::onIAP, this));
        item->setUserData(reinterpret_cast<void*>(i));
    
    }


}

void MagazaScene::onIAP(cocos2d::Ref *sender)
{


}

void MagazaScene::onRequestIAP(cocos2d::Ref* sender)
{
    IAP::refresh();
}

void MagazaScene::onRestoreIAP(cocos2d::Ref* sender)
{
    IAP::restore();
}


void MagazaScene::onInitialized(bool ok)
{
    CCLOG("%s : %d", __func__, ok);
}

void MagazaScene::onSuccess(const Product &p)
{
    if (p.name == "coinpackage") {
        
        CCLOG("Purchase complete: coin_package");
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 1000)));
        
        this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 1000), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon,appManager->userLost, appManager->userPoint);
        
        camExp->startAnimation();
        

    }
    else if (p.name == "coinpackage2") {
    
        CCLOG("Purchase complete: coin_package2");
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 5000)));
    
         this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 5000), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon,appManager->userLost, appManager->userPoint);
        
        camExp->startAnimation();
        
    }
    else if(p.name == "coinpackage3"){
        
        CCLOG("Purchase complete : coin_package3");
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 10000)));
        
         this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 10000), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon,appManager->userLost, appManager->userPoint);
        
        camExp->startAnimation();
        
    }
    else if(p.name == "coinpackage4"){
        
        CCLOG("Purchase complete : coin_package 4");
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 20000)));
        
         this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 20000), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon,appManager->userLost, appManager->userPoint);
        
        camExp->startAnimation();
        
    }
    else if(p.name == "removeads"){
        
        CCLOG("Purchase complete : remove_ads");
        
        UserDefault *userDefaults = UserDefault::getInstance();
        
        if(!userDefaults->getBoolForKey("removed")){
            
            userDefaults->setBoolForKey("removed", true);
            
            userDefaults->flush();
            
            appManager->removeads = userDefaults->getBoolForKey("removed");
        
        }
        
    }
    
    this->removeLoader();

    CCLOG("Purchase Success: %s", p.id.c_str());
}

void MagazaScene::onFailure(const Product &p, const std::string &msg)
{
    
    this->removeLoader();
    
    CCLOG("Purchase Failed: %s", msg.c_str());
}

void MagazaScene::onCanceled(const Product &p)
{
    
    this->removeLoader();
    
    CCLOG("Purchase Canceled: %s", p.id.c_str());
}

void MagazaScene::onRestored(const Product& p)
{
    
    this->removeLoader();
    
    if (p.name == "coinpackage") {
        
        CCLOG("Purchase complete: coin_package");
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 1000)));
        
        this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 1000), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon,appManager->userLost, appManager->userPoint);
        
        
        
    }
    else if (p.name == "coinpackage2") {
        
        CCLOG("Purchase complete: coin_package2");
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 5000)));
        
        this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 5000), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon,appManager->userLost, appManager->userPoint);
        
        
        
    }
    else if(p.name == "coinpackage3"){
        
        CCLOG("Purchase complete : coin_package3");
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 10000)));
        
        this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 10000), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon,appManager->userLost, appManager->userPoint);
        
        
        
    }
    else if(p.name == "coinpackage4"){
        
        CCLOG("Purchase complete : coin_package 4");
        
        goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 20000)));
        
        this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 20000), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon,appManager->userLost, appManager->userPoint);
        
        camExp->startAnimation();
        
    }
    else if(p.name == "removeads"){
        
        CCLOG("Purchase complete : remove_ads");
        
        UserDefault *userDefaults = UserDefault::getInstance();
        
        if(!userDefaults->getBoolForKey("removed")){
            
            userDefaults->setBoolForKey("removed", true);
            
            userDefaults->flush();
            
            appManager->removeads = userDefaults->getBoolForKey("removed");
            
        }
        
    }

    
    CCLOG("Purchase Restored: %s", p.name.c_str());
}

void MagazaScene::onProductRequestSuccess(const std::vector<Product> &products)
{
    this->removeLoader();
    
    updateIAP(products);
}

void MagazaScene::onProductRequestFailure(const std::string &msg)
{
    this->removeLoader();
    
    CCLOG("Fail to load products");
}

void MagazaScene::onRestoreComplete(bool ok, const std::string &msg)
{
    this->removeLoader();
    
    CCLOG("%s:%d:%s", __func__, ok, msg.data());
}


//app42

void MagazaScene::updateUserInformation(const char *username, double gold, double xp, double level, double avatar, double games, double won, double lost, double point){
    
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
    
    storageService->UpdateDocumentByKeyValue(db_name, collection_name, key, value, object, app42callback(MagazaScene::onStorageRequestCompleted, this));
    
}


void MagazaScene::onStorageRequestCompleted(void *response){
    
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

void MagazaScene::updateUserProfile(const char *jsonString){
    
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


//appwarp



void MagazaScene::onConnectDone(int res, int reasonCode){
    
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
void MagazaScene::recover(float dt)
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
        
        this->schedule(schedule_selector(MagazaScene::controlForConnection), 8.0f);
        
    }
    
}

void MagazaScene::controlForConnection(float dt){
    
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


void MagazaScene::removeNoConnectionWarning(){
    
    canSlide = true;
    
    this->removeChild(ayarlarPerde);
    
    this->removeChild(errorPanel);
    
}


void MagazaScene::noConnectionWarning(std::string st1, std::string st2, std::string st3){
    
    canSlide = false;
    
    ayarlarPerde = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde);
    
    ayarlarPerde->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel = Node::create();
    
    errorPanel->setPosition(Vec2(0,0));
    
    this->addChild(errorPanel);
    
    Sprite *panelBG = Sprite::create("Panel.png");
    
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
    
    Label *errorLabel1 = Label::createWithTTF(st1, "chinese rocks rg.ttf", 45);
    
    Label *errorLabel2 = Label::createWithTTF(st2, "chinese rocks rg.ttf", 45);
    
    Label *errorLabel3 = Label::createWithTTF(st3, "chinese rocks rg.ttf", 45);
    
    errorLabel1->setPosition(Vec2(WIDTH*.5,HEIGHT*.7));
    errorLabel2->setPosition(Vec2(WIDTH*.5,HEIGHT*.6));
    errorLabel3->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel->addChild(errorLabel1);
    
    errorPanel->addChild(errorLabel2);
    
    errorPanel->addChild(errorLabel3);
    
    if(waitForRecover == false){
        
        waitForRecover = true;
        
        this->unscheduleRecover();
        
        this->schedule(schedule_selector(MagazaScene::goBackMainForError),5.0f);
        
    }
    
}

void MagazaScene::goBackMainForError(float dt){
    
    this->unschedule(schedule_selector(MagazaScene::goBackMainForError));
    
    //****//
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
    
    
}

void MagazaScene::scheduleRecover()
{
    printf("\nGameScene::scheduleRecover");
    this->schedule(schedule_selector(MagazaScene::recover), 1.0f);
    
    //showReconnectingLayer("Reconnecting ...");
}

void MagazaScene::unscheduleRecover()
{
    printf("\nGameScene::unscheduleRecover");
    this->unschedule(schedule_selector(MagazaScene::recover));
}


//vungle

void MagazaScene::onVungleCacheAvailable() {
    
    
    
}


void MagazaScene::onVungleStarted() {
    
    
    
}


void MagazaScene::onVungleFinished() {
    
    
    
}


void MagazaScene::onVungleAdViewed(bool isComplete){
    
    
    
}


void MagazaScene::onVungleAdReward(const std::string& name){
    
    auto scheduler = Director::getInstance()->getScheduler();
    
    scheduler->performFunctionInCocosThread(CC_CALLBACK_0(MagazaScene::openRewardPanelThread, this));
    
    goldLabel->setString(StringUtils::format("%.0f",(appManager->gold + 25)));
    
    this->updateUserInformation(appManager->username.c_str(), (appManager->gold + 25), appManager->xp, appManager->level, appManager->avatarNumber, appManager->userGames, appManager->userWon, appManager->userLost, appManager->userPoint);
    
}

void MagazaScene::openRewardPanelThread() {
    
    if(appManager->currentLanguage() == false){
        
        this->openRewardPanel("25 ALTIN KAZANDIN");
        
    }
    else {
        
        this->openRewardPanel("25 GOLDS REWARD");
        
    }
    
}

void MagazaScene::removeRewardPanel(){
    
    this->removeChild(ayarlarPerde2);
    
    this->removeChild(errorPanel2);
    
}

void MagazaScene::openRewardPanel(std::string str) {
    
    ayarlarPerde2 = Sprite::create("blackBG.png");
    
    FIT_WH(ayarlarPerde2, WIDTH, HEIGHT);
    
    this->addChild(ayarlarPerde2, 16);
    
    ayarlarPerde2->setPosition(Vec2(WIDTH*.5, HEIGHT*.5));
    
    errorPanel2 = Node::create();
    
    errorPanel2->setPosition(Vec2(0, 0));
    
    this->addChild(errorPanel2,16);
    
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
    
    errorPanel2->addChild(panelBG,16);
    
    //Label *errorLabel1 = Label::createWithTTF(str, "chinese rocks rg.ttf", HEIGHT *.065);
    
    Label *errorLabel1 = Label::createWithBMFont("menuFont.fnt", str);
    
    FIT_H(errorLabel1, HEIGHT*.07);
    
    errorLabel1->setPosition(Vec2(WIDTH*.5,HEIGHT*.5));
    
    errorPanel2->addChild(errorLabel1,16);
    
    auto altinPng = Sprite::create("Altin.png");
    FIT_H(altinPng, HEIGHT*.08);
    altinPng->setPosition(Vec2(WIDTH*.35,HEIGHT*.51));
    errorPanel2->addChild(altinPng,16);
    
    
    
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
    
    
    tamamBut->setPosition(Vec2(WIDTH*.5, HEIGHT*.3));
    
    errorPanel2->addChild(tamamBut,16);
    
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
                
                this->removeRewardPanel();
            
                break;
                
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                
                break;
                
        }
        
        
    });

    
    
}









