//
//  Kart.cpp
//  onlinegame
//
//  Created by Anils Mac on 14.10.2015.
//
//

#include "Kart.h"


//bazı şeyler tekrar yazılacak...
//yeni proje. santra. menunode değişecek. mantıklı performanslı bir yapı kurulacak...

USING_NS_CC;

using namespace std;

void Kart::onEnterTransitionDidFinish() {
    
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Kart::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Kart::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Kart::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    
}


void Kart::drawKartWith(std::string ulke, std::string name, int power){
    
    appManager = AppManager::getInstance();
    
    isPlayed = false;
    
    std::string nation;
    
    nation = ulke + ".png";
    
    this->setKartPower(power);
    
    this->setUlke(ulke);
        
    if(power == 100){
        
        kartBg = Sprite::create();
        
    }
    else if(power > 92){
    
        kartBg = Sprite::create("Efsane Texture.png");
        
    }
    else if(power > 84){
        
        kartBg = Sprite::create("Gold Texture.png");
        
    }
    else if(power<= 84 && power > 80){
        
        kartBg = Sprite::create("Silver Texture.png");
        
    }
    else if(power <= 80){
        
        kartBg = Sprite::create("Bronz Texture.png");
        
    }
    
    this->addChild(kartBg);
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(kartBg, HEIGHT*.27);
            
        }
        else {
            
            FIT_H(kartBg, HEIGHT*.34);
            
        }
        
    }
    else {
        
        FIT_H(kartBg, HEIGHT*.34);
        
    }
    
    
    kartBg->setPosition(Vec2(0,HEIGHT*.024));
    
    this->setName(name);
    
    std::string fname;
    
    fname = name + ".png";
    
    if(power != 100){
    
        kartsprite = Sprite::create(fname);
    
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
            if(appManager->isIpad() == true){
            
                FIT_H(kartsprite, HEIGHT*.25);
            
            }
            else {
            
                FIT_H(kartsprite, HEIGHT*.32);
            
            }
        
        }
        else {
        
            FIT_H(kartsprite, HEIGHT*.32);
        
        }
    }
    else {
        
        kartsprite = Sprite::create(fname);
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(kartsprite, HEIGHT*.27);
                
            }
            else {
                
                FIT_H(kartsprite, HEIGHT*.34);
                
            }
            
        }
        else {
            
            FIT_H(kartsprite, HEIGHT*.34);
            
        }
        
    }
    
    
    this->addChild(kartsprite);
    
    if(power != 100){
    
        kartOn = Sprite::create("KartOn.png");
    
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
            if(appManager->isIpad() == true){
            
                FIT_H(kartOn, HEIGHT*.27);
            
            }
            else {
            
                FIT_H(kartOn, HEIGHT*.34);
            
            }
        
        }
        else {
        
            FIT_H(kartOn, HEIGHT*.34);
        
        }
        
    }
    else {
        
        kartOn = Sprite::create("KartOn.png");
        
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            
            if(appManager->isIpad() == true){
                
                FIT_H(kartOn, HEIGHT*.27);
                
            }
            else {
                
                FIT_H(kartOn, HEIGHT*.34);
                
            }
            
        }
        else {
            
            FIT_H(kartOn, HEIGHT*.34);
            
        }

        
        kartOn->setOpacity(0);
        
    }
    
    this->addChild(kartOn);
    
    //bayrak
    
    if(power != 100){
    
        flag = Sprite::create(nation);
    
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
            if(appManager->isIpad() == true){
            
                FIT_H(flag, HEIGHT*.08);
            
                flag->setPosition(Vec2(-WIDTH*.038,HEIGHT*.124));
            
            }
            else {
            
                FIT_H(flag, HEIGHT*.08);
            
                flag->setPosition(Vec2(-WIDTH*.038,HEIGHT*.124));
            
            }
        
        }
        else {
        
            FIT_H(flag, HEIGHT*.08);
        
            flag->setPosition(Vec2(-WIDTH*.038,HEIGHT*.124));
        
        }
    
    }
    else {
        
        flag = Sprite::create();
        
    }
    
    this->addChild(flag);
    
    if(power < 100){
    
        Label *footballerName = Label::createWithTTF(name, "footballerFont.ttf", HEIGHT*.023);
        footballerName->enableGlow(Color4B::BLACK);
        footballerName->enableShadow();
    
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
            if(appManager->isIpad() == true){
            
                footballerName->setPosition(Vec2(0,-HEIGHT*.1));
            
            }
            else {
            
                footballerName->setPosition(Vec2(0,-HEIGHT*.126));
            
            }
        
        }
        else {
    
            footballerName->setPosition(Vec2(0,-HEIGHT*.126));
        
        }
    
        this->addChild(footballerName);
    
        //for power
    
        stringstream powerStr;
        powerStr<<power;
    
        std::string powerLabelStr = powerStr.str();
    
        this->setKartPower(power);
    
        Label *footballerPower = Label::createWithTTF(powerLabelStr, "chinese rocks rg.ttf", HEIGHT*.035);
    
        footballerPower->enableGlow(Color4B::BLACK);
        
        footballerPower->enableShadow();
    
        footballerPower->enableOutline(Color4B(0, 0, 0, 255));
    
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
            if(appManager->isIpad() == true){
            
                footballerPower->setPosition(Vec2(WIDTH*.04,HEIGHT*.119));
            
            }
            else {
            
                footballerPower->setPosition(Vec2(WIDTH*.04,HEIGHT*.119));
            
            }
        
        }
        else {
    
            footballerPower->setPosition(Vec2(WIDTH*.04,HEIGHT*.119));
        
        }
    
        this->addChild(footballerPower);
    
    }
   
   
}

void Kart::drawUnplayedKart(){
    
    kartsprite = Sprite::create("KartArka.png");
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
        
        if(appManager->isIpad() == true){
            
            FIT_H(kartsprite, HEIGHT*.28);
            
        }
        else {
            
            FIT_H(kartsprite, HEIGHT*.38);
            
        }
        
    }
    else {
    
        FIT_H(kartsprite, HEIGHT*.38);
        
    }
    this->addChild(kartsprite);
    
}

bool Kart::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){


    if(kartOn){
    
    Point nodePoint = kartOn->convertToNodeSpaceAR(touch->getLocation());
    
    if(kartOn->getBoundingBox().containsPoint(nodePoint)){
        
        kartsprite->setOpacity(128);
        
        if(_power < 100){
        
            kartOn->setOpacity(128);
            
        }
        
        kartBg->setOpacity(128);
        
    }
    
    }

    return true;
}

void Kart::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(kartOn){
    
        Point nodePoint = kartOn->convertToNodeSpaceAR(touch->getLocation());
    
        if(!kartOn->getBoundingBox().containsPoint(nodePoint)){
        
            kartsprite->setOpacity(255);
            
            if(_power < 100){
            
                kartOn->setOpacity(255);
                
            }
            
            kartBg->setOpacity(255);
        
        }
    
    }
    
}

void Kart::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    
    if(kartsprite->getOpacity() == 128){
        
        if(AppManager::getInstance()->isUserAdmin == true){
        
            this->isPlayed = true;
            
        }
        
        kartsprite->setOpacity(255);
        
        if(_power < 100){
        
            kartOn->setOpacity(255);
            
        }
        
        kartBg->setOpacity(255);
    }
    
}


