//
//  Kart.h
//  onlinegame
//
//  Created by Anils Mac on 14.10.2015.
//
//

#ifndef __onlinegame__Kart__
#define __onlinegame__Kart__

#include "cocos2d.h"
#include "AppManager.hpp"

USING_NS_CC;

using namespace cocos2d;




class Kart : public cocos2d::Node {

public:
    
    CREATE_FUNC(Kart);
    
    AppManager *appManager;
    
    void onEnterTransitionDidFinish();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    bool getIsPlayed(){
        
        return isPlayed;
        
    }
    
    void drawKartWith(std::string ulke, std::string name, int power);
    
    void drawUnplayedKart();
    
    void setKartNumber(int kartNum){
        
        _kartNum = kartNum;
        
    }
    
    int getKartNumber(){
        
        return _kartNum;
    }
    
    void setKartPower(int power){
        
        _power = power;
        
    }
    
    int getKartPower(){
        
        return _power;
        
    }
    
    void setName(std::string name){
        
        _name = name;
        
    }
    
    std::string getName(){
        
        return _name;
        
    }
    
    void setUlke(std::string ulke){
        
        _ulke = ulke;
        
    }
    
    std::string getUlke(){
        
        return _ulke;
        
    }
    
    
    int _power;
    
    Sprite *kartsprite, *oyuncuSprite, *kartOn, *kartBg, *flag;
    
private:
    
    
    Color4F color;
    
    std::string kartColorStr;
    
    int _kartNum;
    
    bool isPlayed;
    
    float WIDTH = GET_WIDTH;
    
    float HEIGHT = GET_HEIGHT;
  
    
    std::string _name;
    
    std::string _ulke;
    
    
};



#endif /* defined(__onlinegame__Kart__) */
