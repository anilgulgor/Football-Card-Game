//
//  Footballer.hpp
//  onlinegametest
//
//  Created by Anıl Gülgör on 26/10/15.
//
//

#ifndef Footballer_hpp
#define Footballer_hpp

#include "cocos2d.h"
#include "Kart.h"
#include <stdio.h>


using namespace std;
using namespace cocos2d;
USING_NS_CC;

class Footballer : public cocos2d::Ref {
    

public:
    
    static Footballer *create(std::string name , std::string ulke, int power);
    
    void setName(std::string name);
    std::string getName();
    
    void setUlke(std::string ulke);
    std::string getUlke();
    
    void setPower(int power);
    int getPower();
    
    
private:
    
    std::string _name;
    std::string _ulke;
    int _power;
    
    Footballer(std::string name , std::string ulke , int power);
    
    
};

#endif /* Footballer_hpp */
