//
//  Opponent.hpp
//  onlinegametest
//
//  Created by Anıl Gülgör on 16/10/15.
//
//

#ifndef Opponent_hpp
#define Opponent_hpp

#include "cocos2d.h"
#include "Kart.h"

using namespace cocos2d;
USING_NS_CC;

class Opponent : public cocos2d::Ref {
    
    
public:
    
    static Opponent *create(std::string name, Vector<Kart*> opponentKarts);
    
    void setName(std::string name){
        
        _name = name;
        
    }
    
    std::string getName(){
        
        return _name;
        
    }
    
    void setOpponentKarts(Vector<Kart*>opponentKarts){
        
        _opponentKarts = opponentKarts;
        
    }
    
    Vector<Kart*> getOpponentKarts(){
        
        return _opponentKarts;
        
    }
    
private:
    
    std::string _name;
    Vector<Kart*> _opponentKarts;
    
    Opponent(std::string name, Vector<Kart *> opponentKarts);
    
    
};




#endif /* Opponent_hpp */

