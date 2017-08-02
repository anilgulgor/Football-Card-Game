//
//  Opponent.cpp
//  onlinegametest
//
//  Created by Anıl Gülgör on 16/10/15.
//
//

#include "Opponent.hpp"

USING_NS_CC;



Opponent *Opponent::create(std::string name, Vector<Kart *> opponentKarts){
    
    Opponent *opponent = new Opponent(name, opponentKarts);
    return opponent;
    
    
}

Opponent::Opponent(std::string name, Vector<Kart*> opponentKarts){
    
    _name = name;
    _opponentKarts = opponentKarts;
    
}