//
//  Footballer.cpp
//  onlinegametest
//
//  Created by Anıl Gülgör on 26/10/15.
//
//

#include "Footballer.hpp"


//ULKELER STRING OLACAK!!!!!!!!

Footballer *Footballer::create(std::string name, std::string ulke, int power){
    
    Footballer *footballer = new Footballer(name, ulke, power);
    return footballer;
    
}

Footballer::Footballer(std::string name, std::string ulke, int power){
    
    _name = name;
    _ulke = ulke;
    _power = power;
    
}

void Footballer::setName(std::string name){
    
    _name = name;
    
}
std::string Footballer::getName(){
    
    return _name;
    
}

void Footballer::setUlke(std::string ulke){
    
    _ulke = ulke;
    
}
std::string Footballer::getUlke(){
    
    return _ulke;
    
}


void Footballer::setPower(int power){
    
    _power = power;
    
}
int Footballer::getPower(){
    
    return _power;
    
}