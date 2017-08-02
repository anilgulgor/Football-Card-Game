//
//  Player.cpp
//  onlinegametest
//
//  Created by Anıl Gülgör on 15/10/15.
//
//

#include "Player.hpp"



Player *Player::create(std::string name, Vector<Kart *> playersDeck, bool isAdmin){
    
    Player *player = new Player(name, playersDeck, isAdmin);
    return player;
    
}


Player::Player(std::string name, Vector<Kart*>playersDeck, bool isAdmin){
    
    _name = name;
    _playersDeck = playersDeck;
    _isAdmin = isAdmin;
    
}


std::string Player::getName(){
    
    return _name;
    
}

Vector<Kart*> Player::getPlayerKarts() {
    
    return _playersDeck;
    
}


bool Player::getIsAdmin(){
 
    return _isAdmin;
    
}

void Player::setIsAdmin(bool isAdmin){
    
    _isAdmin = isAdmin;
    
}
