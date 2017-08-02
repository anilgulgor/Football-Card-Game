//
//  Player.hpp
//  onlinegametest
//
//  Created by Anıl Gülgör on 15/10/15.
//
//

#ifndef Player_hpp
#define Player_hpp

#include "cocos2d.h"
#include "Kart.h"

using namespace cocos2d;


class Player : public cocos2d::Ref {
    
    
    //constructor isAdmin boolean. eğer adminse hangisinin başlayacağını gönderecek karşıya. 1 ise admin başlayacak. 0 ise opponent başlayacak. ok onu gösterecek.
    
    
public:
    
    static Player *create(std::string name, Vector<Kart*>playersDeck, bool isAdmin);
    
    void showPlayerKartsOnScreen();
    std::string getName();
    Vector<Kart*> getPlayerKarts();
    Vector<Kart*>_playersDeck;
    
    void setPlayerKarts(Vector<Kart*> tempKarts){
        
        _playersDeck = tempKarts;
        
    }
   
    bool _isAdmin;
    
    bool getIsAdmin();
    
    void setIsAdmin(bool isAdmin);
    
private:
    
    std::string _name;
    
    
    
    Player(std::string name, Vector<Kart*>playersDeck, bool isAdmin);
    
    
    
};


#endif /* Player_hpp */
